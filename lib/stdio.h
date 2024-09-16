////////////////////////////////////////////////
//                                            //
//                   stdio.h                  //
//	        Small I/O libary              //
//                                            //
////////////////////////////////////////////////

/////////////////// Variables //////////////////

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define BG_COLOR 0b000

unsigned int xPos = 0;
unsigned int yPos = 0;

uint16_t* vidptr = (uint16_t*)0xB8000;

//////////////// Scancodes array ///////////////

uint8_t KEYBOARD_SCANCODES[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    // 0-9
    '9', '0', '-', '=', '\b',                         // 10-14 (Backspace)
    '\t',                                             // 15 (Tab)
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', // 16-25
    '[', ']', '\n',                                   // 26-28 (Enter)
    0,                                                // 29 (Control)
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', // 30-39
    '\'', '`', 0,                                     // 40-42 (Left shift)
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',               // 43-49
    'm', ',', '.', '/', 0,                            // 50-54 (Right shift)
    '*', 0, ' ', 0,                                   // 55-58 (Space bar, Alt, Caps lock)
    0, 0, 0, 0, 0, 0, 0, 0,                           // 59-66 (F1-F10)
    0, 0, 0, 0, '-',                                  // 67-71 (Num Lock, Scroll Lock, arrows)
    0, 0, '+',                                        // 72-75 (arrows, End, Page keys)
    0, 0, 0, 0, 0,                                    // 76-80 (Insert, Delete)
    0, 0, 0, 0, 0,                                    // 81-85 (undefined keys)
    0, 0, 0, 0, 0, 0, 0                               // 86-127 (F11, F12, undefined keys)
};

uint8_t KEYBOARD_SCANCODES_UPPER[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*',    // 0-9
    '(', ')', '_', '+', '\b',                         // 10-14 (Backspace)
    '\t',                                             // 15 (Tab)
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', // 16-25
    '{', '}', '\n',                                   // 26-28 (Enter)
    0,                                                // 29 (Control)
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', // 30-39
    '\"', '~', 0,                                     // 40-42 (Left shift)
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',                // 43-49
    'M', '<', '>', '/', 0,                            // 50-54 (Right shift)
    '*', 0, ' ', 0,                                   // 55-58 (Space bar, Alt, Caps lock)
    0, 0, 0, 0, 0, 0, 0, 0,                           // 59-66 (F1-F10)
    0, 0, 0, 0, '-',                                  // 67-71 (Num Lock, Scroll Lock, arrows)
    0, 0, '+',                                        // 72-75 (arrows, End, Page keys)
    0, 0, 0, 0, 0,                                    // 76-80 (Insert, Delete)
    0, 0, 0, 0, 0,                                    // 81-85 (undefined keys)
    0, 0, 0, 0, 0, 0, 0                               // 86-127 (F11, F12, undefined keys)
};

////////////////// Clear screen ////////////////

uint16_t mask_symbol(uint8_t color, char c){// color - 0b000 - RGB; char - char to be masked
	//return (uint16_t)((uint8_t)color << 4) << 8 | (uint16_t)c;
	return (uint16_t)(color << 4 | (uint8_t)0b1111) << 8 | (uint16_t)c;
}

uint16_t mask_symbol_full(uint8_t color, char c){
	return ((uint16_t)color << 8) | (uint16_t)c;
}

void clearscreen(void) {
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = mask_symbol(BG_COLOR, ' ');
	}

	xPos = 0;
	yPos = 0;
}

void clearscreen_bsod(void) {
	for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++){
		vidptr[i] = mask_symbol(0b001, ' ');
	}

	xPos = 0;
	yPos = 0;
}

/////////////////// Scroll up //////////////////

void scroll_up(void) {
	clearscreen(); // not realized yet
}

////////////////// Add 1 symbol to xPos || (xPos && yPos) ///

void pospp(bool newline){
	if(!newline){
		xPos++;

		if(xPos >= VGA_WIDTH){yPos++;}
	}
	else{
		xPos = 0;
		yPos++;
	}

	if(yPos >= VGA_HEIGHT){clearscreen();}
}

void posmm(void){
	xPos--;

	if(xPos < 0){
		xPos = VGA_WIDTH;
		yPos--;
	}

	if(yPos < 0){clearscreen();}
}

//////////////////// Newline ///////////////////

void newline(void) {
	pospp(true); // add new line
}

/////// Boolean Variables for uppercase ////////

bool shift_pressed = false;
bool caps_lock_active = false;

//////////////////// Get char //////////////////

char __getch() {
    while (!(inb(0x64) & 0x01)) {
        // Waiting for buffer
    }
    
    // Scancodes reading
    unsigned char scancode = inb(0x60);

    // Key release ignoring
    if (scancode & 0x80) {
        unsigned char keycode = scancode & 0x7F;
        if (keycode == 0x2A || keycode == 0x36) shift_pressed = false; // Shift releasing
        return 0;
	
    } else {
        if (scancode == 0x2A || scancode == 0x36) shift_pressed = true;  // Shift
        else if (scancode == 0x3A) caps_lock_active = !caps_lock_active; // CAPSLOCK

        const char *scancodes = (shift_pressed || caps_lock_active) ? KEYBOARD_SCANCODES_UPPER : KEYBOARD_SCANCODES;
        char ascii_char = scancodes[scancode]; // Getting ASCII
        return ascii_char;
    }
}

void anykey(){ // gde tut na keyborde etot anykey?
	char c;
	sleep(10);
	while(1){
		c = __getch();

		if(c == 0) continue;

		clearscreen();
		break;
	}
}

/////////////////// Scan input /////////////////

void scanMouse(){
	outb(0x60, 0xF4);

	println("-> Waiting for mouse response...", 15);

	while(inb(0x64) & 1 == 0){}

	uint16_t res = inb(0x64);

	char* buf;
	its(res, 10, buf);

	if(res != 0xFA){println("OK, received data from mouse, displaying it below!", 15); println(buf, 10);}
	else{println("STRANGE, received 0xFA from mouse, maybe its controller is broken...?", 12);}
}

void scan(char *buffer, unsigned int buffer_size, const int color) {
    unsigned int index = 0;
    char c;
    
    while (1) {
        c = __getch();

        // Key release
        if (c == 0) continue;
        
        // Enter
        if (c == '\n') {
            buffer[index] = '\0';
            newline();
            break;
        }
        
        // Backspace
        else if (c == '\b') { 
            if (index > 0) {
                index--;
                vidptr[yPos * VGA_WIDTH + xPos - 1] = mask_symbol(BG_COLOR, ' ');
		posmm();
            }
        }
        
        // Non-printing chars
        else if (c == 0) continue;
        
        // Adding char to the buffer
        else {
            if (index < buffer_size - 1) {
                buffer[index++] = c;
                vidptr[yPos * VGA_WIDTH + xPos] = mask_symbol(BG_COLOR, c);
		pospp(false);
            }
        }
        
        // Scrolling
        if (yPos == VGA_HEIGHT) scroll_up();
    }
}

///////////////////// UITS /////////////////////

void uits(uint32_t number, char *buffer, size_t buffer_size) {
    if (buffer_size == 0) return;

    char temp[32];
    int i = 0;

    do {
        temp[i++] = '0' + (number % 10);
        number /= 10;
    } while (number > 0);

    int start = 0;
    int end = i - 1;

    while (end >= 0 && start < buffer_size - 1) buffer[start++] = temp[end--];
    buffer[start] = '\0';
}

///////////////// Print string ////////////////

void print(const char *str, int color) {
    unsigned int i = 0;
    while (str[i] != '\0') {
        vidptr[yPos * VGA_WIDTH + xPos] = mask_symbol(BG_COLOR, str[i]);
	pospp(false);

        i++;
    }

    if (yPos >= VGA_HEIGHT) scroll_up();
}

////////////// Print with newline //////////////

void println(const char *str, const int color) {
	print(str,color);
	newline();
}

void printcolor(char* str, uint16_t data){
	while(*str != '\0'){
		vidptr[yPos * VGA_WIDTH + xPos] = mask_symbol(data, *str);
		*str++;
	}
}

///////////////// Int to string ////////////////

void its(int32_t i, uint8_t base, char *buf) {
	bool negative = false;
	if(i < 0) {
		negative = true;
		i *= -1;
	}
	
	int32_t index = 0;
	do {
		int32_t remainder = i % base;
		buf[index++] =
				(remainder > 9) ? (remainder - 10) + 'A' : remainder + '0';
		i /= base;
	} while (i > 0);
	
	if(negative) { buf[index++] = '-'; }
	buf[index] = '\0';
}
