void ghoul(){
	char* str;
	for(int i = 1000; i > 0; i -= 7){
		uits(i, str, sizeof(char*));

		println(str, 15);
		
		sleep(1);
	}
}
