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
	descriptor->kernel_cs = KCODE_START;
	descriptor->attributes = flags;
	descriptor->isr_high = (uint32_t)isr >> 16;
	descriptor->reserved = 0;
}

void get_idt(idtr_t* p){
	__asm__ volatile("sidt %0" : "=m"(*p));
}

void idt_init(){
	__asm__ volatile("cli");

	idtr_t address = {(uint16_t)sizeof(idtr_t) * 254, &vectors[0]};

	plog("Setting up IRQ handlers...");

	for(int vector = 0; vector < 32; vector++){
		idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
		print(".", 0b010);
		sleep(1);
	}
	newline();

	println("-> DONE!", 0b010);

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

void ping_idt(void){
	int i;
	int ok = 0;
	int okidexes[255];

	for(i = 0; i < 255; i++){
		if(vectors[i].isr_low > 0 || vectors[i].isr_high > 0){plog("IDT ENTRY OK!"); ok++; okidexes[i] = i;}
		else{panic("IDT ENTRY BAD!");}

		sleep(1);
	}

	for(int i = 0; i < 255; i++){if(okidexes[i] > 0){printf("%d%s\n", okidexes[i], " is OK.");}}
	printf("%d%s\n", ok, "/255 entries OK.");
}
