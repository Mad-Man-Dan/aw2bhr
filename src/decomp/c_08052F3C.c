#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052F3C.
 * sub_08052F3C @ 0x08052F3C
 */

/* Wave 51, W51-A. The 2 x 8 halfword row copy, then two counters cleared.
 *
 * The row pointers are what reproduce the ROM's loop: the shared `i * 16` is
 * CSEd into one `lsls` feeding both `adds`, and check_dbra_loop then reverses
 * the ascending `j` into `movs r3,#7 ... subs; cmp #0; bge` while both address
 * givs still ASCEND. A plain `gUnknown_03004550[i][j] = gUnknown_08551A08[i*8+j]`
 * leaves the counter un-reversed and swaps the two pointers' registers.
 *
 * `t` is what puts the constant in r0 at the tail. The two trailing stores are
 * `movs r0,#0; ldr r1,=..; strh; ldr r1,=..; strh` -- the CONSTANT's pseudo is
 * created before either address pseudo, which `g = 0; g2 = 0;` does not do here.
 * The wave-45 note blamed expand_assignment expanding the lvalue first; that is
 * not the mechanism. Measured this wave: the tail comes out constant-first iff a
 * literal 0 pseudo already exists earlier in the function for CSE to reuse. With
 * the double-subscript loop above, the ascending counter's own `mov r3,#0`
 * supplies one and the tail is correct by accident; once dbra reverses that
 * counter to 7 the only remaining 0 is `i`'s initialiser, which is a variable's
 * init and not a shared constant, so the address wins r0 instead. Binding the 0
 * to a local restores it without costing an instruction.
 *
 * The wave-45 note also recorded that gUnknown_03004550 "must stay FLAT" and
 * that a `[][8]` reshape "is not available as a lever". Wave 50 reshaped it to
 * `s16 [][8]` anyway, which is what made the row-pointer spelling above possible
 * -- the old draft no longer even compiles.
 */
void BattleAnimParamMenu_ResetToDefaults(void)
{
    int i;
    int j;
    s16 *dst;
    const s16 *src;
    int t;

    for (i = 0; i < 2; i++)
    {
        dst = gUnknown_03004550[i];
        src = gUnknown_08551A08 + i * 8;

        for (j = 0; j < 8; j++)
            dst[j] = src[j];
    }

    t = 0;
    gUnknown_030045AC = t;
    gUnknown_03004540 = t;
}
asm(".global sub_08052F3C\n.thumb_set sub_08052F3C, BattleAnimParamMenu_ResetToDefaults\n");
