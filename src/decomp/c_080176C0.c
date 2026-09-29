#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080176C0.
 * sub_080176C0 @ 0x080176C0
 */

/* A saturating add of `a` to two counters, each capped at 9999. The compares
 * are `bhs`, so both the counters and the amount are unsigned.
 *
 * The ROM's `ldr r1, =0x0808E558; ldr r4, [r1]` is NOT a pointer global -- see
 * the 0x0808E558 note in unknown-globals.h. It is agbcc's own -fforce-addr
 * address-constant pool holding &gUnknown_0200C420, and naming the global
 * directly reproduces the function exactly, including the `ldr r1, [r1]` reload
 * before unk04: what stays live in r1 is the POOL WORD's address, so each use
 * re-loads the object address through it. */
void AddBattleMapPoints(u32 a)
{
    if (0x270f - gUnknown_0200C420.unk00 < a)
        gUnknown_0200C420.unk00 = 0x270f;
    else
        gUnknown_0200C420.unk00 += a;

    if (0x270f - gUnknown_0200C420.unk04 < a)
        gUnknown_0200C420.unk04 = 0x270f;
    else
        gUnknown_0200C420.unk04 += a;
}
asm(".global sub_080176C0\n.thumb_set sub_080176C0, AddBattleMapPoints\n");
