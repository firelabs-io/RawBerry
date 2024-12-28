/* 
 * ---------------------------------------------------------------
 *                            RawBerry
 * (c) RawBerry Team
 * Published on license MIT. Read `/LICENSE` for more details.
 * ---------------------------------------------------------------
*/
#include "main.h"
#include "../drivers/vga.h"
#include "../drivers/port.h"
#include "../drivers/keyboard.h"
#include "../include/String.h"
#include "../include/memory.h"
#include "../include/ascii.h"
#include "../include/math.h"
#include "../drivers/fs.h"
#include "../include/stdio.h" // :: There is no standard yet, either switch to "stdio.h" or comment it out until
//                               :: there is implementation for it
char* commands[] = {
    "echo %1", "prints argument", // 0 1
    "clear", "clears screen", // 2 3
    "help", "display help", // 4 5
    "logo", "shows rawberry logo", // 6 7
    "about", "about the os", // 8 9
    "write %1", "write string to memory", // 10 11
    "read", "read from memory", // 12 13
    "mem-usage", "print memory usage (from write)",
};
void display() {
    for (int i = 0; i < 8; i++) {  
        vga_print_string(commands[i * 2]);   
        vga_print_string(": ");
        vga_print_string(commands[i * 2 + 1]); 
        vga_print_string("\n");
    }
}

void fat_test(){
    FAT fat;
    fat.FAT_TABLE[0] = 0;
    fat.blocks[0].next = 1;
    for (int i = 0; i < 12; i++) {
        fat.blocks[0].block[i] = 'A';
    }
    fat.FAT_TABLE[1] = 1;
    fat.blocks[1].next = -1;
    for (int i = 0; i < 12; i++) {
        fat.blocks[1].block[i] = 'B';
    }
    read_fat(&fat, 0);
}


void launch_kernel(void) {
    vga_initialize();
    vga_print_string("Welcome to RawBerry OS!\n");
    vga_print_string("[YANE TERMINAL 1.0.0]\n");
    memory_entry memory_table[15] = {};

    const int32_t start_address = 0x400000; // keep it as const
    int32_t last_address = 0x400000 + Strlen("Hello, RawBerryOS!");
    int32_t last = 1;
    // to use end_adress do: start_address + Strlen(string)

    char* Keyboard_buffer;
    while (1) {
        vga_print_string(": ");
        Keyboard_buffer = readStr();

        if (strEql(strSlice(Keyboard_buffer, 0, 4), "echo") == 0) {
            char* sliced = strSlice(Keyboard_buffer, 5, Strlen(Keyboard_buffer));
            vga_print_string(sliced);
            
        } else if (strEql(strSlice(Keyboard_buffer, 0, 5), "clear") == 0) {
            vga_clear_screen();

        } else if (strEql(strSlice(Keyboard_buffer, 0, 4), "help") == 0) {
            display();

        } else if (strEql(strSlice(Keyboard_buffer, 0, 4), "logo") == 0) {
            logo();

        } else if (strEql(strSlice(Keyboard_buffer, 0, 5), "about") == 0) {
            print_info();

        } else if (strEql(strSlice(Keyboard_buffer, 0, 5), "write") == 0) {
            char* buffer = strSlice(Keyboard_buffer, 6, Strlen(Keyboard_buffer));
            memory_write(memory_table, last_address, last_address+Strlen(buffer), last, buffer);
            last++;
            last_address = last_address + Strlen(buffer);
            vga_print_string("Written to memory.\n");

        } else if (strEql(strSlice(Keyboard_buffer, 0, 4), "read") == 0) {
            memory_print(memory_table, 15);
            char* buffer = memre(start_address, last_address);
            //vga_print_string("Read: ");
            //vga_print_string(buffer);
        } else if (strEql(strSlice(Keyboard_buffer, 0, 5), "crash") == 0){
            asm volatile ("ud2");
        } else if (strEql(strSlice(Keyboard_buffer, 0, 9), "mem-usage") == 0){
            memory_info(memory_table, 15);
        } else if (strEql(strSlice(Keyboard_buffer, 0, 6), "delete") == 0){
            const char* buffer = strSlice(Keyboard_buffer, 7, Strlen(Keyboard_buffer)-1);
            int index = stol(buffer);
            memory_delete(memory_table, index);
            vga_print_string("memory deleted\n");
        } else if (strEql(strSlice(Keyboard_buffer, 0, 3), "fat") == 0){
            fat_test();
        }
    }
    return;
}

