#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D7B4.
 * sub_0802D7B4 @ 0x0802D7B4
 */

/* MATCHED, wave 56 (W56-E).  Draws the seven-slot menu: one row per non-empty
 * gUnknown_02023830 record, each row a name string, a cost and a window fill.
 *
 * promote.json needs "rodata": ["0x08090C10", "0x08090C14"] -- those two ROM
 * words are agbcc's own -fforce-addr address constants for gUnknown_02023830
 * and gUnknown_030033EC.  BOTH symbols are ALSO reached in this same function
 * through ordinary inline pool words, so one function shows both spellings of
 * the same global; that is -fforce-addr choosing per reference, not two
 * different objects.  The honest spelling (name the real global, never the
 * 0x08090Cxx slot) is what produced this match.
 *
 * gTextTable is NOT a pool slot -- a scaled index is added before the
 * load, so it is a genuine array base and is referenced normally.
 *
 * Three readings that each cost an attempt:
 *
 *  - `v` is u16, not int.  It reaches WriteUnitTileQuadAt's u16 fifth parameter, and
 *    an int local spills SHIFTED (`lsls #16; str` / `ldr; lsrs #16`) where the
 *    ROM stores and reloads the value straight.  +4 bytes.
 *
 *  - The `+1` byte of the record must fold into the load DISPLACEMENT.
 *    `gUnknown_02023830[idx * 4 + 1]` does not: fold reassociates it to
 *    `(idx * 4 + 1) + base` and emits `adds r0, #1` with `ldrb [r0, #0]`.
 *    Binding the element address to a pointer for that one statement gives
 *    the ROM's `ldrb [r0, #1]`.
 *
 *  - Do NOT reach the record through a locally-declared struct cast on the
 *    `u8 *` base (the gUnknown_08499590 idiom).  It is correct addressing but
 *    it CSEs the base into ONE loop-invariant pseudo, which then takes sl and
 *    evicts `flag` to a fresh stack slot: +4 frame, +4 bytes, one extra pool
 *    word.  The ROM rematerialises `ldr rN, =gUnknown_02023830` at each of the
 *    four uses, which is what plain array subscripting emits.
 *
 * `i < 7` is the unsigned `cmp #6; bhi` at the loop bottom, and it is written
 * FIRST in the `&&` because the ROM tests it before re-reading the record. */
void DrawDeploymentList(int a1)
{
    u8 buf[16];
    u16 base;
    u16 i;
    u8 flag;
    u16 v;
    u8 *e;
    u8 *s;
    int j;
    s16 row;

    base = a1;

    for (i = 0; i < 7 && gUnknown_02023830[((s16)base + i) * 4] != 0; i++) {
        e = &gUnknown_02023830[((s16)base + i) * 4];

        if (e[1] == 0) {
            flag = 0;
            v = gUnknown_03003F2C;
        } else {
            flag = 1;
            v = 0x100;
        }

        s = gTextTable[*(u16 *)&gUnknown_085D5ABC[
                gUnknown_02023830[((s16)base + i) * 4]]];

        buf[0] = 0x19;
        j = 0;
        while (s[j] != 0) {
            buf[j + 1] = s[j];
            j++;
        }
        buf[j + 1] = 0;

        row = i * 2 + 5;

        PutTextScriptImmediate(4, row, gBG0TilemapBuffer, buf, 0x8000, flag);
        DrawTallNumberRightAligned(0xe, row, gBG0TilemapBuffer,
                     GetUnitCostWithCoBonus(gUnknown_030033EC,
                                  gUnknown_02023830[((s16)base + i) * 4]) * 10,
                     0x8000, flag);
        WriteUnitTileQuadAt(gBG0TilemapBuffer, 2, row,
                     gUnknown_02023830[((s16)base + i) * 4], v, 0, 0);
    }

    BG_EnableSyncBG0();
}
asm(".global sub_0802D7B4\n.thumb_set sub_0802D7B4, DrawDeploymentList\n");
