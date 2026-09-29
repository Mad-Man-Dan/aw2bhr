#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D53C.
 * sub_0806D53C @ 0x0806D53C, sub_0806D620 @ 0x0806D620
 */

/* The install half of RulesScreenDismissArmyColumns's teardown: one sub_080152EC(3) object per
 * live gUnknown_08580934->unk08 slot into unk34[], each given a tile block
 * uploaded by LoadCoFace.
 *
 * The two accumulators are BOTH strength-reduced givs off `i`, not source
 * locals, and the preheader ordering is what says so: the source's own
 * `i = 0` is the first instruction after the push, then the LICM hoists
 * (the constant 0 for unk46, and &gUnknown_08580934), and only then the giv
 * inits 0x190 and 0 -- the fixed three-pass order in docs/agbcc-codegen.md.
 * They are the same induction variable 0x190 apart, which is why `0x190 + i *
 * 0x24` appears once as a whole (the tile number handed to LoadCoFace) and
 * once as the sum `0x190 + (i * 0x24)` rebuilt in the loop for unk44.
 *
 * `i * 4` is shared with the unk34[] index scaling by CSE -- that is where
 * unk40's and unk42's `adds r1, r6, rC` come from, not from a separate
 * multiply.
 *
 * The `== 2` arm is the EARLIER one in the source: it is the arm carrying the
 * explicit `b` to the tail while the else arm falls through, which is the
 * wave-45 switch/if layout rule read backwards. */
void RulesScreenSpawnArmyColumns(void)
{
    struct Unk08580934_Obj *o;
    int i;

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        o = (struct Unk08580934_Obj *)sub_080152EC(gUnknown_08581ECC, 3);
        o->unk1c = i;
        gUnknown_08580934->unk34[i] = o;
        o->unk46 = 0;
        o->unk2c = GetArmyColumnX(i);
        o->unk2a = 0x28;
        o->unk24 = i * 2;
        o->unk44 = 0x190 + i * 0x24 + (i << 12);
        LoadCoFace(gUnknown_08580934->unk20[i],
                     (void *)(((0x190 + i * 0x24) & 0x3ff) * 32 + 0x06010000),
                     i + 0x10);
        o->unk40 = i * 4 - 0x7d70;
        if (gUnknown_08580934->unk09[i] == 2)
            o->unk42 = 0x42b0;
        else
            o->unk42 = i * 4 + 0x42a0;
    }
}
asm(".global sub_0806D53C\n.thumb_set sub_0806D53C, RulesScreenSpawnArmyColumns\n");

/* The third of the trio. It shares ONLY the frame with MatchSetupDismissRuleOptions and
 * RulesScreenDismissRuleOptions -- the loop body is genuinely different:
 *   - the table is unk34[], not unk54[];
 *   - the bound is DYNAMIC. `gUnknown_08580934->unk08` is re-read every
 *     iteration, so it stays in the `for` condition and must not be hoisted
 *     into a local; that is what puts the `ldrb r2,[r2,#8]` after the body and
 *     the entry `cmp r3,r0; bge` before it.
 *   - `o->unk28 / 32` on the s16 member is what emits `cmp #0; bge; adds #0x1f;
 *     asrs #5`. The +0x1f is agbcc's round-toward-zero bias for a signed power
 *     of two divide -- write the divide, not the bias. */
void RulesScreenDismissArmyColumns(void)
{
    struct Unk08580934 *g;
    struct Unk08580934_Obj *o;
    int i;

    if (FindSlotScript((s32)gUnknown_08581ECC) != -1)
    {
        for (i = 0; i < gUnknown_08580934->unk08; i++)
        {
            g = gUnknown_08580934;
            o = g->unk34[i];
            o->unk24 = o->unk28 / 32;
            o->unk34 = -5;
            o->unk30 = 0;
        }
        ClearCallbackOfSlotsRunningScript(gUnknown_08581ECC);
    }
}
asm(".global sub_0806D620\n.thumb_set sub_0806D620, RulesScreenDismissArmyColumns\n");
