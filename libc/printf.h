void printf(char* format, ...){
	va_list args;

	va_start(args, format);

	while(*format != '\0'){
		switch(*format++){
			case 's':
				print(va_arg(args, const char*), 0b111);
				break;
			case 'd':
				char* num;
				uits(va_arg(args, int), num, sizeof(char*));
				print(num, 15);
				break;
			case '\n':
				newline();
				break;
			default:
				break;
		}
	}
}
