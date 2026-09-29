#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804FF44.
 * sub_0804FF44 @ 0x0804FF44, sub_0804FFFC @ 0x0804FFFC
 */

void sub_0804FF44(u16 a1)
{
    u16 t;

    if (a1 == gUnknown_0300450C)
    {
        if (gUnknown_08562128[gUnknown_03004580[a1][4]] != 4)
        {
            t = gUnknown_085D6A48[gUnknown_03004580[a1][1]][0] - 9;
            if (t <= 1)
            {
                CpuFastSet(gUnknown_08552138[gUnknown_08562128[gUnknown_03004580[a1][4]]],
                           (void *)(0x06010000 + (u16)((a1 << 13) + 0x1E00)), 0x80);
                gUnknown_0300453C = a1;
                gUnknown_0202967C[a1][0] =
                    sub_08015410(gUnknown_08553580, 1, 0, gUnknown_08559420, 0);
            }
        }
    }
}

/* MATCHED. Wave 37, W37-J.
 *
 * The one non-obvious thing is `p`. Written inline as
 * `oam.paletteNum = gUnknown_08551D0C[gUnknown_0300453C][0];` combine narrows
 * the LOAD to `ldrb` -- only four bits of the value reach the destination
 * bitfield, so force_to_mode narrows the MEM itself -- and the ROM has `ldrh`.
 * Binding the halfword to a `u16` local first is the whole fix; see the
 * "narrow bitfield destinations narrow the LOAD" section of
 * docs/agbcc-codegen.md.
 *
 * `oam.tileNum = gUnknown_0300453C * 0x100 + 0xF0` is a MULTIPLY, not a shift,
 * and the `& 0x3FF` / `& 0xFC00` pair around it is the 10-bit bitfield insert
 * rather than anything in the source. gUnknown_0855357C is a new u16-per-side
 * ROM table, declared in include/unknown-globals.h this wave.
 */
void sub_0804FFFC(void)
{
    struct OamData oam;
    u16 p;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    oam.hFlip = gUnknown_0300453C ^ 1;
    p = gUnknown_08551D0C[gUnknown_0300453C][0];
    oam.paletteNum = p;
    oam.tileNum = gUnknown_0300453C * 0x100 + 0xF0;
    oam.priority = 2;
    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    gUnknown_02029C0C[gUnknown_0300453C] = gUnknown_0855357C[gUnknown_0300453C];
    gUnknown_02029C10[gUnknown_0300453C] = 0x96;
}
