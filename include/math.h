#include "../include/string.h"
#include "../drivers/vga.h"

char* itol(int num) {
    static char buffer[32];
    int i = 30;

    if (num == 0) buffer[i--] = '0';

    if (num < 0) {
        buffer[i--] = '-';
        num = -num;
    }

    while (num > 0) {
        buffer[i--] = num % 10 + '0';
        num /= 10;
    }

    return &buffer[i + 1];
}

int stol(const char* str) {
    int result = 0;

    while (*str) {
        result = result * 10 + (*str++ - '0');
    }

    return result;
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
