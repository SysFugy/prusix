typedef struct{
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t reserved;
	uint8_t attributes;
	uint16_t isr_high;
}__attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;

extern void* isr_stub_table[];

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];
static idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* e = &idt[vector];

    e->isr_low = (uint32_t)isr & 0xFFFF;
    e->kernel_cs = 0x08;
    e->attributes = flags;
    e->isr_high = (uint32_t)isr >> 16;
    e->reserved = 0;
}

void idt_init(void) {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)(sizeof(idt_entry_t) * 256 - 1);

    for(int i = 0; i < 32; i++) {
        if(isr_stub_table[i]) idt_set_descriptor(i, isr_stub_table[i], 0x8E);
        else bsod("ERR_NO_IVEC", "NO IVEC FOUND!");
    }

    __asm__ volatile("lidt %0" :: "m"(idtr));
}

void exception_handler(void) {
    bsod("ERR_EXCEPTION", "CPU EXCEPTION! ANY KEY TO POWER OFF!");
    anykey();
    acpi_off();
}
