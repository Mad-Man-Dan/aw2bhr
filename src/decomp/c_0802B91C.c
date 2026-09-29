#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802B91C.
 * sub_0802B91C @ 0x0802B91C
 */

/* Copies up to four 0x20-byte HP/status digit tiles plus one 0x80-byte block
 * into OBJ VRAM for a unit's map sprite, then puts the sprite out through
 * PutOamHi with one of two 5-entry pointer tables depending on whether
 * anything was drawn.
 *
 * Three source-level levers, none of which a straight transcription gets
 * right, and each one alone was the whole difference:
 *
 * (1) a4 is s16, NOT u16 with (s16) casts at the uses. The ROM materialises
 *     `a4 << 16` once (`lsls r4, r3, #0x10`) and derives BOTH the u16 third
 *     argument of ShouldDrawTransportMarker (`lsrs r2, r4, #0x10`) and every `(s16)a4`
 *     (`asrs`, six sites) from it. A u16 parameter hands the already
 *     zero-extended pseudo to the callee with no shift pair at all. Same for
 *     a1/a2: `(s16)a2 | 0x400` lets combine sink the extension past the OR
 *     (`orrs` then `lsls;asrs`), where the ROM has `lsls;asrs` then `orrs`
 *     -- an s16 parameter's extension is a promotion, not a sinkable cast.
 *
 * (2) Each arm binds three locals. `dst` is what puts the tile index AFTER
 *     the two bl's; without it the index statements are emitted ahead of
 *     them and have to survive the calls. `t`/`u` are what stop `fold`
 *     reassociating `a8 * 4 + ((a4 & 1) + 0x1dc)` into
 *     `(a8 * 4 + 0x1dc) + (a4 & 1)`: split_tree() pulls an INTEGER_CST out of
 *     either operand of a PLUS, so the only way to keep the ROM's grouping is
 *     for the inner sum to be opaque. Probed: one local for the a4 term alone
 *     keeps the grouping but emits it BEFORE `a8 * 4`; two give the order too.
 *
 * (3) The table choice is an if/else writing a `void *` local, not a `?:`
 *     inside the argument list. The ROM evaluates the whole table lookup --
 *     branch, index, `ldr` -- before touching a1, a2 or a8, which argument
 *     evaluation of a COND_EXPR in position 3 does not do. It also stops
 *     `(a4 - 1) * 4` reassociating into `a4 * 4 - 4` with the multiply
 *     hoisted above the branch. Worth 4 bytes and ~50 differing bytes.
 *
 * a3 and a8 are byte-neutral between u16 and s16 here (probed all four
 * combinations, all four match); s16 is written for uniformity with a1/a2/a4
 * and is NOT proven.
 *
 * gUnknown_0849A218 and gUnknown_0849A22C are two separate 5-entry tables,
 * not one array: the ROM holds 5 words at each, and 0x0849A240 is unrelated
 * data (0x40000001). The index is `a4 - 1`, so a4 runs 1..5.
 */
void DrawCursorInfoUnitIcon(s16 a1, s16 a2, s16 a3, s16 a4, u8 a5, u8 a6, u8 a7, s16 a8)
{
    void *p;
    int n = 0;

    if (a5 != 0)
        a5 = 1;

    if (ShouldDrawTransportMarker(a3, gUnknown_030033EC, a4))
    {
        u8 *dst;
        int t;
        int u;

        dst = GetUnitExtraGraphics() + (0x78 - GetUnitSheetFrameTileCount()) * 0x20;
        t = a8 * 4;
        u = (a4 & 1) + 0x1dc;
        RegisterDataMove(dst, (void *)(0x06010000 + (t + u) * 0x20), 0x20);
        n++;
    }
    else
    {
        if (a5 != 0)
        {
            u8 *dst;
            int t;
            int u;

            dst = GetUnitExtraGraphics() + (0x75 - GetUnitSheetFrameTileCount()) * 0x20;
            t = a8 * 4;
            u = (a4 & 1) + 0x1dc;
            RegisterDataMove(dst, (void *)(0x06010000 + (t + u) * 0x20), 0x20);
            n++;
        }

        if (a6 != 0)
        {
            u8 *dst;
            int t;
            int u;

            dst = GetUnitExtraGraphics() + (0x76 - GetUnitSheetFrameTileCount()) * 0x20;
            t = a8 * 4;
            u = (a4 & 1) + 0x1dc;
            RegisterDataMove(dst, (void *)(0x06010000 + (t + u) * 0x20), 0x20);
            n++;
        }

        if (a7 & 0x20)
        {
            u8 *dst;
            int t;
            int u;

            dst = GetUnitExtraGraphics() + (0x77 - GetUnitSheetFrameTileCount()) * 0x20;
            t = a8 * 4;
            u = (a4 & 1) + 0x1dc;
            RegisterDataMove(dst, (void *)(0x06010000 + (t + u) * 0x20), 0x20);
            n++;
        }
    }

    RegisterDataMove(GetUnitSheetGraphics() + (GetUnitSpriteTile(a4, a3) & 0x3ff) * 0x20,
                 (void *)(0x06013B40 + a8 * 0x80), 0x80);

    if (n != 0)
        p = gUnknown_0849A218[a4 - 1];
    else
        p = gUnknown_0849A22C[a4 - 1];

    PutOamHi(a1, a2 | 0x400, p, (a8 * 4) | 0xf000);
}
asm(".global sub_0802B91C\n.thumb_set sub_0802B91C, DrawCursorInfoUnitIcon\n");
