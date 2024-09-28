////////////////////////////////////////////////
//                                            //
//                   init.c                   //
//	         Logs and system starting         //
//                                            //
////////////////////////////////////////////////

/////////////////// Main loop //////////////////

void init(const int mode) {
        /////////// Mode 0, Preparing //////////
	if (mode == 0) clearscreen();
	
	////////////// Mode 1, Logs ////////////
	else if (mode == 1) {
		//check_mem();
		check_acpi();
		check_idt();
		check_ps2();
		//check_gdt();

		idt_init();
		initGDT();

		newline();
	}
	
        ///////////// Mode 2, Logo /////////////
	else if(mode == 2) {
		const char *logo[5] = {
			" _____             _     ",
			"|  _  |___ _ _ ___|_|_ _ ",
			"|   __|  _| | |_ -| |_'_|",
			"|__|  |_| |___|___|_|_,_|",
			"             v0.3-Zemgale"};
	int i = 0;
	while(i < 5) {
		if (i == 4) println(logo[i++], 8); 
		else println(logo[i++], 7);
	}
	
	newline();
	i = 0;
	
	//////////// Mode 3, LLShell ///////////
	} else if (mode == 3) {
		newline();
		for(;;) {		
			char input_buffer[BUFFER_SIZE];
			print("psh-> ", 15);
			scan(input_buffer, BUFFER_SIZE, 15);

			if (strcmp(input_buffer, "panic") == 0) init(4);
			else if(strcmp(input_buffer, "test_printf") == 0){printf("%s%d\n", "string, next: number - ", 10);}
			else if(strcmp(input_buffer, "test_mouse") == 0){printf("%s\n", "---> Running mouse test..."); scanMouse();}
			else if(strcmp(input_buffer, "int") == 0) __asm__ volatile("int $27");
			else if(strcmp(input_buffer, "fill") == 0) fill_bg();
			else if(strcmp(input_buffer, "idt") == 0) ping_idt();
			else if(strcmp(input_buffer, "gdt") == 0) ping_gdt();
			else if(strcmp(input_buffer, "triangle") == 0) triangle();
			else if (strcmp(input_buffer, "off") == 0) init(5);
			else if (strncmp(input_buffer, "mode", 4) == 0) {
				int mode;
				if (sscanf_c(input_buffer + 5, &mode)) init(mode);
			}

			else if(strcmp(input_buffer, "") == 0) {
				// Ignoring
			} 
			
			else panic("Unknown command", "Unknown command panic!");
		}
	} 
	
	///////////// Mode 4, Panic ////////////
	else if(mode == 4) {
		sod("909_SOD_TESTERROR", "THIS IS A TEST SOD, PRESS ANY KEY TO LEAVE"); anykey();
	}
	
	//////////// Mode 5, PowerOff //////////
	else if(mode == 5) acpi_off();
	else sod("908_MODEERROR", "INVALID INIT MODE, PRESS ANY KEY TO LEAVE");

}
