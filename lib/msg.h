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