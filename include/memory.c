#include "memory.h"
#include "stddef.h"
#define MAX_SIZE 256


void memwr(const char* string, int32_t start, int32_t end){
    unsigned char* ptr = (unsigned char*)start;  
    while (*string && (ptr < (unsigned char*)end)) {
        *ptr = *string;    
        string++;          
        ptr++;             
    }
}
char* memre(int32_t start, int32_t end){
    unsigned char* ptr = (unsigned char*)start;  
    static char buffer[MAX_SIZE];
    int32_t i = 0;
    while (ptr < (unsigned char*)end && i < 1024) {
        buffer[i] = *ptr;  
        ptr++;             
        i++;               
    }

    
    buffer[i] = '\0';
    return buffer;   
}

void memptv(int32_t* end, int32_t new){
    *end = new;
}