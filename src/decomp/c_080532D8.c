#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080532D8.
 * sub_080532D8 @ 0x080532D8
 */

#include "proc.h"

/* Wave 50, W50-F. A teardown sweep: five proc-id slots per (side, slot), each
 * killed with the same `if (id != -1 && IsSlotScriptActiveAt(id)) EndSlotScriptAt(id);`
 * guard. The FIRST of the five carries an extra outer `!= 0` test whose false
 * arm jumps past the next three groups -- so the last group, gUnknown_02029668,
 * sits outside that `if` and runs unconditionally.
 *
 * The inner loop's `sl` countdown, `r7`/[sp,#8] byte offsets and the r5 cursor
 * are all strength_reduce givs off a plain ascending `for (j = 0; j < 5; j++)`;
 * `subs; cmp #0; bge` at the bottom is check_dbra_loop and not a descending
 * source loop.
 *
 * gUnknown_03004570 had to be retyped s16 -- see the comment on its
 * declaration. */
void BattleAnimScene_OnEnd(ProcPtr proc)
{
    int i;
    int j;

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (gUnknown_02029A10[i].entries[j].unk18 != 0)
            {
                if (gUnknown_02029A10[i].entries[j].unk18 != -1
                    && IsSlotScriptActiveAt(gUnknown_02029A10[i].entries[j].unk18))
                {
                    EndSlotScriptAt(gUnknown_02029A10[i].entries[j].unk18);
                    gUnknown_02029A10[i].entries[j].unk18 = 0;
                }

                if (gUnknown_020296B0[i].unk02[j] != -1
                    && IsSlotScriptActiveAt(gUnknown_020296B0[i].unk02[j]))
                    EndSlotScriptAt(gUnknown_020296B0[i].unk02[j]);

                if (gUnknown_020298E0[i].unk02[j] != -1
                    && IsSlotScriptActiveAt(gUnknown_020298E0[i].unk02[j]))
                    EndSlotScriptAt(gUnknown_020298E0[i].unk02[j]);

                if (gUnknown_0202967C[i][j] != -1
                    && IsSlotScriptActiveAt(gUnknown_0202967C[i][j]))
                    EndSlotScriptAt(gUnknown_0202967C[i][j]);
            }

            if (gUnknown_02029668[i][j] != -1
                && IsSlotScriptActiveAt(gUnknown_02029668[i][j]))
                EndSlotScriptAt(gUnknown_02029668[i][j]);
        }
    }

    if (gUnknown_03004570 != -1 && IsSlotScriptActiveAt(gUnknown_03004570))
        EndSlotScriptAt(gUnknown_03004570);

    EndAllSpriteScripts();
    Proc_End(proc);
    ClearMainLoopFrameMaskAndEnableSpriteLayer();
    EnableSpriteLayerMode();
}
asm(".global sub_080532D8\n.thumb_set sub_080532D8, BattleAnimScene_OnEnd\n");
