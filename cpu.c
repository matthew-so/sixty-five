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

void adc(u16 in) {
    in = read(in);
    sr.c = (a + sr.c + in > 255) ? 1 : 0;
    sr.v = ((a & 0x80 == in & 0x80) && (a & 0x80 != (a + sr.c + in) & 0x80)) ? 1 : 0;
    a += sr.c + in;
    setnz(a);
}

void adci(u16 in) {
    sr.c = (a + sr.c + in > 255) ? 1 : 0;
    sr.v = ((a & 0x80 == in & 0x80) && (a & 0x80 != (a + sr.c + in) & 0x80)) ? 1 : 0;
    a += sr.c + in;
    setnz(a);
}

void and(u16 in) {
    a &= read(in);
    setnz(a);
}

void andi(u16 in) {
    a &= in;
    setnz(a);
}

void asl(u16 in) {
    sr.c = (unsigned int) ((read(in) & 0x80) >> 7);
    write(in, read(in) << 1);
    setnz(read(in) << 1);
}

void asla(u16 in) {
    sr.c = (unsigned int) ((a & 0x80) >> 7);
    a = a << 1;
    setnz(a);
}

void bcc(u16 in) {
    pc += (sr.c == 0)? pc + in - 128 : 0;
}

void bcs(u16 in) {
    pc += (sr.c == 1)? pc + in - 128 : 0;
}

void beq(u16 in) {
    pc += (sr.z == 1)? pc + in - 128 : 0;
}

void bit(u16 in) {
    sr.z = (u8) (a & read(in) & 0x01);
    sr.n = (u8) (read(in) & 0x80) >> 7;
    sr.v = (u8) (read(in) & 0x40) >> 7;
}

void bmi(u16 in) {
    pc += (sr.n == 1)? pc + in - 128 : 0;
}

void bne(u16 in) {
    pc += (sr.z == 0)? pc + in - 128 : 0;
}

void bpl(u16 in) {
    pc += (sr.n == 0)? pc + in - 128 : 0;
}

void bvc(u16 in) {
    pc += (sr.v == 0)? pc + in - 128 : 0;
}

void bvs(u16 in) {
    pc += (sr.v == 0)? pc + in - 128 : 0;
}

void clc(u16 in) {
    sr.c = 0;
}

void cld(u16 in) {
    sr.d = 0;
}

void cli(u16 in) {
    sr.i = 0;
}

void clv(u16 in) {
    sr.v = 0;
}

void cmp(u16 in) {
    sr.c = a > read(in) ? 1 : 0;
    sr.z = a == read(in) ? 1 : 0;
    sr.n = a < read(in) ? 1 : 0;
}

void cmpi(u16 in) {
    sr.c = a > in ? 1 : 0;
    sr.z = a == in ? 1 : 0;
    sr.n = a < in ? 1 : 0;
}

void cpx(u16 in) {
    sr.c = x > read(in) ? 1 : 0;
    sr.z = x == read(in) ? 1 : 0;
    sr.n = x < read(in) ? 1 : 0;
}

void cpxi(u16 in) {
    sr.c = x > in ? 1 : 0;
    sr.z = x == in ? 1 : 0;
    sr.n = x < in ? 1 : 0;
}

void cpy(u16 in) {
    sr.c = y > read(in) ? 1 : 0;
    sr.z = y == read(in) ? 1 : 0;
    sr.n = y < read(in) ? 1 : 0;
}

void cpyi(u16 in) {
    sr.c = y > in ? 1 : 0;
    sr.z = y == in ? 1 : 0;
    sr.n = y < in ? 1 : 0;
}

void dec(u16 in) {
    write(in, (u8)(read(in) - 1));
    setnz(read(in));
}

void dex(u16 in) {
    x--;
    setnz(x);
}

void dey(u16 in) {
    y--;
    setnz(y);
}

void eor(u16 in) {
    a ^= read(in);
    setnz(a);
}

void eori(u16 in) {
    a ^= in;
    setnz(a);
}

void inc(u16 in) {
    write(in, (u8)(read(in) + 1));
    setnz(read(in));
}

void inx(u16 in) {
    x++;
    setnz(x);
}

void iny(u16 in) {
    y++;
    setnz(y);
}

void jmp(u16 in) {
    pc = read((u16)(pc + 1)) + (read((u16)(pc + 2)) << 8);
}

void jsr(u16 in) {
    write((u16)(0x0100 + sp), (u8)((pc + 2) >> 8));
    write((u16)(0x0100 + sp - 1), (u8)((pc + 2)));
    sp -= 2;
    pc = read((u16)(pc + 1)) + (read((u16)(pc + 2)) << 8);
}

void lda(u16 in) {
    a = load(in);
    setnz(a);
}

void ldai(u16 in) {
    a = (u8) in;
    setnz(a);
}

void ldx(u16 in) {
    x = load(in);
    setnz(x);
}

void ldxi(u16 in) {
    x = (u8) in;
    setnz(x);
}

void ldy(u16 in) {
    y = load(in);
    setnz(y);
}

void ldyi(u16 in) {
    y = (u8) in;
    setnz(y);
}

void lsr(u16 in) {
    sr.c = (u8) (read(in) & 0x01);
    write(in, read(in) >> 1);
    setnz(a);
}

void lsra(u16 in) {
    sr.c = (u8) (in & 0x01);
    a = a >> 1;
    setnz(a);
}

void nop(u16 in) {

}

void ora(u16 in) {
    a |= read(in);
    setnz(a);
}

void orai(u16 in) {
    a |= in;
    setnz(a);
}

void pha(u16 in) {
    write((u16)(0x0100+sp), a);
    sp--;
}

void php(u16 in) {
    write((u16)(0x0100+sp), (u8)((sr.n << 7) + (sr.v << 6) + (sr.b << 4) + (sr.d << 3)
    + (sr.i << 2) + (sr.z << 1) + sr.c));
    sp--;
}

void pla(u16 in) {
    a = read((u16)(0x0100+sp + 1));
    setnz(a);
    sp++;
}

void plp(u16 in) {
    u8 val = read((u16)(0x0100 + sp + 1));
    sp++;
    sr.n = (u8)((val & 0x80) >> 7);
    sr.v = (u8)((val & 0x40) >> 6);
    sr.b = (u8)((val & 0x10) >> 4);
    sr.d = (u8)((val & 0x08) >> 3);
    sr.i = (u8)((val & 0x04) >> 2);
    sr.z = (u8)((val & 0x02) >> 1);
    sr.c = (u8)((val & 0x01));
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