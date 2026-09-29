#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064B68.
 * sub_08064B68 @ 0x08064B68
 */

/* One of a verified trio with RulesScreenDismissRuleOptions and RulesScreenDismissArmyColumns: the same gate,
 * the same tail, three different loop bodies. gUnknown_0858096C is reached
 * through agbcc's own -fforce-addr pool word (the ROM word at 0x0816E0F4),
 * which is what the `ldr r4, =...; ldr r0, [r4]` double load is -- naming the
 * blob directly reproduces it. See the comment on gUnknown_0858096C in
 * include/unknown-globals.h.
 *
 * `p = &g->unk84;` is load-bearing. Written as `(&g->unk84)[i]` the address
 * folds to `(g + i) + 0x84` and the two `add`s come out in the ROM's opposite
 * order; binding the base to its own pointer local puts the constant's add
 * first, which is what the ROM has. Same lever as the one in the "p[X + C] vs
 * *(p + X + C)" note in docs/agbcc-codegen.md. */
void MatchSetupDismissRuleOptions(int a)
{
    struct Unk08580934 *g;
    struct Unk08580934_Obj *o;
    u8 *p;
    int i;

    if (FindSlotScript((s32)gUnknown_0858096C) != -1)
    {
        for (i = 0; i <= 6; i++)
        {
            g = gUnknown_08580934;
            o = g->unk54[i];
            p = &g->unk84;
            p[i] = o->unk48;
            o->unk24 = 6 - i;
            o->unk3c = a;
            o->unk3a = 0;
        }
        ClearCallbackOfSlotsRunningScript(gUnknown_0858096C);
    }
}
asm(".global sub_08064B68\n.thumb_set sub_08064B68, MatchSetupDismissRuleOptions\n");
