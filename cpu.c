#include "mem.h"

u8 a;
u8 x;
u8 y;

u16 pc;
u8 sp;
u8 ir;

u8[8] sr;

u8 cycle;

u16 acc(void) {
    return 0;
}

u16 abs(void) {
    return (u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8);
}

u16 absx(void) {
    return (u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8) + x;
}

u16 absy(void) {
    return (u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8) + y;
}

u16 imm(void) {
    return read((u16)(pc+1));
}

u16 impl(void) {
    return 0;
}

u16 ind(void) {
    return read((u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8));
}

u16 xind(void) {
    return read((u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8) + x);
}

u16 indy(void) {
    return read((u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8)) + y;
}

u16 rel(void) {
    return pc + read((u16)(pc+1));
}

u16 zpg(void) {
    return read((u16)(pc+1));
}

u16 zpgx(void) {
    return (u16) (read((u16)(pc+1))+x);
}

u16 zpgy(void) {
    return (u16) (read((u16)(pc+1))+y);
}

void update() {
    if (cycle < )
}