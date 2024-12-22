#ifndef MEMORY_H
#define MEMORY_H
#include "stddef.h"
typedef struct {    
    uint32_t start_addr;  
    uint32_t end_addr;    
} memory_entry;
void memwr(const char* string, int32_t start, int32_t end);
char* memre(int32_t start, int32_t end);
void memptv(int32_t* end, int32_t new);
#endif