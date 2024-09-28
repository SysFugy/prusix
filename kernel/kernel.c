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
#include "../progs/init.c"

void kmain(void) {
	init(0);	// Initialize IDT
	init(1);	// Prepare
	init(2);	// Logo
	init(3);	// Logs
	init(4);	// LLShell
	
	return;
}
