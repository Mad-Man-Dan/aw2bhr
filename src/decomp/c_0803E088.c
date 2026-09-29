#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E088.
 * sub_0803E088 @ 0x0803E088
 */

/* The object AddInventionRecord hands back. Only +3, +5, +6 and +7 are named; +3 is
 * BITFIELDS and not a scalar mask -- the clear of bits 2..5 is
 * `movs r2, #0x3d; rsbs r2, r2, #0`, the mov/neg form a scalar `&= ~0x3c`
 * would have narrowed to a bare `movs #0xc3`. */
struct Unk3E01C
{
    /* 0x00 */ u8 filler_00[0x03];
    /* 0x03 */ u8 unk03_0 : 2;
               u8 unk03_2 : 4;
               u8 unk03_6 : 2;
    /* 0x04 */ u8 filler_04[0x01];
    /* 0x05 */ u8 unk05;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
};

void *AddInventionRecordExt(int a1, int a2, int a3, int a4, int a5, int a6,
                   int a7, int a8, int a9, int a10)
{
    struct Unk3E01C *p = AddInventionRecord(a1, a2, a3, a4, a5, a6);

    p->unk05 = a7;
    p->unk03_6 = a9;
    p->unk03_2 = a8;
    p->unk06 = a7;
    p->unk07 = a10;

    return p;
}
asm(".global sub_0803E088\n.thumb_set sub_0803E088, AddInventionRecordExt\n");
