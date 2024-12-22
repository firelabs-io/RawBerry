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

void memory_info(const int32_t start_address, int32_t last_address){
    int32_t result = last_address - start_address;
    if (result < 1024) {
        vga_print_string(itol(result));
        vga_print_string(" bytes\n");
    } else if (result < 1024 * 1024) { // Less than 1 MB
        vga_print_string(itol(result / 1024));
        vga_print_string(" KB\n");
    } else if (result < 1024 * 1024 * 1024) { // Less than 1 GB
        vga_print_string(itol(result / (1024 * 1024)));
        vga_print_string(" MB\n");
    } else { // 1 GB or more
        vga_print_string(itol(result / (1024 * 1024 * 1024)));
        vga_print_string(" GB\n");
    }
}
