////////////////////////////////////////////////
//                                            //
//                   stdio.h                  //
//	        Small I/O libary              //
//                                            //
////////////////////////////////////////////////

/////////////////// Variables //////////////////

unsigned int current_loc = 0;
char *vidptr = (char*)0xb8000;

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

void clearscreen(void) {
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07;
	}
}

/////////////////// Scroll up //////////////////

void scroll_up(void) {
	clearscreen();
   	unsigned int i;
    	for (i = current_loc; i < SCREENSIZE; i++) vidptr[i - current_loc] = vidptr[i];
	current_loc = 0;
}

//////////////////// Newline ///////////////////

void newline(void) {
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));
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

/////////////////// Scan input /////////////////


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
                vidptr[current_loc - 2] = ' ';
                vidptr[current_loc - 1] = color;
                current_loc -= BYTES_FOR_EACH_ELEMENT;
            }
        }
        
        // Non-printing chars
        else if (c == 0) continue;
        
        // Adding char to the buffer
        else {
            if (index < buffer_size - 1) {
                buffer[index++] = c;
                vidptr[current_loc++] = c;
                vidptr[current_loc++] = color;
            }
        }
        
        // Scrolling
        if (current_loc >= SCREENSIZE - BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE) scroll_up();
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
        if (current_loc >= SCREENSIZE) scroll_up();
        vidptr[current_loc++] = str[i];
        vidptr[current_loc++] = color;
        i++;
    }
    
    if (current_loc >= SCREENSIZE) scroll_up();
}

////////////// Print with newline //////////////

void println(const char *str, const int color) {
	print(str,color);
	newline();
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