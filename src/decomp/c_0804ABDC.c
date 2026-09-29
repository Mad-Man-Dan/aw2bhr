#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804ABDC.
 * sub_0804ABDC @ 0x0804ABDC
 */

/* The HUD renderer for the gUnknown_030044E0 map cursor. All four
 * gUnknown_0812A2xx symbols are `-fforce-addr` .rodata address-constant words,
 * dumped from baserom.gba: 0x0812A290 -> &gUnknown_030044E0 (the same word
 * src/decomp/c_0804AAF8.c records at 0x0812A28C), 0x0812A294 ->
 * gUnknown_0813204C, 0x0812A298 -> &gUnknown_030030A0 and 0x0812A29C ->
 * gUnknown_0848B6C6. Every one is named directly here.
 *
 * unk1e is read BOTH ways off the same offset: as a byte for GetNameEntryGridChar's u8
 * argument and as a SIGNED HALFWORD (`movs rI,#0x1e; ldrsh`, the only thumb
 * ldrsh form) to index gUnknown_084C3D5C beside the s16 unk20 indexing
 * gUnknown_084C3D14. The declared u8 is kept and the halfword view is a cast.
 *
 * Three spellings here are load-bearing:
 *   - `v` is int, not u8: the range tests are `blt`/`ble`, i.e. SIGNED.
 *   - the guard is the NEGATION of `v >= 0x23 && (v <= 0x25 || v == 0x40)`.
 *     Written positively as `v < 0x23 || ...` agbcc canonicalises the first
 *     test to `cmp #0x22; ble` where the ROM has `cmp #0x23; blt`.
 *   - the two frame offsets are BYTE offsets, computed once and added to a
 *     casted base. An `int` index used as `arr[f]` in two arms scales it twice.
 *   - the embedded `(sx = ...)` blocks fold from turning `t - (g - 0x30)` into
 *     `(t + 0x30) - g`. */
void NameEntry_DrawSprites(void)
{
    int v;
    int i;
    int f;
    int sx;
    u16 y;

    if (gUnknown_030044E0->unk62 != 0)
    {
        v = GetNameEntryGridChar(gUnknown_030044E0->unk20 * 15 + gUnknown_030044E0->unk1e);

        if (gUnknown_030044E0->unk22 > 0x78)
            gUnknown_030044E0->unk22 = 0;

        f = ((gUnknown_030044E0->unk22 / 4) & 0xf) * 2;
        sub_08013664((u16 *)((u8 *)gUnknown_0813204C + f), 0x2fe, 2);

        if (v == 0x40 || v == 0x24 || v == 0x25 || v == 0x23)
        {
            i = 0;
            if (v != 0x40)
            {
                i = 1;
                if (v != 0x24)
                {
                    i = 2;
                    if (v != 0x25)
                        i = 3;
                }
            }
            sub_08013664((u16 *)((u8 *)gUnknown_08131D8C + f),
                         ((i + 0x10) << 5) + 0xc, 2);
        }

        if (gUnknown_030044E0->unk24 != 0)
            gUnknown_030044E0->unk24--;
        else
            gUnknown_030044E0->unk22++;

        if (gUnknown_030044E0->unk26 > 0x78)
            gUnknown_030044E0->unk26 = 0;

        f = ((gUnknown_030044E0->unk26 / 4) & 0xf) * 2;

        if (gUnknown_030044E0->unk65 > gUnknown_030044E0->unk60 - 2
            || gUnknown_030044E0->unk5d > gUnknown_030044E0->unk5f - 1)
            sub_08013664((u16 *)((u8 *)gUnknown_08131DEC + f), 0x29e, 2);
        else
            sub_08013664((u16 *)((u8 *)gUnknown_0813204C + f), 0x29e, 2);

        if (gUnknown_030044E0->unk28 != 0)
            gUnknown_030044E0->unk28--;
        else
            gUnknown_030044E0->unk26++;

        switch (v)
        {
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x40:
            break;

        default:
            PutOamHi((gUnknown_084C3D5C[*(s16 *)&gUnknown_030044E0->unk1e]
                          - (sx = gUnknown_030030A0 - 0x30)) & 0x1ff,
                         gUnknown_084C3D14[gUnknown_030044E0->unk20] + 0x30,
                         gUnknown_084C3B72, 0x701d);
            break;
        }

        y = gUnknown_030044E0->unk65;
        if (y > gUnknown_030044E0->unk60 - 8)
            y = gUnknown_030044E0->unk60 - 8;
        PutOamHi((gUnknown_030044E0->unk61 * 8 + y - gUnknown_030030A0) & 0x1ff,
                     0x20, gUnknown_0848B688, 0x401e);
    }

    if (gUnknown_030044E0->unk5c != 0)
        PutOamHi(0x7c, 0x80, gUnknown_0848B6C6, 0x1000);

    PutOamHi(0x9e, 0x80, gUnknown_0848B6C6, 0x2008);
    PutOamHi(0xc0, 0x80, gUnknown_0848B6C6, 0x3010);
}
asm(".global sub_0804ABDC\n.thumb_set sub_0804ABDC, NameEntry_DrawSprites\n");

/* MATCHED in wave 66. The last two levers were an explicit nested if-chain for
 * the 0/1/2/3 selector (removing the four-byte duplicated decision tail) and a
 * switch with cases 0x23/0x24/0x25/0x40, whose default arm produces the ROM's
 * `cmp #0x23; blt` decision tree. Historical wave-37 findings follow.
 * Three separate findings landed on the
 * way and are all in the draft above -- each was worth real bytes:
 *   - the two frame offsets must be BYTE offsets computed once
 *     (`((unkN / 4) & 0xf) * 2`) and added to a `(u8 *)`-cast base. An `int`
 *     index used as `arr[f]` in two arms scales it twice, once per arm, where
 *     the ROM keeps `lsls r5,r0,#1` before the branch.
 *   - `v` is `int`, not the `u8` GetNameEntryGridChar returns: the range tests are
 *     `blt`/`ble`, i.e. SIGNED, and a u8 object gives `bls`.
 *   - the embedded `(sx = gUnknown_030030A0 - 0x30)` is what stops fold
 *     rewriting `t - (g - 0x30)` into `(t + 0x30) - g`. Same lever as the
 *     `(n = ...)` in src/decomp/c_080267AC.c.
 * What is LEFT is +4 bytes somewhere in the first half (the diff is already
 * shifted by 4 at offset 0xce, before the unk26 wrap test), plus one
 * same-size encoding difference at the PutOamHi guard: the ROM has
 * `cmp r6,#0x23; blt` and this candidate `cmp r6,#0x22; ble`. Writing the
 * guard positively (`v < 0x23 || (v > 0x25 && v != 0x40)`) and as the negation
 * of `v >= 0x23 && (v <= 0x25 || v == 0x40)` produce the SAME `cmp #0x22; ble`,
 * so the `blt` form is not reachable from the comparison's spelling and the
 * lever is somewhere else.
 * NOT tried: the `i` selection as an if-chain rather than a nested `?:`; the
 * frame-index locals split into two variables instead of reusing `f`. */
