#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08061668.
 * sub_08061668 @ 0x08061668
 */

/* Picks the lowest-cost entry of the 0xFF-terminated gUnknown_085766E4 list
 * whose unk02 matches gUnknown_0857680F[gUnknown_030046C0.unk06], writes its
 * unk00/unk01 coordinate pair through `out`, marks the chosen record consumed
 * (unk03 = 0xFE) and returns whether anything was found. 9999 is the "nothing
 * found" sentinel left in out[0].
 *
 * Two spellings here are measured, not stylistic:
 *
 *  - the sentinel goes through a named `u16` local. Storing the literal
 *    directly (`out[0] = 9999;`) costs ONE EXTRA INSTRUCTION: agbcc builds the
 *    pool constant in an SImode pseudo and then copies it into an HImode one
 *    for the `strh` (`ldr r2,pool; adds r1,r2,#0; strh r1,[r0]`). Assigning it
 *    to a u16 local first makes the pseudo HImode from the start and the `ldr`
 *    writes the store's own register (`ldr r1,pool; strh r1,[r0]`). Probed on
 *    its own in isolation, so this is about HImode stores generally and not
 *    about this function; the (u16) cast, a `s16 *` destination and a
 *    volatile-style deref all still emit the copy.
 *
 *  - the list is walked by SUBSCRIPT, not with a pointer local. Both shapes
 *    strength-reduce to the same `adds r1,#4` walker, but the subscript form
 *    keeps the base register alive beside it (the ROM's `adds r1,r3,#0` seed)
 *    and reproduces the ROM's register assignment exactly; a `p = gUnknown_...;
 *    p++` local walks the base itself and permutes r1/r2/r3/ip/r8.
 *
 * The re-read of the global at the end (`ldr r0,[pool]; ldr r1,[r0]`) is
 * -fforce-addr: the symbol is named from two basic blocks, so agbcc gives it a
 * private .rodata word -- that word is the ROM's gUnknown_0816DAFC, which holds
 * 0x085766E4. */
int AiPickBestScoredBuildSite(u16 *out)
{
    u16 none;
    int i;
    int best;
    u8 lim;

    i = 0;
    best = 0;
    lim = 0xfe;
    none = 9999;
    out[0] = none;

    while (gUnknown_085766E4[i].unk00 != 0xff)
    {
        if (gUnknown_085766E4[i].unk03 < lim
            && gUnknown_0857680F[gUnknown_030046C0.unk06] == gUnknown_085766E4[i].unk02)
        {
            out[0] = gUnknown_085766E4[i].unk00;
            out[1] = gUnknown_085766E4[i].unk01;
            lim = gUnknown_085766E4[i].unk03;
            best = i;
        }
        i++;
    }

    if (out[0] == 9999)
        return 0;

    gUnknown_085766E4[best].unk03 = 0xfe;
    return 1;
}
asm(".global sub_08061668\n.thumb_set sub_08061668, AiPickBestScoredBuildSite\n");
