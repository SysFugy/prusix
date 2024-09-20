
;                    kernel.asm                 
;	            Initialization              

bits 32
section .text
        align 4
        dd 0x1BADB002              	
        dd 0x00                    	
        dd - (0x1BADB002 + 0x00)   	

global port_byte_in
global start
extern kmain 				

port_byte_in:
	mov dx, word [esp + 4]
	in al, dx
	ret
start:
	cli 				
	mov esp, stack_space		
	call kmain
	hlt 				

section .bss
resb 8192

stack_space:
