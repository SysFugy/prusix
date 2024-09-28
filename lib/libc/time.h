////////////////////////////////////////////////
//                                            //
//                    time.h                  //
//	        Time manipulating             //
//                                            //
////////////////////////////////////////////////

///////////////////// Sleep ////////////////////

void sleep(const int count) {
	int x = 0;
	while(x != count) {
		__asm__ volatile (
			"mov $100000, %ecx\n"
			"1:\n"
			"rep nop\n"
			"loop 1b\n"
		);
		x++;
	}
}
