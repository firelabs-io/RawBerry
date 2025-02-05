#include "../drivers/ata.h"
uint8_t abuffer[512];
void disk_write(uint32_t lba, const void* buffer){
    const uint8_t* data = (const uint8_t*)buffer;
    for (int i = 0; i < 512; i++) {
        abuffer[i] = data[i];  // Copy byte-by-byte
    }
    ata_write(lba, abuffer);
    for (int i = 0; i < 512; i++) {
        abuffer[i] = 0;  // Reset each byte to zero
    }
}

void disk_read(uint32_t lba, const void* buffer){
    ata_read(lba, abuffer);
    uint8_t* dest = (uint8_t*)buffer;
    for (int i = 0; i < 512; i++) {
        dest[i] = abuffer[i];  // Copy byte-by-byte from 'abuffer' to 'buffer'
    }
    for (int i = 0; i < 512; i++) {
        abuffer[i] = 0;  // Reset each byte to zero
    }
}