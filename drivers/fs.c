#include "../include/memory.h"
#include "../include/stddef.h"
#include "../include/math.h"
#include "vga.h"
#include "fs.h"

void read_fat(FAT *fat, int start){
    int block_index = fat->FAT_TABLE[start];
    int block_count = 0;
    while (block_index != -1) {
        vga_print_string("Reading block ");
        vga_print_string(itol(block_count));
        vga_put_char('\n');
        vga_print_string(fat->blocks[block_index].block);
        vga_put_char('\n');
        block_index = fat->blocks[block_index].next;
        block_count++;
    }
}