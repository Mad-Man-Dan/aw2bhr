#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011D10.
 * sub_08011D10 @ 0x08011D10, sub_08011D7C @ 0x08011D7C, sub_08011DE8 @ 0x08011DE8, sub_08011E54 @ 0x08011E54, sub_08011EF0 @ 0x08011EF0, sub_08011F70 @ 0x08011F70
 */

/* One of six pushes onto the gUnknown_0200B3B4 request queue, all the same
 * shape and differing only in which members they fill and the tag at +0x0a.
 *
 * The dead `ldrb r1,[r0,#0xa]` in front of the tag store is NOT a source read:
 * unk0a and unk08 are already declared `volatile` in struct Unk0200B3B4, and a
 * volatile narrow member's assignment carries the load agbcc emits for the
 * read-modify-write and then never uses. Reproduced exactly by a plain
 * assignment -- do not try to explain it with a bitfield.
 *
 * The index is re-read from the volatile gUnknown_03002F30 and re-scaled for
 * every member, which is what the volatile buys; a bound local index collapses
 * all four to one address. */
s16 RegisterDecompress(void *a, void *b)
{
    if (gUnknown_03002F30 == 0x30)
        return -1;

    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00 = (u32)a;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk04 = (u32)b;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk0a = 3;
    gUnknown_03002F30++;

    return gUnknown_03002F30 - 1;
}
asm(".global sub_08011D10\n.thumb_set sub_08011D10, RegisterDecompress\n");

/* gUnknown_0200B3B4 queue push, tag 4. See RegisterDecompress for the shape. */
s16 RegisterFillZero16(void *a, int b)
{
    if (gUnknown_03002F30 == 0x30)
        return -1;

    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00 = (u32)a;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk08 = b;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk0a = 4;
    gUnknown_03002F30++;

    return gUnknown_03002F30 - 1;
}
asm(".global sub_08011D7C\n.thumb_set sub_08011D7C, RegisterFillZero16\n");

/* gUnknown_0200B3B4 queue push, tag 5 -- byte-for-byte RegisterFillZero16 with a
 * different tag. See RegisterDecompress for the shape. */
s16 RegisterFillZero32(void *a, int b)
{
    if (gUnknown_03002F30 == 0x30)
        return -1;

    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00 = (u32)a;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk08 = b;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk0a = 5;
    gUnknown_03002F30++;

    return gUnknown_03002F30 - 1;
}
asm(".global sub_08011DE8\n.thumb_set sub_08011DE8, RegisterFillZero32\n");

/* gUnknown_0200B3B4 queue push, tag 0, plus the immediate-mode bypass: when
 * gUnknown_030044D0 is set the request is performed on the spot through
 * CpuCopyAuto and 0 is returned instead of a slot index. The three arguments
 * need no setup at that call -- they are still in r0/r1/r2 -- which is why the
 * bypass costs one `adds r0, r3, #0`. See RegisterDecompress for the queue shape.
 *
 * Returns s16, not the `int` its declaration carried: the epilogue narrows the
 * index with `lsls #0x10; asrs #0x10`, exactly as the five siblings do. */
s16 RegisterDataMove(void *a, void *b, u16 c)
{
    if (gUnknown_030044D0 != 0)
    {
        CpuCopyAuto(a, b, c);
        return 0;
    }

    if (gUnknown_03002F30 == 0x30)
        return -1;

    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00 = (u32)a;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk04 = (u32)b;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk08 = c;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk0a = 0;
    gUnknown_03002F30++;

    return gUnknown_03002F30 - 1;
}
asm(".global sub_08011E54\n.thumb_set sub_08011E54, RegisterDataMove\n");

/* gUnknown_0200B3B4 queue push, tag 1. See RegisterDecompress for the shape. */
s16 sub_08011EF0(void *a, void *b, u16 c)
{
    if (gUnknown_03002F30 == 0x30)
        return -1;

    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00 = (u32)a;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk04 = (u32)b;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk08 = c;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk0a = 1;
    gUnknown_03002F30++;

    return gUnknown_03002F30 - 1;
}

/* gUnknown_0200B3B4 queue push, tag 2 -- byte-for-byte sub_08011EF0 with a
 * different tag. See RegisterDecompress for the shape. */
s16 sub_08011F70(void *a, void *b, u16 c)
{
    if (gUnknown_03002F30 == 0x30)
        return -1;

    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00 = (u32)a;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk04 = (u32)b;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk08 = c;
    gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk0a = 2;
    gUnknown_03002F30++;

    return gUnknown_03002F30 - 1;
}
