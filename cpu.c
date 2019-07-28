#include "mem.h"

u8 a;
u8 x;
u8 y;

u16 pc;
u8 sp = 255;
u8 ir;

enum {A, I, M} am;

struct {
    unsigned int n: 1;
    unsigned int v: 1;
    unsigned int b: 1;
    unsigned int d: 1;
    unsigned int i: 1;
    unsigned int z: 1;
    unsigned int c: 1;
} sr;

u8 cycle;

u16 acc(void) {
    am = A;
    return 0;
}

u16 abs(void) {
    am = M;
    return (u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8);
}

u16 absx(void) {
    am = M;
    return (u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8) + x;
}

u16 absy(void) {
    am = M;
    return (u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8) + y;
}

u16 imm(void) {
    am = I;
    return read((u16)(pc+1));
}

u16 impl(void) {
    return 0;
}

u16 ind(void) {
    am = M;
    return read((u16) read((u16)(pc+1)) + (read((u16)(pc+2)) << 8));
}

u16 xind(void) {
    am = M;
    return read((u16) (read((u16)(pc+1))+x));
}

u16 indy(void) {
    am = M;
    return read((u16) (read((u16)(pc+1)))) + y;
}

u16 rel(void) {
    am = M;
    return pc + read((u16)(pc+1));
}

u16 zpg(void) {
    am = M;
    return read((u16)(pc+1));
}

u16 zpgx(void) {
    am = M;
    return (u16) (read((u16)(pc+1))+x);
}

u16 zpgy(void) {
    am = M;
    return (u16) (read((u16)(pc+1))+y);
}

void setnz(u8 in) {
    sr.n = (in & 0x80) ? 1 : 0;
    sr.z = (!in) ? 1 : 0;
}

void convert(u16 *in) {
    if (am == M) {
        u16 val = read(*in) + (read((u16)(*in + 1)) << 8);
        *in = val;
    }
}

void adc(u16 in) {
    convert(&in);
    sr.c = (a + sr.c + in > 255) ? 1 : 0;
    sr.v = ((a & 0x80 == in & 0x80) && (a & 0x80 != (a + sr.c + in) & 0x80)) ? 1 : 0;
    a += sr.c + in;
    setnz(a);
}

void and(u16 in) {
    convert(&in);
    a &= in;
    setnz(a);
}

void asl(u16 in) {
    convert(&in);
    if (am == A) {
        sr.c = (unsigned int) ((a & 0x80) >> 8);
        a = a << 1;
        setnz(a);
    } else {
        sr.c = (unsigned int) ((a & 0x80) >> 8);
    }
}

void nop(u16 in) {

}

void pha(u16 in) {
    write((u16)(0x0100+sp), a);
    sp--;
}

void pla(u16 in) {
    a = read((u16)(0x0100+sp));
    setnz(a);
    sp++;
}

void tax(u16 in) {
    setnz(a);
    x = a;
}

void tay(u16 in) {
    setnz(a);
    y = a;
}

void tsx(u16 in) {
    setnz(sp);
    x = sp;
}

void txa(u16 in) {
    setnz(a);
    a = x;
}

void txs(u16 in) {
    setnz(x);
    sp = x;
}

void txs(u16 in) {
    setnz(x);
    sp = x;
}

void tya(u16 in) {
    setnz(a);
    a = y;
}

void update() {

}