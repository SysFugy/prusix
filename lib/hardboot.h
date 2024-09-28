void force_reboot(){
    idtr_t fake_idt;
    fake_idt.base = 0;
    fake_idt.limit = 0;

    __asm__ volatile("lidt %0" :: "m"(fake_idt));
    __asm__ volatile("int 15");
}
