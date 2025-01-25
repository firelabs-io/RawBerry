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
#include "../drivers/ata.h"
#include "test.h"
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
uint8_t buf[512];
void ata_test_write(){
    uint8_t a[] = {104, 101, 108, 108, 111, 111};
    for (int i = 0; i < 512; i++) {
        if (i < 5){
            buf[i] = a[i];
        }
        else{
            buf[i] = 0;
        }
    }
    uint32_t lba = 100;
    ata_write(lba, buf);
}
void ata_test_read(){
    uint32_t lba = 100;
    ata_read(lba, buf);
}

void palloc(size_t bytes, memory_entry* memory_table, int size) {
    size_t aligned_size = (bytes + 7) & ~7;

    for (int i = 0; i < size; i++) {
        if (memory_table[i].start_addr == 0 && memory_table[i].end_addr == 0) {
            vga_print_string("found empty block");

            if (i > 0) {
                memory_table[i].start_addr = memory_table[i - 1].end_addr;
            } else {
                memory_table[i].start_addr = 0x400000;
            }

            memory_table[i].end_addr = memory_table[i].start_addr + aligned_size;

            vga_print_string("Allocated block from ");
            vga_print_string(itol(memory_table[i].start_addr));
            vga_print_string(" to ");
            vga_print_string(itol(memory_table[i].end_addr));
            vga_put_char('\n');
            return;
        }
    }

    vga_print_string("No available memory blocks for allocation.\n");
}

void pfree(memory_entry* memory_table, int size, int index){
    if (index < size && index >= 0){
        memory_table[index].start_addr = 0;
        memory_table[index].end_addr = 0;
        vga_print_string("freed the block\n");
    }
}

void mem_test(){
    memory_entry memory_table[16] = {};
    palloc(4, memory_table, 16);
    memory_print(memory_table, 16);
    vga_put_char('\n');
    pfree(memory_table, 16, 0);
    memory_print(memory_table, 16);
}

void launch_kernel(void) {
    vga_initialize();
    vga_print_string("Welcome to RawBerry OS!\n");
    vga_print_string("[YANE TERMINAL 1.0.0]\n");
    memory_entry memory_table[16] = {};
    mem_test();

    const int32_t start_address = 0x400000; // keep it as const
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
        } else if(strEql(strSlice(Keyboard_buffer, 0, 3), "get") == 0){
            get_data();
        } 
    }
    return;
}

