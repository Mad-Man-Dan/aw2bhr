#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039F80.
 * sub_08039F80 @ 0x08039F80
 */

/* MATCHED wave 51 (W51-D). Redraws the unit-info panel: a stat pair, a label,
 * and -- only when the unit has more than one of whatever GetUnitFiringRangeWithCoBonus counts
 * -- a second row of the same three. Every row x is gUnknown_0849D89C->unk00
 * plus a fixed column, nudged 4 left when the value shown is a single digit.
 *
 * The nudges are CONDITIONAL EXPRESSIONS, not k = 0; plus an if (worth 16
 * bytes): do_store_flag computes the operand first, presets 0, then
 * conditionally overwrites.
 *
 * THE FIX, after the draft sat at 97.6% with the second row record pointer and
 * nudge holding each other registers. Two facts, both about live-range length
 * rather than statement order (see docs/agbcc-codegen.md):
 *
 *   - k is an int holding 0xFC, sign-extended with an explicit (s8) at each
 *     use -- NOT an s8 local. The two spellings are byte-identical at the
 *     definition and at the use, and differ only in how many pseudos they
 *     create: a narrow local assignment is a truncating copy that combine
 *     propagates away, splitting k into one short-lived pseudo per row, while
 *     an int keeps ONE pseudo spanning both. The ROM holds the nudge in r3 in
 *     BOTH rows, which one pseudo can do and two cannot. That long live range
 *     is what wins r3 and pushes the record pointer down to r2.
 *   - p is declared with the others but assigned ONLY in the second row. The
 *     draft also assigned it in the prologue, making it the longest-lived
 *     pseudo in the function and handing it r3. CSE serves the prologue and the
 *     first row from the q load, exactly as the ROM does.
 *
 * Binding the second row record to a local r was in the draft as a claimed fix
 * and is the OPPOSITE: removing it is what put the pointer in r2. Allocation
 * order here is longest-live-range-first, and the first allocated gets r3
 * (ARM REG_ALLOC_ORDER is 3, 2, 1, 0).
 */
void UnitInfoPanel_DrawAmmoAndRange(void)
{
    struct Unit *q;
    int k;
    struct Unk0849D89C *p;

    q = gUnknown_0849D89C->unk04;

    if (gUnknown_085D5ABC[q->type].maxAmmo != 0)
    {
        k = q->ammo <= 9 ? 0xFC : 0;
        DrawSpriteNumberFont1((u16)((s8)k + 0x40 + gUnknown_0849D89C->unk00), 0x38, q->ammo);

        k = gUnknown_085D5ABC[q->type].maxAmmo <= 9 ? 0xFC : 0;
        p = gUnknown_0849D89C;
        DrawSpriteNumberFont1((u16)((s8)k + 0x49 + p->unk00), 0x40,
                     gUnknown_085D5ABC[q->type].maxAmmo);

        PutOamHi(gUnknown_0849D89C->unk00 + 0x41, 0x3c, gUnknown_0849D8A0, 0x13CA);

        if (GetUnitFiringRangeWithCoBonus(gUnknown_0849D89C->unk08, q->type) > 1)
        {
            DrawOamObject(0x3b, gUnknown_0849D89C->unk00 + 0x4c, 0x38, 0, 0);
            DrawSpriteNumberFont1(gUnknown_0849D89C->unk00 + 0x50, 0x40,
                         gUnknown_085D5ABC[q->type].minRange);
            PutOamHi(gUnknown_0849D89C->unk00 + 0x58, 0x41, gUnknown_0849D8A0, 0x13CB);
            DrawSpriteNumberFont1(gUnknown_0849D89C->unk00 + 0x60, 0x40,
                         GetUnitFiringRangeWithCoBonus(gUnknown_0849D89C->unk08, q->type));
        }
    }
}
asm(".global sub_08039F80\n.thumb_set sub_08039F80, UnitInfoPanel_DrawAmmoAndRange\n");
