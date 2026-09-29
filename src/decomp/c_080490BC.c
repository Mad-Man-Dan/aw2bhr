#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080490BC.
 * sub_080490BC @ 0x080490BC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"

/* A proc tick. B (gpKeySt->unk0c & 2) re-opens the view and bumps unk83c; the
 * proc then breaks unless unk836 is set AND unk83c has fallen back to zero,
 * waits out sub_08019260 and the ShopScreen_StepOffsetToMinus38 slide, and finally repaints and
 * breaks.
 *
 * `v` is not cosmetic: the ROM passes the ShopScreen_StepOffsetToMinus38 result straight back as
 * FillTilemapRect's sixth argument (`str r2, [sp, #4]` reusing the register the
 * `lsr #0x10` left it in), which is also what proves ShopScreen_StepOffsetToMinus38 returns a
 * value rather than a flag.
 *
 * gUnknown_0812A15C is a -fforce-addr address word holding &gUnknown_084C30F8,
 * not a global -- the same word as 0x0812A150/154/158. Four references across
 * a merge is what makes agbcc park &gUnknown_084C30F8 in r4 here. */
void ShopScreen_WaitGreetingThenSlide(ProcPtr proc)
{
    u16 v;

    if ((gpKeySt->unk0c & 2) != 0)
    {
        ShopScreen_EndMessage();
        gUnknown_084C30F8->unk83c++;
    }

    if (gUnknown_084C30F8->unk836 == 0 || gUnknown_084C30F8->unk83c != 0)
    {
        Proc_Break(proc);
        return;
    }

    if (sub_08019260())
        return;

    v = ShopScreen_StepOffsetToMinus38();

    if (v != 0)
        return;

    gUnknown_084C30F8->unk839++;

    if (gUnknown_084C30F8->unk836 != 0)
        FillTilemapRect(gBG0TilemapBuffer, 7, 0xf, 0x17, 4, v);

    PlayMusicOrSfx2(0x71);
    Proc_Break(proc);
}

asm(".global sub_080490BC\n.thumb_set sub_080490BC, ShopScreen_WaitGreetingThenSlide\n");
