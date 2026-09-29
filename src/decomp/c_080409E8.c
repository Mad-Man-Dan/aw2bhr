#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080409E8.
 * sub_080409E8 @ 0x080409E8
 */

#include "proc.h"
/* The pair starter behind the SiloFire_StartStrike group: one proc on tree 3 and a
 * second parented on the first, both carrying the same clamped screen position
 * and the same three bytes. The map byte is split differently between them --
 * low five bits plus a5 in the top three on the parent, the whole byte and its
 * top three bits on the child.
 *
 * The map planes must be reached as MEMBERS of a struct cast onto
 * gUnknown_08499590, inline rather than through a `map` local (W34-F's rule,
 * and src/decomp/c_08003DC4.c's note). A flat `gUnknown_08499590[0x1432 + row +
 * x]` reassociates to `(p + idx) + K` where the ROM has `(p + K) + idx`, which
 * is +4 bytes and a different high-register assignment. */
#define MAP gMap
struct Unk409E8Proc
{
    /* 00 */ u8 filler_00[0x3c];
    /* 3c */ u16 unk3c;
    /* 3e */ u16 unk3e;
    /* 40 */ u8 filler_40[0x04];
    /* 44 */ u8 unk44;
    /* 45 */ u8 unk45;
    /* 46 */ u8 filler_46[0x02];
    /* 48 */ u8 unk48;
    /* 49 */ u8 unk49;
    /* 4a */ u8 unk4a;
};

void StartCaptureAnimation(int a1, int a2, int a3, int a4, int a5)
{
    struct Unk409E8Proc *p;
    struct Unk409E8Proc *q;
    int x;
    int y;

    x = a1 * 16 - gMap->scrollX + 8;
    y = a2 * 16 - gMap->scrollY + 0x10;
    if (x <= 0x1f)
        x = 0x20;
    if (x > 0xd0)
        x = 0xd0;
    if (y <= 0x4f)
        y = 0x50;
    if (y > 0xa0)
        y = 0xa0;
    p = Proc_Start(gUnknown_0849FD84, PROC_TREE_3);
    p->unk3c = x;
    p->unk3e = y;
    p->unk48 = a3;
    p->unk49 = a4;
    p->unk44 = a5;
    p->unk4a = (MAP->terrain[MAP->rowOffset[a2] + a1] & 0x1f) | (a5 << 5);
    q = Proc_Start(gUnknown_0849FD44, p);
    q->unk3c = x;
    q->unk3e = y;
    q->unk48 = a3;
    q->unk49 = a4;
    q->unk44 = a5;
    q->unk45 = MAP->terrain[MAP->rowOffset[a2] + a1] >> 5;
    q->unk4a = MAP->terrain[MAP->rowOffset[a2] + a1];
}
asm(".global sub_080409E8\n.thumb_set sub_080409E8, StartCaptureAnimation\n");
