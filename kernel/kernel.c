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

void ping_gdt(){
	if(!(gdt[1].limit_low)){panic("NO GDT DETECTED!");}
	else{plog("GDT OK!");}
}

GDTR getGDT(){
}

void initGDT(){
	setGDTEntry(0, 0, 0, 0, 0);
	setGDTEntry(1, magic, 0xFFFF, 0x9A, 0xC0);

	__asm__ volatile("lgdt %0" :: "m"(gdtr));
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
	idt_entry_t* e = &idt[vector];

	e->isr_low = (uint32_t)isr & 0xFFFF;
	e->kernel_cs = 0x08;
	e->attributes = flags;
	e->isr_high = (uint32_t)isr >> 16;
	e->reserved = 0;
}

void idt_init(void);
void idt_init(){
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * 254;

	for(int i = 0; i < 32; i++){
		if(isr_stub_table[i]){
			idt_set_descriptor(i, isr_stub_table[i], 0x8E);
		}
		else{bsod("ERR_NO_IVEC", "NO IVEC FOUND!");}
	}

	__asm__ volatile("lidt %0" :: "m"(idtr));
}

void ping_idt(){
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
	bsod("ERR_EXCEPTION_GAY", "CPU EXCEPTION! ANY KEY TO POWER OFF!");
	anykey();
	acpi_off();
}

void kmain(void) {
	init(0);	// Initialize IDT
	init(1);	// Prepare
	init(2);	// Logo
	init(3);	// Logs
	init(4);	// LLShell
	
	return;
}
