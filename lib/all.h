////////////////////////////////////////////////
//                                            //
//                   all.h                    //
//          All libaries in 1 file            //
//                                            //
////////////////////////////////////////////////

#ifndef ALL_H
#define ALL_H

#define KCODE_START 0x1BADB002

#include "acpi.h"
#include "io.h"
#include "string.h"
#include "time.h"

#include "stdlib.h"
#include "stdio.h"
#include "msg.h"
#include "check_drivers.h"

#include "../libc/all.h"

#include "./fun/ghoul.h"
#include "./IDT/idt.h" // not yet.
#include "./GDT/gdt.h"

#endif 
