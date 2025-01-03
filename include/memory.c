#include "memory.h"
#include "stddef.h"
#include "../include/math.h"

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

void memory_print(memory_entry* memory_table, int32_t size) {
    int i = 0;
    while (i < size) {
        
        if (memory_table[i].start_addr != 0 && memory_table[i].end_addr != 0) {
            vga_print_string("block: ");
            vga_print_string(itol(memory_table[i].start_addr));  
            vga_print_string(" ");
            vga_print_string(itol(memory_table[i].end_addr));    
            vga_print_string(" ");
            
            
            char* memory_contents = memre(memory_table[i].start_addr, memory_table[i].end_addr);
            vga_print_string(memory_contents);  
            vga_print_string("\n");
        }
        
        i++;
    }
}


void memory_write(memory_entry* memory_table, int32_t start, int32_t end, int32_t index, const char* string){
    memwr(string, start, end);
    memory_table[index].start_addr = start;
    memory_table[index].end_addr = end;
}

void memory_delete(memory_entry* memory_table, int32_t index) {
    uint8_t* ptr = (uint8_t*)memory_table[index].start_addr;
    uint8_t* end = (uint8_t*)memory_table[index].end_addr;
    while (ptr < end) {
        *ptr = 0;  
        ptr++;
    }
    
    memory_table[index].start_addr = 0;
    memory_table[index].end_addr = 0;
}
void memory_info(memory_entry* memory_table, int32_t size) {
    int32_t total_memory = 0;

    // Iterate over all the memory blocks in the table
    for (int i = 0; i < size; i++) {
        total_memory += (memory_table[i].end_addr - memory_table[i].start_addr);
    }

    // Calculate and print the total memory usage
    if (total_memory < 1024) {
        vga_print_string(itol(total_memory));
        vga_print_string(" bytes\n");
    } else if (total_memory < 1024 * 1024) { // Less than 1 MB
        vga_print_string(itol(total_memory / 1024));
        vga_print_string(" KB\n");
    } else if (total_memory < 1024 * 1024 * 1024) { // Less than 1 GB
        vga_print_string(itol(total_memory / (1024 * 1024)));
        vga_print_string(" MB\n");
    } else { // 1 GB or more
        vga_print_string(itol(total_memory / (1024 * 1024 * 1024)));
        vga_print_string(" GB\n");
    }
}
