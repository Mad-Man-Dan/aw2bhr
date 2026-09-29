#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030930.
 * sub_08030930 @ 0x08030930
 */

/* MATCHED (wave 42, W42-K). Needs its .rodata pool word placed:
 *   "rodata": ["0x08090CDC"]
 * That word is an agbcc -fforce-addr address constant holding
 * &gUnknown_0849B018, NOT a global -- see the W41-E note in
 * unknown-globals.h. The honest `gUnknown_0849B018->member` spelling
 * reproduces the three-deep `ldr; ldr; ldr` chain by itself.
 *
 * LinkQueueCommand's twin: the same push into the ring, but a 128-byte payload
 * instead of 20, which is why unk04 (the LENGTH field) is 0x80 here. The
 * `u8 i` counter with `i < 128` is what produces the `cmp r0,#0; bge` exit
 * on the SHIFTED value -- gcc turns `(u8)(i + 1) < 128` into a sign test on
 * (i + 1) << 24, and that is an ASCENDING loop despite the `bge`.
 *
 * `p` binds gUnknown_0849B018 for the two unk20 accesses so the pointer is
 * loaded once (`ldr r1,[r3]`) and reused across the unk04 store. Every other
 * `gUnknown_0849B018->` reloads it, and that is not a spelling choice: the
 * intervening `strb` through `entry` may alias the pointer variable, so gcc
 * has to assume it does.
 *
 * The parameter is a byte buffer and `u32` is very probably the wrong
 * spelling, but it is what unknown-functions.h declares and what the matched
 * caller c_0803355C.c passes; the question is byte-neutral, so the promoted
 * caller wins. See the note on the declaration. */

void LinkQueueBlock(u32 a1)
{
    struct Unk08090CD8Entry *entry;
    struct Unk0849B018 *p;
    u8 i;

    entry = &gUnknown_0849B018->unk12c[gUnknown_0849B018->unk1aad];
    entry->unk00 = 0xAF;
    entry->unk01 = gUnknown_0849B018->unk06;
    p = gUnknown_0849B018;
    entry->unk02 = p->unk20;
    entry->unk04 = 0x80;
    p->unk20++;

    for (i = 0; i < 128; i++)
        entry->unk06[i] = ((u8 *)a1)[i];

    gUnknown_0849B018->unk1aad++;
    gUnknown_0849B018->unk1aad &= 0x1F;
}
asm(".global sub_08030930\n.thumb_set sub_08030930, LinkQueueBlock\n");
