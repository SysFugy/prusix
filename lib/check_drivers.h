////////////////////////////////////////////////
//                                            //
//             check_drivers.h                //
//             Second mode logs               //
//                                            //
////////////////////////////////////////////////

//////////////////// RAM ///////////////////////

void check_mem() {
    unsigned int memory_size;

    asm (
        "movl $0x8800, %%eax\n"
        "int $0x15\n"
        "movl %%eax, %0\n"
        : "=r" (memory_size)
        :
        : "%eax"
    );

    plog("Memory found");
}

///////////////////// ACPI /////////////////////

#define ACPI_RSDP_SIGNATURE "RSD PTR "

typedef struct {
    char signature[8];
    unsigned char checksum;
    char oem_id[6];
    char revision;
    unsigned int rsdt_address;
    unsigned int length;
    unsigned long long xsdt_address;
    unsigned char extended_checksum;
    unsigned char reserved[3];
} __attribute__((packed)) acpi_rsdp_t;

bool find_rsdp(acpi_rsdp_t **rsdp) {
    unsigned int base = 0xE0000;
    unsigned int end = 0xFFFFF;

    for (unsigned int addr = base; addr <= end; addr += 16) {
        acpi_rsdp_t *candidate = (acpi_rsdp_t *)addr;

        if (candidate->signature[0] == 'R' &&
            candidate->signature[1] == 'S' &&
            candidate->signature[2] == 'D' &&
            candidate->signature[3] == ' ' &&
            candidate->signature[4] == 'P' &&
            candidate->signature[5] == 'T' &&
            candidate->signature[6] == 'R' &&
            candidate->signature[7] == ' ') {
            *rsdp = candidate;
            return true;
        }
    }
    return false;
}

void check_acpi(void) {
    acpi_rsdp_t *rsdp = NULL;

    if (find_rsdp(&rsdp)) {
        unsigned char sum = 0;
        unsigned char *ptr = (unsigned char *)rsdp;
        for (unsigned int i = 0; i < rsdp->length; i++) sum += ptr[i];

        if (sum == 0) plog("ACPI is working");
        else panic("0x0000009F");
    } else panic("0x00000124");
}

//////////////////// VGA ///////////////////////


bool vga_support() {
    uint8_t mode;
    
    asm (
        "mov $0x00, %%ah\n"
        "mov $0x03, %%al\n"
        "int $0x10\n"
        "mov %%al, %0\n"
        : "=r" (mode)
        :
        : "ax"
    );
    
    return mode == 0x03;     
}

void check_vga() {
	if(vga_support()) plog("VGA is working");
	else panic("0x0000009F");
}

///////////////////// PS/2 /////////////////////

#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64
#define PS2_DATA_PORT 0x60

#define PS2_STATUS_OUTPUT_BUF_FULL 0x01
#define PS2_STATUS_INPUT_BUF_FULL 0x02
#define PS2_STATUS_SYSTEM_FLAG 0x04
#define PS2_STATUS_COMMAND_DATA 0x08
#define PS2_STATUS_TIMEOUT_ERROR 0x40
#define PS2_STATUS_PARITY_ERROR 0x80

int check_ps2() {
    uint8_t status = inb(PS2_STATUS_PORT);
    if (status & PS2_STATUS_PARITY_ERROR) panic("0x00000000");
    if (status & PS2_STATUS_TIMEOUT_ERROR) panic("0x00000000");
    plog("PS/2 is working");
}
