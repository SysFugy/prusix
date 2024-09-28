void triangle() {
    for (int i = 1; i <= 10; i++) {
        for (int j = i; j < 10; j++) print(" ", 12);
        for (int j = 1; j <= (2 * i - 1); j++) print("#", 12);
        newline();
    }
}
