KERNEL_DIR := ../kernel
OUTPUT_DIR := ./boot
ISO_NAME := prusix_v0-2.iso
KERNEL_ASM := $(KERNEL_DIR)/kernel.asm
KERNEL_C := $(KERNEL_DIR)/kernel.c
KERNEL_ASM_OBJ := ./kernel_asm.o
KERNEL_OBJ := ./kernel.o
LINKER_SCRIPT := link.ld
ISO_OUTPUT := ./$(ISO_NAME)

REQUIRED_TOOLS := nasm gcc ld grub-mkrescue qemu-system-i386 find

all: check_tools compile_kernel build_kernel build_iso

check_tools:
	@for tool in $(REQUIRED_TOOLS); do \
		echo "[Prusix] Checking $$tool..."; \
		$$tool --version; \
	done

compile_kernel: $(KERNEL_ASM_OBJ) $(KERNEL_OBJ)

$(KERNEL_ASM_OBJ): $(KERNEL_ASM)
	echo "[Prusix] Compiling assembly initialization part..."; \
	nasm -f elf32 $(KERNEL_ASM) -o $(KERNEL_ASM_OBJ)

$(KERNEL_OBJ): $(KERNEL_C)
	echo "[Prusix] Compiling kernel..."; \
	gcc -fno-stack-protector -m32 -c $(KERNEL_C) -o $(KERNEL_OBJ)

build_kernel: $(KERNEL_ASM_OBJ) $(KERNEL_OBJ)
	echo "[Prusix] Linking libraries..."; \
	ld -m elf_i386 -T $(LINKER_SCRIPT) -o prusix $(KERNEL_ASM_OBJ) $(KERNEL_OBJ)

build_iso: build_kernel
	echo "[Prusix] Creating ISO..."; \
	mkdir -p $(OUTPUT_DIR); \
	cp prusix $(OUTPUT_DIR)/; \
	grub-mkrescue -o ${ISO_NAME} .

clean:
	echo "[Prusix] Cleaning..."; \
	rm -f *.o prusix

run: all
	echo "[Prusix] Starting QEMU..."; \
	qemu-system-i386 -m 40M -cdrom $(ISO_OUTPUT)

.PHONY: all check_tools compile_kernel build_kernel build_iso clean run
