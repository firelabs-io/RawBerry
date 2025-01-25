#include <stdint.h>
#include "vga.h"
void print_hex(uint8_t byte) {
    char hex_digits[] = "0123456789ABCDEF";
    
    vga_put_char(hex_digits[(byte >> 4) & 0xF]);  // Print the high nibble
    vga_put_char(hex_digits[byte & 0xF]);         // Print the low nibble
    vga_put_char(' ');                            // Print the separator
}
// Write a 16-bit word to an I/O port
static inline void outw(uint16_t port, uint16_t value) {
    __asm__ volatile ("outw %0, %1" : : "a"(value), "dN"(port));
}
static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}


static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile (
        "inb %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}

void wait_for_ready() {
    while (inb(0x1F7) & 0x80) {  
        
    }
}


void ata_write(uint32_t lba, const void* buffer) {
    outb(0x1F2, 0x01);               
    outb(0x1F3, (uint8_t)(lba & 0xFF));            
    outb(0x1F4, (uint8_t)((lba >> 8) & 0xFF));    
    outb(0x1F5, (uint8_t)((lba >> 16) & 0xFF));   
    outb(0x1F6, 0xE0);               

    outb(0x1F7, 0x30);               

    wait_for_ready();                

    
    const uint16_t* buf = (const uint16_t*)buffer;
    for (int i = 0; i < 256; i++) {
        outw(0x1F0, buf[i]);
    }

    
    wait_for_ready();
}

void ata_read(uint32_t lba, void* buffer) {
    outb(0x1F2, 0x01);                           // Sector count
    outb(0x1F3, (uint8_t)(lba & 0xFF));          // LBA low byte
    outb(0x1F4, (uint8_t)((lba >> 8) & 0xFF));   // LBA mid byte
    outb(0x1F5, (uint8_t)((lba >> 16) & 0xFF));  // LBA high byte
    outb(0x1F6, 0xE0);                           // Drive and command (primary master)

    outb(0x1F7, 0x20);                           // Read command
    wait_for_ready();                            // Wait for the drive to be ready

    uint16_t* buf = (uint16_t*)buffer;           // Treat buffer as 16-bit
    for (int i = 0; i < 256; i++) {              // Read 256 words (512 bytes)
        buf[i] = inw(0x1F0);                     // Use `inw` for word read

        char char1 = (char)(buf[i] & 0xFF);       // Lower byte
        char char2 = (char)((buf[i] >> 8) & 0xFF); // Upper byte

        vga_put_char((char1 >= 32 && char1 <= 126) ? char1 : '.');
        vga_put_char((char2 >= 32 && char2 <= 126) ? char2 : '.');
    }
    vga_print_string("\n");
}