#ifndef FS_H
#define FS_H
#include "../include/memory.h"
#include "../include/stddef.h"
#include "../include/math.h"
#include "vga.h"

typedef struct {
    int32_t next;
    char block[1024];
} FAT_BLOCK;
typedef struct {
    int FAT_TABLE[20];
    FAT_BLOCK blocks[20];
} FAT;
void read_fat(FAT* fat, int start);
#endif