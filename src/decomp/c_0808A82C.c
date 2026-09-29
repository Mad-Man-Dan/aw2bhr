#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A82C.
 * sub_0808A82C @ 0x0808A82C, sub_0808A844 @ 0x0808A844, sub_0808A884 @ 0x0808A884
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"

/* sub_08014824 is declared `int sub_08014824(void)` and takes NOTHING: r0 still
 * holds the proc at the `bl` only because nothing overwrote it, which is a
 * coincidence of the `adds r4, r0, #0` copy and not an argument. The bare
 * `cmp r0, #0` with no shift pair is what keeps the return `int`. */
void CampaignIntro_IDLE_0808A82D(ProcPtr proc)
{
    if (sub_08014824() == 0)
        Proc_Break(proc);
}

/* `ldrh r1, [r0, #4]` off gpKeySt is the `held` member, and bit 0 is A.
 * Six statements in the arm, all sequential: nothing between the `bl`s reads
 * r0, so none of them is nested inside another. */
void CampaignIntro_IDLE_0808A845(ProcPtr proc)
{
    if (gpKeySt->pressed & 1)
    {
        sub_08013C00();
        sub_08013AEC();
        sub_08014BC0(proc);
        StartEventScript(gUnknown_084A0D58);
        sub_0801A5B0(0);
        Proc_Break(proc);
    }
}

/* TWO separate `if`s, not an `||`: the first arm's Proc_Start falls straight
 * into the gpKeySt test rather than branching past it, and both arms share one
 * pool word for the script. `lsls r0, r0, #0x18` after sub_08019260 is agbcc
 * re-narrowing its declared `bool8` return. Bit 3 of `held` is Start. */
void CampaignIntro_IDLE_0808A885(ProcPtr proc)
{
    if (!sub_08019260())
        Proc_Start(gUnknown_08617094, proc);

    if (gpKeySt->pressed & 8)
        Proc_Start(gUnknown_08617094, proc);
}

asm(".global sub_0808A82C\n.thumb_set sub_0808A82C, CampaignIntro_IDLE_0808A82D\n"
    ".global sub_0808A844\n.thumb_set sub_0808A844, CampaignIntro_IDLE_0808A845\n"
    ".global sub_0808A884\n.thumb_set sub_0808A884, CampaignIntro_IDLE_0808A885\n");
