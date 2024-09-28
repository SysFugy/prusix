////////////////////////////////////////////////
//                                            //
//                   msg.h                    //
//	    Small libary for logging          //
//                                            //
////////////////////////////////////////////////

/////////////////// Print log //////////////////

char plog (char *text) {
	print(" - [  ", 0b111);
	print("LOG", 0b010);
	print("  ] ", 0b111);
	println(text, 0b011);
}

////////////////// Print panic /////////////////

char panic (char *text, char *desc) {
	print(" - [ ", 0b111);
	print("PANIC", 0b100);
	print(" ] ", 0b111);
	println(text, 0b110);
	println(desc, 0b110);
}

///////////////// Throw SOD ////////////////////

char sod (char* code, char* text){
	uint8_t color = 0b001;

	clearscreen_bsod();
	printcolor(" <- [ BSOD: ERROR CODE ", color);
	printcolor(code, color);
	printcolor(": ", color);
	printcolor(text, color);
	printcolor(" ] -> PLEASE REPORT THIS TO SOMEONE", color);
}
