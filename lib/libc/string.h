////////////////////////////////////////////////
//                                            //
//                  string.h                  //
//	        String formatting             //
//                                            //
////////////////////////////////////////////////

//////////////////// STRCMP ////////////////////

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

//////////////////// STRNCMP ///////////////////

int strncmp(const char *str1, const char *str2, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) if (str1[i] != str2[i] || str1[i] == '\0' || str2[i] == '\0') return (unsigned char)str1[i] - (unsigned char)str2[i];
    return 0;
}

//////////////////// STRCSPN ///////////////////

size_t strcspn(const char *str, const char *reject) {
    const char *p;
    const char *q;

    for (p = str; *p; p++) for (q = reject; *q; q++) if (*p == *q) return p - str;
    return p - str;
}