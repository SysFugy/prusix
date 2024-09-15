////////////////////////////////////////////////
//                                            //
//                   msg.h                    //
//	    Small libary for logging          //
//                                            //
////////////////////////////////////////////////

/////////////////// Print log //////////////////

char plog (char *text) {
	print(" - [  ", 15);
	print("LOG", 10);
	print("  ] ", 15);
	println(text, 15);
}

////////////////// Print panic /////////////////

char panic (char *text) {
	print(" - [ ", 15);
	print("PANIC", 12);
	print(" ] ", 15);
	println(text, 15);
}

//////////////// Throw BSOD ////////////////////

char bsod (char* code, char* text){
	uint16_t color = 0b00011111;

	clearscreen_bsod();
	printcolor(" <- [ BSOD: ERROR CODE ", color);
	printcolor(code, color);
	printcolor(": ", color);
	printcolor(text, color);
	printcolor(" ] -> PLEASE REPORT THIS TO SOMEONE", color);
}
