#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "stringManip.h"

char* str_trim(char* str) {
    char *s = str;
    while(*s && isspace(*s)) s++;
    
    if (*s == 0) return s;  // Handle empty string
    
    char *end = s + strlen(s) - 1;
    while(end > s && isspace(*end)) end--;
    *(end + 1) = '\0';
    
    return s;
}

char* str_cut(char* str, int start, int end) {
    if (!str || start < 0 || end < start || end >= strlen(str)) {
        return NULL;
    }
    
    int len = end - start + 1;
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    strncpy(result, str + start, len);
    result[len] = '\0';
    
    return result;
}