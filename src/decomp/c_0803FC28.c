#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803FC28.
 * sub_0803FC28 @ 0x0803FC28
 */

/* MATCHED (wave 49, W49-C), first attempt. 344/344 bytes, relocs match.
 *
 * A rectangle query over the same 0x02028360 decoration list InventionTurn_PrepareNextFire walks:
 * skip every record whose (unk00, unk01) extent misses the (a1, a2, a3, a4) box,
 * then dispatch on unk02_6.
 *
 * Every field here is an EXISTING struct Unk02028360 bitfield, and the ROM shows
 * three different extraction widths for one halfword, all of them get_best_mode
 * picking the narrowest unit that covers the field: unk02_0 and unk02_3 are
 * `ldrb [r5,#2]` plus one shift pair each (bits 0..5 live in byte 2), unk02_6 is
 * `ldrh [r5,#2]; lsls #0x16; lsrs #0x1c` (bits 6..9 span two bytes), and
 * unk02_e is `ldrb [r5,#3]; ands #0xc0; cmp #0x40` -- a MASK-AND-COMPARE, not an
 * extract, which is what `== 1` on a 2-bit field always folds to.
 *
 * The `subs #2; cmp #6; bhi` switch is the dense jump-table form over 2..8; the
 * table's entries for 4 and 6 point at the default, which needs no case label of
 * its own -- 5 cases across 7 slots is dense enough for expand_end_case to take
 * the table. Bodies come out in source order 3, 5, 8, 2, 7, and jump.c merges
 * cases 3 and 5 onto one PutMapObjectSprite tail and merges case 3's `unk04 == 0` arm
 * into case 5's; both are cross-jumping, not shared source.
 *
 * `goto next` rather than `continue`: the guards sit above a switch whose arms
 * `break`, so a `continue` would read the same but the loop is a `while` with
 * the increment at the bottom either way.
 */

void DrawInventionSprites(int a1, int a2, int a3, int a4)
{
    struct Unk02028360 *p;
    const u8 *t;

    p = GetInventionRecordByIndex(0);
    while (p->unk02_6 != 0)
    {
        if (p->unk00 + p->unk02_0 < a1)
            goto next;
        if (a1 + a3 < p->unk00)
            goto next;
        if (p->unk01 + p->unk02_3 < a2)
            goto next;
        if (a2 + a4 < p->unk01)
            goto next;
        switch (p->unk02_6)
        {
        case 3:
            if (p->unk04 == 0)
                t = gUnknown_0849FA3C;
            else if (p->unk02_e == 1)
                t = gUnknown_0849FA08;
            else
                t = gUnknown_0849FA22;
            PutMapObjectSprite(p->unk00, p->unk01, t, GetArmyByTeamColor(5), 0);
            break;
        case 5:
            if (p->unk04 == 0)
                t = gUnknown_0849FA3C;
            else if (IsHardCampaignMode())
                t = gUnknown_0849FA78;
            else
                t = gUnknown_0849FA5E;
            PutMapObjectSprite(p->unk00, p->unk01, t, GetArmyByTeamColor(5), 0);
            break;
        case 8:
            PutMapObjectSprite(p->unk00, p->unk01, gUnknown_0849FA56, GetArmyByTeamColor(5), 0);
            break;
        case 2:
            PutMapObjectSprite(p->unk00, p->unk01, gUnknown_0849FA56, -1, 0);
            break;
        case 7:
            PutMapObjectSprite(p->unk00, p->unk01, gUnknown_0849FA9A, GetArmyByTeamColor(5), 0);
            break;
        }
    next:
        p++;
    }
}
asm(".global sub_0803FC28\n.thumb_set sub_0803FC28, DrawInventionSprites\n");
