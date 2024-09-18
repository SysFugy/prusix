////////////////////////////////////////////////
//                                            //
//                  kernel.c                  //
//	           Main file                  //
//                                            //
////////////////////////////////////////////////

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

#include "cfg.c"
#include "../lib/all.h"

#include "../progs/init/init.c"

const uint8_t magic = 0;

extern void* isr_stub_table[];

__attribute__((aligned(0x10)))
static GDT gdt[GDT_SIZE];

static GDTR gdtr = {(uint16_t)sizeof(GDT) * GDT_SIZE - 1, &gdt};
static idtr_t idtr;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

GDTR getGDT(){
	GDTR gdt;
	__asm__ volatile("sgdt %0" : "=m"(gdt));

	return gdt;
}

void initGDT(){
	__asm__ volatile("cli");

	plog("Loading GDT...");

	__asm__ volatile("lgdt %0" :: "m"(gdtr));

	GDTR address = getGDT();

	//printf("%d%s%d", address.offset, "/", address.size);

	setGDTEntry(0, 0, 0, 0, 0);
	setGDTEntry(1, magic, 0xFFFF, 0x9A, 0xC0);
	//setGDTEntry(2, -, 0xFFFFF, 0x92, 0xC0);
	if(address.offset == gdt){plog("-> OK!");}
	else{bsod("GDT_ERR_CODE", "FAILED TO SET GDT ADDRESS"); __asm__("hlt");}
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
	idt_entry_t* e = &idt[vector];

	e->isr_low = (uint32_t)isr & 0xFFFF;
	e->kernel_cs = 0x08; // todo
	e->attributes = flags;
	e->isr_high = (uint32_t)isr >> 16;
	e->reserved = 0;

	/*
	char* num;
	char* addr;

	uits((int)vector, num, sizeof(num));
	uits((int)isr, addr, sizeof(addr));

	print("descriptor ", 0b111);
	print(num, 0b111);
	print(": ", 0b111);
	println(addr, 0b111);
	*/
}

void idt_init(void);
void idt_init(){
	idtr.base = (uint32_t)&idt;
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * 255 - 1;

	for(uint8_t vector = 0; vector < 32; vector++){
		idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
	}

	__asm__ volatile("lidt %0" :: "m"(idtr));
	plog("INITED IDT!");
	//__asm__ volatile("sti");
}

void ping_idt(){
	for(int i = 0; i < 32; i++){
		if(idt[i].kernel_cs > 0){plog("OK!");}
		else{panic("NOT OK!");}
	}
}

void setGDTEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity){
	GDT e;
	gdt[index].base_low = (base & 0xFFFF);
	gdt[index].base_middle = (base >> 16) & 0xFF;
	gdt[index].base_high = (base >> 24) & 0xFF;

	gdt[index].limit_low = limit & 0xFFFF;

	gdt[index].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);

	gdt[index].access = access;
}

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
	panic("EXCEPTION!");
}

void kmain(void) {
	init(0);	// Initialize IDT
	init(1);	// Prepare
	init(2);	// Logo
	init(3);	// Logs
	init(4);	// LLShell
	
	return;
}
