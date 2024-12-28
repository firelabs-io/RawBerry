#ifndef MEMORY_H
#define MEMORY_H
#include "stddef.h"
#include "../include/math.h"
typedef struct {    
    uint32_t start_addr;  
    uint32_t end_addr;    
} memory_entry;
extern void memwr(const char* string, int32_t start, int32_t end);
extern char* memre(int32_t start, int32_t end);
extern void memptv(int32_t* end, int32_t new);
extern void memory_print(memory_entry* memory_table, int32_t size);
extern void memory_write(memory_entry* memory_table, int32_t start, int32_t end, int32_t index, const char* string);
extern void memory_delete(memory_entry* memory_table, int32_t index);
extern void memory_info(memory_entry* memory_table, int32_t size);
#endif