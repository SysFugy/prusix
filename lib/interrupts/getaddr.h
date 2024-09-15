void isr_no_err_stub(void);
void isr_err_stub(void);

extern void* isr_stub_table[];

typedef struct{
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t reserved;
	uint8_t attributes;
	uint16_t isr_high;
} __attribute__((packed)) idt_entry_t;

typedef struct{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(0x10)))
idt_entry_t vectors[255];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags){
	idt_entry_t* descriptor = &vectors[vector];

	descriptor->isr_low = (uint32_t)isr & 0xFFFF;
	descriptor->kernel_cs = 0x00;
	descriptor->attributes = flags;
	descriptor->isr_high = (uint32_t)isr >> 16;
	descriptor->reserved = 1;
}

void get_idt(idtr_t* p){
	__asm__ volatile("sidt %0" : "=m"(*p));
}

void idt_init(){
	__asm__ volatile("cli");

	idtr_t address = {(uint16_t)sizeof(idtr_t) * 254, &vectors[0]};

	plog("Setting up IRQ handlers...");
	for(uint8_t vector = 0; vector < 32; vector++){
		idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
	}

	plog("Registering IDT structure...");
	__asm__ volatile("lidt %0" :: "m"(address));

	idtr_t p;

	get_idt(&p);

	char* baseChar;
	char* newChar;

	uits(address.base, baseChar, sizeof(baseChar));
	uits(p.base, newChar, sizeof(newChar));

	if(p.base == address.base){plog("DONE setting up IDT!");}
	else{panic("FAILED to set up IDT!"); bsod("ERR_IDT_FAIL", "p.base != address.base (/lib/interrupts/getaddr.h)"); __asm__("hlt");}

	//__asm__ volatile("sti");
}
