#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075904.
 * sub_08075904 @ 0x08075904, sub_080759A0 @ 0x080759A0, sub_08075A54 @ 0x08075A54
 */

/* MATCHED (wave 38, W38-E), first attempt. Unassigned extra. Twin of
 * ColorWorldMapSectionWithBank, which is the same walk writing a CALLER-supplied id instead of
 * the blob's own and echoing each id through an out-pointer.
 *
 * Stamps a rectangular tile-id blob into the top nibble of gUnknown_08614280's
 * tilemap: for every non-zero id in a width*height blob it rewrites
 * `map[(y + j) * 32 + x + i]` as `(old & 0xFFF) | (id << 12)`. Returns 0 when
 * the record has no blob, 1 otherwise. See gUnknown_08615194's unk0c in
 * include/unknown-globals.h for the record type.
 *
 * TWO SPELLINGS ARE LOAD-BEARING, and both come from the matched sibling
 * src/decomp/c_08075F44.c, which indexes the same tilemap the same way:
 *
 *  - `(u16 *)gUnknown_08614280 + 0x3E0` for the second screen block. The symbol
 *    is a POINTER, not the buffer, so the arms are `ldr rN,=sym; ldr rM,[rN]`
 *    and the bias is 0x3E0 HALFWORDS = the ROM's `0xf8 << 3` = 0x7C0 bytes.
 *  - The row stride is `* 32`, NOT `<< 5`. This is the opposite of what
 *    c_08075F44.c needed and the two together pin the rule: fold reorders a
 *    commutative PLUS so a MULT_EXPR operand comes first, so `y * 32 + x` emits
 *    `adds rD, rMUL, rX` (the ROM here) while `(y << 5) + x` emits
 *    `adds rD, rX, rSHIFT` (the ROM there). Read the ADD's operand order, not
 *    the `lsls`, which is identical either way.
 *
 * `p->unk00` is loaded once and used twice (the `> 0x1F` test and the index)
 * because nothing writes memory between them; its twin re-loads it, and the
 * only difference is that the twin stores through its out-pointer in between.
 * That is ordinary CSE invalidation and neither needs a local.
 */
struct Unk8075904Map
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ const u8 *unk04;
};
#include "hardware.h"

int ColorWorldMapSection(int a1)
{
    const struct Unk8075904Map *p;
    const u8 *src;
    u16 *base;
    int i;
    int j;

    p = gUnknown_08615194[a1].mapSectionToColor;

    if (p == 0)
        return 0;

    src = p->unk04;

    for (j = 0; j < p->unk03; j++)
    {
        for (i = 0; i < p->unk02; i++)
        {
            if (*src != 0)
            {
                if (p->unk00 + i > 0x1F)
                    base = (u16 *)gUnknown_08614280 + 0x3E0;
                else
                    base = (u16 *)gUnknown_08614280;

                base[(p->unk01 + j) * 32 + p->unk00 + i] =
                    (base[(p->unk01 + j) * 32 + p->unk00 + i] & 0xFFF)
                        | (*src << 12);
            }

            src++;
        }
    }

    return 1;
}
asm(".global sub_08075904\n.thumb_set sub_08075904, ColorWorldMapSection\n");

u8 ColorWorldMapSectionWithBank(int a1, int a2, u8 *a3)
{
    const struct Unk8075904Map *p;
    const u8 *src;
    u16 *base;
    u8 v;
    int i;
    int j;

    v = a2;

    p = gUnknown_08615194[a1].mapSectionToColor;

    if (p == 0)
        return 0;

    src = p->unk04;

    for (j = 0; j < p->unk03; j++)
    {
        for (i = 0; i < p->unk02; i++)
        {
            if (*src != 0)
            {
                *a3 = *src;

                if (p->unk00 + i > 0x1F)
                    base = (u16 *)gUnknown_08614280 + 0x3E0;
                else
                    base = (u16 *)gUnknown_08614280;

                base[(p->unk01 + j) * 32 + p->unk00 + i] =
                    (base[(p->unk01 + j) * 32 + p->unk00 + i] & 0xFFF)
                        | (v << 12);
            }

            src++;
        }
    }

    return 1;
}
asm(".global sub_080759A0\n.thumb_set sub_080759A0, ColorWorldMapSectionWithBank\n");

/* MATCHED (wave 38, W38-E), first attempt. Unassigned extra.
 *
 * Unpacks ten BGR555 entries of gPal into gUnknown_0200B614's three-bytes-per-
 * colour shadow, and records the mode byte in gUnknown_0200B5F4[c]. See the
 * evidence for gUnknown_0200B614 in include/unknown-globals.h.
 *
 * `s8 i` IS THE LOOP COUNTER, and this is the cleanest example of the s8-local
 * shape in the block. agbcc's PROMOTE_MODE holds every sub-word local
 * ZERO-extended, so the increment ends
 *     adds r3, #1; lsls r3, #0x18; lsrs r1, r3, #0x18; asrs r3, r3, #0x18
 * -- one shift pair producing the STORED form (u8, r1) and a second producing
 * the READ form (s8, r3), off the same `lsls`. The exit test then compares the
 * sign-extended temp, which is the wave-38 brief's "a non-coalesced pair means
 * the EXIT TEST reads the temp" rule seen from the source side. `int i` loses
 * all four shifts; `u8 i` loses the `asrs`.
 *
 * The first channel read is `ldrb` and the other two are `ldrh` in the ROM.
 * That is not three different accesses in the source -- it is agbcc narrowing
 * the load for `gPal[n] & 0x1f`, where only the low byte survives the mask,
 * while `>> 5` and `>> 10` need the halfword. Writing all three as the same
 * `gPal[...]` expression is what produces it.
 *
 * The three-byte stride is spelled `* 3` and shares its `idx * 2` subexpression
 * with the `&gPal[idx]` halfword address in the same statement (the ROM's
 * `lsls r4, r2, #1; adds r2, r4, r2`), so the two indices must be written from
 * the SAME `c * 16 + i` expression rather than bound separately.
 */
void LoadFadeComponentsFromPalette(int a1, int a2)
{
    u8 c;
    s8 i;

    c = a1;
    gUnknown_0200B5F4[c] = a2;

    for (i = 0; i <= 9; i++)
    {
        gUnknown_0200B614[(c * 16 + i) * 3] = gPal[c * 16 + i] & 0x1F;
        gUnknown_0200B614[(c * 16 + i) * 3 + 1] = (gPal[c * 16 + i] >> 5) & 0x1F;
        gUnknown_0200B614[(c * 16 + i) * 3 + 2] = (gPal[c * 16 + i] >> 10) & 0x1F;
    }
}
asm(".global sub_08075A54\n.thumb_set sub_08075A54, LoadFadeComponentsFromPalette\n");
