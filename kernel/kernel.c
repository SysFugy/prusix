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

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
	bsod("ERR_CPU_EXCEPTION", "CPU INTERRUPT DETECTED!");
	__asm__("hlt");
}

void kmain(void) {
	init(0);	// Initialize IDT
	init(1);	// Prepare
	init(2);	// Logo
	init(3);	// Logs
	init(4);	// LLShell
	
	return;
}
