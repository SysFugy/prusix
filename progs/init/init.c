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
		//check_vga();
		check_ps2();

		initGDT();
		idt_init();

		newline();
	}
	
        ///////////// Mode 2, Logo /////////////
	else if(mode == 2) {
		const char *logo[5] = {
			" _____             _     ",
			"|  _  |___ _ _ ___|_|_ _ ",
			"|   __|  _| | |_ -| |_'_|",
			"|__|  |_| |___|___|_|_,_|",
			"              Version 0.2"};
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
			print("LLShell > ", 15);
			scan(input_buffer, BUFFER_SIZE, 15);

			if (strcmp(input_buffer, "panic") == 0) init(4);
			else if(strcmp(input_buffer, "test_bsod") == 0){bsod("909_TESTERROR", "THIS IS A TEST BSOD, PRESS ANY KEY TO LEAVE"); anykey();}
			else if(strcmp(input_buffer, "test_printf") == 0){printf("%s%d\n", "string, next: number - ", 10);}
			else if(strcmp(input_buffer, "test_mouse") == 0){printf("%s\n", "---> Running mouse test..."); scanMouse();}
			else if(strcmp(input_buffer, "int") == 0){printf("%d\n", 0 / 0); while (1);}
			else if(strcmp(input_buffer, "im_ghoul") == 0){ghoul();}
			else if (strcmp(input_buffer, "off") == 0) init(5);
			else if (strncmp(input_buffer, "mode", 4) == 0) {
				int mode;
				if (sscanf_c(input_buffer + 5, &mode)) init(mode);
			}

			else if(strcmp(input_buffer, "") == 0) {
				// Ignoring
			} 
			
			else panic("Unknown command");
		}
	} 
	
	///////////// Mode 4, Panic ////////////
	else if(mode == 4) {
		panic("Panic!");
		while(1) {
			// Wang >:3
		}
	}
	
	//////////// Mode 5, PowerOff //////////
	else if(mode == 5) acpi_off();
	else {
		panic("Unknown mode");
	}

}
