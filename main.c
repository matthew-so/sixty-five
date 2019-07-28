#include <stdio.h>
#include "mem.h"

int main() {
    write(2, 3);
    write(1, 2);
    u8 t = 255;
    printf("%u", (u16) (u8) ((u8)read(0+1)-t));
}