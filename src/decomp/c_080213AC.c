#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080213AC.
 * sub_080213AC @ 0x080213AC
 */

/* The per-turn refresh: repaint the two 0x508-byte overlay planes at
 * gUnknown_08499590 + 0x1E42 for the two cursor slots gUnknown_03004070 and
 * gUnknown_03004088, then -- only in the gPlaySt.unk0d mode -- re-run
 * every army's turn-start pass and stamp StampVisionByPlaneMask over each record
 * GetInventionRecordByIndex hands back.
 *
 * gUnknown_08090964 and gUnknown_08090968 in the asm are NOT globals: the ROM
 * words there hold 0x03003FC0 and 0x08499590, so they are agbcc's own
 * -fforce-addr address constants for gPlaySt and gUnknown_08499590.
 * Both are named several times across a control-flow merge here, which is the
 * documented trigger; the honest spelling reproduces them.
 *
 * The record walk reads offset 2 of struct Unk02028360 two different widths --
 * `ldrh` masked with 0x3C0 for the loop test and `ldrb` with shift pairs in the
 * body -- and that is exactly the already-declared bitfield triple
 * unk02_0:3 / unk02_3:3 / unk02_6:4.  The `ldrh; ands #0x3C0; cmp #0` is
 * fold's optimize_bit_field_compare rewriting `unk02_6 != 0` into a test of the
 * containing halfword; the two body reads are ordinary extract_bit_field shift
 * pairs (`lsls #0x1d/lsrs #0x1d` and `lsls #0x1a/lsrs #0x1d`), which a plain
 * `& 7` on a byte member could not produce -- that spells `movs #7; ands`.
 *
 * `lsls r7,r2,#0x10` in the inner preheader is the loop optimiser hoisting the
 * (s16) cast of x; x and y are plain ints. */

void RebuildVisionPlanes(void)
{
    struct Unk02028360 *p;
    int x;
    int y;
    int xend;
    int yend;

    if (gPlaySt.fog != 0
        && gPlayers[gUnknown_030033EC].aiControlled == 2)
    {
        sub_08020754(&gMap->visible[
                         gUnknown_03004070 * 0x508]);
    }
    else
    {
        FillMapBuffer(&gMap->visible[
                         gUnknown_03004070 * 0x508],
                     1 - gPlaySt.fog);
    }

    FillMapBuffer(&gMap->visible[
                     gUnknown_03004088 * 0x508],
                 1 - gPlaySt.fog);

    if (gPlaySt.fog != 0)
    {
        StampArmyVision(1);
        StampArmyVision(2);
        StampArmyVision(3);
        StampArmyVision(4);

        p = GetInventionRecordByIndex(0);

        while (p->unk02_6 != 0)
        {
            xend = p->unk00 + p->unk02_0;
            yend = p->unk01 + p->unk02_3;

            for (x = p->unk00; x < xend; x++)
            {
                for (y = p->unk01; y < yend; y++)
                    StampVisionByPlaneMask(x, y, 0, 3, 1, 0);
            }

            p++;
        }
    }
}
asm(".global sub_080213AC\n.thumb_set sub_080213AC, RebuildVisionPlanes\n");
