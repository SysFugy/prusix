////////////////////////////////////////////////
//                                            //
//                   cfg.c                    //
//                  Config                    //
//                                            //
////////////////////////////////////////////////

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define PANIC_TIME 15
#define BUFFER_SIZE 256

#define ENTER_KEY_CODE 0x1C
#define BACKSPACE_KEY_CODE 0x0E
#define SCAN_CODE_SHIFT

extern void disable_interrputs();
extern void enable_interrputs();
