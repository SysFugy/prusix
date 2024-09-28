////////////////////////////////////////////////
//                                            //
//                   acpi.h                   //
//                ACPI Driver                 //
//                                            //
////////////////////////////////////////////////

/////////////////// Shutdown ///////////////////

void acpi_off() {
    unsigned short port = 0x604;
    unsigned short value = 0x2000;
    outb(port, value);
    //__asm__ volatile ("outw %0, %1" :: "a"(value), "Nd"(port));
    while (1) {
        __asm__ volatile ("hlt");
    }
}
