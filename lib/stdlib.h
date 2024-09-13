////////////////////////////////////////////////
//                                            //
//                  stdlib.h                  //
//             Homemande STD libary           //
//                                            //
////////////////////////////////////////////////

//////////////// Checking chars ////////////////

static bool isitdigit(char c) {
    return (c >= '0' && c <= '9');
}

static bool isitspace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

///////////////////// Sscanf ///////////////////

bool sscanf_c(const char *str, int *value) {
    const char *p = str;
    int result = 0;
    bool negative = false;

    while (isitspace(*p)) p++;
    if (*p == '-') {
        negative = true;
        p++;
    } else if (*p == '+') {
        p++;
    }
    if (!isitdigit(*p)) return false;

    while (isitdigit(*p)) {
        result = result * 10 + (*p - '0');
        p++;
    }

    *value = negative ? -result : result;
    while (isitspace(*p)) p++;
    return *p == '\0';
}