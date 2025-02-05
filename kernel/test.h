#include "../drivers/vga.h"
#include "../drivers/port.h"
#include "../drivers/keyboard.h"
#include "../include/String.h"
#include "../include/memory.h"
#include "../include/math.h"



void get_data() {
    // Variables to hold CPUID results
    char vendor[13];
    unsigned int eax, ebx, ecx, edx;

    // Get Vendor ID
    eax = 0;  // CPUID with EAX = 0 returns the vendor ID
    asm volatile (
        "cpuid"                // CPUID instruction
        : "=b"(ebx), "=c"(ecx), "=d"(edx)  // Outputs: EBX, ECX, and EDX will contain the vendor ID
        : "a"(eax)             // Input: EAX = 0
    );

    // Combine the parts to form the vendor string
    vendor[0] = (char)(ebx & 0xFF);
    vendor[1] = (char)((ebx >> 8) & 0xFF);
    vendor[2] = (char)((ebx >> 16) & 0xFF);
    vendor[3] = (char)((ebx >> 24) & 0xFF);

    vendor[4] = (char)(edx & 0xFF);
    vendor[5] = (char)((edx >> 8) & 0xFF);
    vendor[6] = (char)((edx >> 16) & 0xFF);
    vendor[7] = (char)((edx >> 24) & 0xFF);

    vendor[8] = (char)(ecx & 0xFF);
    vendor[9] = (char)((ecx >> 8) & 0xFF);
    vendor[10] = (char)((ecx >> 16) & 0xFF);
    vendor[11] = (char)((ecx >> 24) & 0xFF);

    vendor[12] = '\0';  // Null-terminate the string

    // Print the vendor ID
    vga_print_string("CPU Vendor: ");
    vga_print_string(vendor);
    vga_print_string("\n");

    // Get Processor Info (Features)
    eax = 1;  // CPUID with EAX = 1 returns processor features
    asm volatile (
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)  // Outputs: EAX, EBX, ECX, EDX will contain feature flags
        : "a"(eax)                                     // Input: EAX = 1
    );

    // Check if the processor supports SSE, for example, by checking the SSE flag in EDX
    if (edx & (1 << 25)) {
        vga_print_string("SSE supported\n");
    } else {
        vga_print_string("SSE not supported\n");
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