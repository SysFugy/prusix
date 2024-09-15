const uint16_t GDTADDRESS = 0xC1000;
uint8_t* GDTStaticPointer = (uint8_t*)GDTADDRESS;
uint8_t* GDTpointer = (uint8_t*)GDTADDRESS;

typedef struct{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) GDTR;

typedef struct{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed)) GDT;

GDT gdt[8];

// GDTR is a structure that should POINT to GDT, it can be passed to LGDT

GDTR getGDT(){
	GDTR gdt;

	__asm__ volatile("sgdt %0" : "=m"(gdt));

	return gdt;
}

void initGDT(){
	__asm__ volatile("cli");

	GDTR gdtr = {sizeof(GDT) * 8, gdt};
	plog("Loading GDT...");
	__asm__("lgdt %0" :: "m"(gdtr));

	GDTR address = getGDT();
	
	setGDTEntry(0, 0, 0, 0, 0);
	//setGDTEntry(1, 0xB8000, 0xFFFFF, 0x9A, 0xC0);
	//setGDTEntry(2, 0xB8000, 0xFFFFF, 0x92, 0xC0);

	if(address.offset == gdt){plog("-> OK!");}
	else{bsod("GDT_ERR_CODE", "FAILED TO SET GDT ADDRESS"); __asm__("hlt");}
}

/*
void encodeGdtEntry(uint8_t *target, struct GDT source)
{
	if (source.limit > 0xFFFFF) {kerror("GDT cannot encode limits larger than 0xFFFFF");}

	target[0] = source.limit & 0xFF;
	target[1] = (source.limit >> 8) & 0xFF;
	target[6] = (source.limit >> 16) & 0x0F;

	target[2] = source.base & 0xFF;
	target[3] = (source.base >> 8) & 0xFF;
	target[4] = (source.base >> 16) & 0xFF;
	target[7] = (source.base >> 24) & 0xFF;

	target[5] = source.access_byte;

	target[6] |= (source.flags << 4);
}*/

void setNextGDTEntry(uint8_t entry){
	*GDTpointer = entry;
	GDTpointer++;
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
