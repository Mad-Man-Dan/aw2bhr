#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049F24.
 * sub_08049F24 @ 0x08049F24
 */

/* MATCHED byte-for-byte (wave 48, W48-I).
 *
 * PROMOTION NEEDS TWO .rodata POOL WORDS PLACED:
 *   "rodata": ["0x0812A170", "0x0812A174"]   (then split_rodata.py + gen_lds.py)
 * They are agbcc -fforce-addr address constants holding 0x02028E40 and
 * 0x084C37E4, i.e. &gUnknown_02028E40 and &gUnknown_084C37E4 -- two more of the
 * 0x0812Axxx run unknown-globals.h documents, NOT objects. The honest spelling
 * produces both; the pair appears because both globals are referenced inside
 * the loop, which is the documented loop-driven-pair case.
 *
 * THREE SPELLINGS ARE LOAD-BEARING; the function does not match without them.
 *
 * 1. `off = t * 2;` as its own statement, with the base added as a `u8 *`.
 *    Almost certainly not the original text, but it is the only spelling found
 *    that puts the base `ldr` AFTER both shifts, and here is why -- this is the
 *    transferable part. combine merges the u16 truncation's `lsrs #0x10` with
 *    the u16 subscript's `lsls #1` into a single `lsrs #0xf`, and the merged
 *    insn lands at the position of the LATER of the two. So the base `ldr`'s
 *    position relative to it is decided by which STATEMENT the `* 2` is in:
 *      `u16 t; t = ...;` then `&gUnknown_08131DEC[t]`     -> ldr BETWEEN the
 *                                                            two shifts (96.4%)
 *      `u32 t; t = ...;` then `&gUnknown_08131DEC[(u16)t]` -> ldr BEFORE both
 *                                                            shifts (95.0%)
 *      the whole modulo chain inline in the call           -> the base is
 *                                        hoisted into a callee-saved register
 *                                        ahead of both __umodsi3 calls
 *      the scaling in its OWN statement (below)            -> ldr AFTER both,
 *                                                            which is the ROM
 *    `gUnknown_08131DEC + t` and `t + gUnknown_08131DEC` are byte-identical to
 *    `&gUnknown_08131DEC[t]`; reversing pointer/integer operands does nothing.
 *
 * 2. `s16 j = i` for argument 2 only. Argument 2 and argument 4 each need their
 *    OWN `lsls rN, r4, #4`. Spelled with one type agbcc CSEs `i * 16`, keeps it
 *    in a scratch and copies it (`adds r1, r0, #0`) -- same instruction count,
 *    wrong registers, and it also pushes the 0/1 flag into the OR's destination
 *    register. Any spelling that makes the two multiplies different expressions
 *    fixes both at once. This one came out of decomp-permuter (300 s, ~19,000
 *    iterations, 88.6% -> 96.4%). `(i + 1) * 16` also breaks the CSE, but then
 *    agbcc shares the `i + 1` with the loop's own increment and the tail loses
 *    2 bytes -- c_08048850.c's `i * 2 + 2` finding seen from the other side.
 *
 * 3. `sel` as a separate local. `((i == gUnknown_02028E40) + 2) << 12` written
 *    inline folds both arms to constants across a `b` (+8 bytes).
 *    `(sel + 2) << 12 | i * 16` and `(sel + 2) * 0x1000` are byte-identical to
 *    the spelling below.
 *
 * Proved: gGameClock is read UNSIGNED (two __umodsi3 calls and a `lsrs`
 * for the /4) although it is declared s32; `lsls #0x10; lsrs #0xf` is the u16
 * truncation folded with the u16 array subscript's *2 and NOT a mask (the
 * wave-16 rule); the trailing sprite's attribute is
 * `gUnknown_02028E40 * 48 + 0x46` (emitted `(g * 2 + g) << 4`) OR 0x2000.
 */

void LanguageSelect_DrawSprites(void)
{
    u16 t;
    u8 i;
    int sel;
    int off;
    s16 j;

    t = (u32)gGameClock % 0x78 / 4 % 0xf;
    off = t * 2;
    sub_08013664((u16 *)((u8 *)gUnknown_08131DEC + off), 0x26a, 2);

    for (i = 0; i <= 3; i++)
    {
        j = i;
        sel = 0;
        if (i == gUnknown_02028E40)
            sel = 1;
        PutOamHi(0x60, j * 16 + 16, gUnknown_084C37E4,
            i * 16 | (sel + 2) << 12);
    }

    PutOamHi(0x58, 0x60, gUnknown_084C3800,
        (gUnknown_02028E40 * 48 + 0x46) | 0x2000);
}
asm(".global sub_08049F24\n.thumb_set sub_08049F24, LanguageSelect_DrawSprites\n");
