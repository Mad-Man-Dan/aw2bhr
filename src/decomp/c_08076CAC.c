#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076CAC.
 * sub_08076CAC @ 0x08076CAC, sub_08076D68 @ 0x08076D68
 */

/* Wave 35 (W35-B). gUnknown_081CC590 IS NOT A GLOBAL -- it is one of agbcc's
 * own `-fforce-addr` address-constant words, and the word at 0x081CC590 in
 * baserom.gba holds 0x0202FDFC, i.e. &gUnknown_0202FDFC. The ROM's
 * `ldr r3, =sym; ldr r5, [r3]; ldrh [r5, #4]` is pool word -> struct base ->
 * member, exactly two loads, which is what the honest spelling emits.
 *
 * The previous park at 80.3% declared a fictional `struct Unk81CC590 *` global
 * and bound `&gUnknown_081CC590` to a `**` local; that reproduced the
 * instruction stream but not the register allocation (37 bytes, every one a
 * register number). See docs/agbcc-codegen.md, wave 35 (W35-B).
 *
 * Clamps the camera's X target (+0x04) against the origin (+0x00) and reports
 * whether either moved. ox/oy are s16 locals: the ROM loads them `ldrh` and
 * sign-extends again at the closing compare. `t` has to be its own local so the
 * guard is fully evaluated before `a`, and it is REUSED for the inner
 * `unk00 + d` -- that reuse is what puts the inner value in the same register
 * the guard used, and it was the last 3 bytes. The first clamp arm is `+= d`,
 * not `= ox + d`: `+=` reuses the halfword CSE already holds. */
int MoveWorldMapCursorX(s16 d)
{
    s16 ox = gUnknown_0202FDFC.unk04;
    s16 oy = gUnknown_0202FDFC.unk00;
    int t = gUnknown_0202FDFC.unk04 + gUnknown_0202FDFC.unk00 + d;
    int a = gUnknown_0202FDFC.unk04 + d;

    if ((u32)t <= 0x19F)
    {
        if ((u32)(a - 0x20) <= 0x9F)
        {
            gUnknown_0202FDFC.unk04 += d;
        }
        else
        {
            t = gUnknown_0202FDFC.unk00 + d;

            if ((a <= 0xBF && t >= 0 && d < 0)
                || (a > 0x20 && t <= 0xBF && d > 0))
                gUnknown_0202FDFC.unk00 += d;
            else if (a < 0)
                gUnknown_0202FDFC.unk04 = 0;
            else if (a > 0xE0)
                gUnknown_0202FDFC.unk04 = 0xE0;
            else
                gUnknown_0202FDFC.unk04 += d;
        }
    }

    if (ox != gUnknown_0202FDFC.unk04 || oy != gUnknown_0202FDFC.unk00)
        return 1;

    return 0;
}
asm(".global sub_08076CAC\n.thumb_set sub_08076CAC, MoveWorldMapCursorX\n");

/* Wave 35 (W35-B). MoveWorldMapCursorX's Y axis. gUnknown_081CC594 is NOT a global:
 * the word at 0x081CC594 in baserom.gba holds 0x0202FDFC, so it is agbcc's
 * `-fforce-addr` address constant for &gUnknown_0202FDFC, one private copy per
 * function. See work/MoveWorldMapCursorX and docs/agbcc-codegen.md (wave 35, W35-B).
 *
 * Same shape as its X twin, with the vertical limits (0xEF, 0x4F, 0x6F, 0x5F,
 * 0x90) in place of the horizontal ones and the +0x06/+0x02 pair in place of
 * +0x04/+0x00. */
int MoveWorldMapCursorY(s16 d)
{
    s16 ox = gUnknown_0202FDFC.unk06;
    s16 oy = gUnknown_0202FDFC.unk02;
    int t = gUnknown_0202FDFC.unk06 + gUnknown_0202FDFC.unk02 + d;
    int a = gUnknown_0202FDFC.unk06 + d;

    if ((u32)t <= 0xEF)
    {
        if ((u32)(a - 0x20) <= 0x4F)
        {
            gUnknown_0202FDFC.unk06 += d;
        }
        else
        {
            t = gUnknown_0202FDFC.unk02 + d;

            if ((a <= 0x6F && t >= 0 && d < 0)
                || (a > 0x20 && t <= 0x5F && d > 0))
                gUnknown_0202FDFC.unk02 += d;
            else if (a < 0)
                gUnknown_0202FDFC.unk06 = 0;
            else if (a > 0x90)
                gUnknown_0202FDFC.unk06 = 0x90;
            else
                gUnknown_0202FDFC.unk06 += d;
        }
    }

    if (ox != gUnknown_0202FDFC.unk06 || oy != gUnknown_0202FDFC.unk02)
        return 1;

    return 0;
}
asm(".global sub_08076D68\n.thumb_set sub_08076D68, MoveWorldMapCursorY\n");
