#include <stdio.h>
#include "mem.h"

unsigned char test[65536] = {0};

u8 read(u16 add) {
    return test[add];
/*
    if (add & 0x1000) {
        return readR(add & 0x0FFF);
    } else {
        if (add & 0x0080) {
            if (add & 0x0200) {

            } else {
                return readA(add & 0x007F);
            }
        } else {
            return readT(add & 0x003F);
        }
    }
*/
}

void write(u16 loc, u8 add) {
    test[loc] = add;
}

