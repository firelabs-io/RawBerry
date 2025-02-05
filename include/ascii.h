#include "../drivers/vga.h"

char* ascii_art[] = {
    "                %*%  %#@        ",
    "                #=+%@++#@       ",
    "            @%%%#+==*+=%%%@@%@@ ",
    "         @#=+****==*+=======+%  ",
    "        @*+***+******==*+=%%    ",
    "       @**+*+**=**++****=++++*@ ",
    "      @%****+*=+*=**=**=***#*#  ",
    "      @**++********=**+=*++%    ",
    "      @=+**++*=*=+*****+*++@    ",
    "      %*+=**********-*+=**#@    ",
    "      %-+**=*+***+=*=+***@@     ",
    "      %*********=+****%@@       ",
    "      %=+=***-+***#%@@          ",
    "      @#*####%%%%@              ",
    "        @                       ",
    "                                ",
};

void logo(){
    for (int i = 0; i < sizeof(ascii_art) / sizeof(ascii_art[0]); i++) {
        vga_print_string(ascii_art[i]);
        vga_print_string("\n");
    }
}

void print_info(){
    vga_clear_screen();
    logo();
    vga_print_string("rawberry is a os, lightweight and simple by RawBerry Team.'\n");
    vga_print_string("version(running):dev-7071A\n");
}