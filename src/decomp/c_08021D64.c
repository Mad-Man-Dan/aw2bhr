#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08021D64.
 * LoadSeaAnimFrame @ 0x08021D64
 */

#include "hardware.h"

/* UNPARKED, wave 32 (W32-A). Wave 31 left this at 91.7%: the ROM groups the
 * VRAM base with the `(n & 1) << 12` term (`adds r2, r2, r0; adds r1, r1, r2`)
 * and every candidate grouped it with the chr_block term instead.
 *
 * THE LEVER IS WHICH TERM CARRIES THE POINTER CAST, not parenthesisation and
 * not a temporary -- wave 31 had ruled out all three orderings, explicit
 * parentheses, an `int off` local and a `u8 *dst` local, and it was right that
 * none of them works. agbcc's `associate_trees` rewrites `(X + C) + Y` as
 * `X + (Y + C)`: the integer constant migrates to the term that is NOT the
 * pointer. So casting the CHR_BLOCK term to `u8 *` and writing the constant
 * next to it drives the constant onto the `(n & 1)` term, which is where the
 * ROM has it -- and it fixes the evaluation order at the same time, because the
 * pointer term is evaluated first.
 *
 * Wave 31 tried this shape with the casts the other way round
 * (`(u8 *)(0x06002000 + ((n & 1) << 12)) + chr`), which is the same rule
 * pushing the constant the wrong way. */
void LoadSeaAnimFrame(int a1)
{
    u8 n = a1;

    CpuCopyAuto(gUnknown_080C1FC4 + (n << 12),
                 (u8 *)(gUnknown_0300251C.bits.chr_block << 14) + 0x06002000
                     + ((n & 1) << 12),
                 0x1000);
}

asm(".global sub_08021D64\n.thumb_set sub_08021D64, LoadSeaAnimFrame\n");
