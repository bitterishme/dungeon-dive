#include <string.h>
#include <ctype.h>
#include "stringManip.h"

char *str_trim(char *s) {
    // Skip leading whitespace
    while(isspace(*s)) s++;
    
    // If string is empty, return it
    if (*s == 0) return s;
    
    // Find end of string
    char *end = s + strlen(s) - 1;
    
    // Trim trailing whitespace
    while(end > s && isspace(*end)) end--;
    
    // Null terminate the string
    *(end + 1) = 0;
    
    return s;
}

char *str_cut(char *s, int start, int end) {
    // Check if indices are valid
    if (start < 0 || end < start || end >= strlen(s)) {
        return NULL;
    }
    
    // Move to start position
    s += start;
    
    // Null terminate at end position
    s[end - start + 1] = '\0';
    
    return s;
}