#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043E8C.
 * sub_08043E8C @ 0x08043E8C
 */

/* MATCHED (wave 37, W37-Q4), three attempts.
 * Needs "rodata": ["0x08091384"] in data/promoted.json (the force-addr word
 * holding &gUnknown_084A0090).  Note the SECOND reference to that same global,
 * in the CpuFastSet arm, is a plain literal-pool `ldr` in the ROM; the honest
 * spelling reproduces both, so do not try to make them agree.
 *
 * Stages the slot's 0x180-byte tile blob into gUnknown_02017C50 and rewrites
 * it into the caller's buffer, forcing every zero 4bpp nibble to a non-zero
 * palette index (1 per nibble position) so nothing in the portrait is
 * transparent.  a3 is either a palette slot (<= 14, handed to LoadCoPalette)
 * or a raw CpuFastSet destination.
 *
 * Two spellings were the whole cost, and both refute a rule in the wave brief:
 *
 * 1. `v &= M; v |= B;` as TWO statements, not `v = (v & M) | B;`.  With `u16
 *    v` the single-expression form is narrowed to HImode by convert_to_integer
 *    and no truncation is emitted; the compound-assignment pair leaves the
 *    `lsls #0x10; lsrs #0x10` zero-extension after the OR, which the ROM has
 *    at nibbles 3, 2 and 1.  Nibble 0 is the single-expression form -- its
 *    result goes straight to the `strh`, and the ROM has no truncation there.
 *    (`int v` with explicit `(u16)` casts is a third, different answer: it
 *    also changes the mask constants to their sign-extended forms 0xFFFFF0FF
 *    /0xFFFFFF0F/0xFFFFFFF0, which the ROM does not use.)
 *
 * 2. The INNER loop counts UP in the source -- `for (i = 0; i < 16; i++)` --
 *    and check_dbra_loop reverses it.  `subs r2,#1; cmp r2,#0; bge` looks like
 *    a genuine descending loop and is not; writing it as `for (i = 15; i >= 0;
 *    i--)` is byte-for-byte identical EXCEPT that the two insns at the top of
 *    the outer body come out in the other order.  The reversal inserts the new
 *    `i = 15` init at the END of the inner preheader, i.e. AFTER the outer
 *    biv's `adds r3,r0,#1`; a genuinely descending source puts the init first.
 *    That was the last 4 bytes, and ~19,400 permuter iterations did not find
 *    it.
 */
void LoadCoMiniPortraitOpaque(int a1, u16 *a2, int a3)
{
    u16 *src;
    int j;
    int i;
    u16 v;

    src = gUnknown_02017C50;
    CpuCopyAuto(gUnknown_084A0090[a1].miniPortrait, src, 0x180);
    for (j = 0; j < 12; j++)
    {
        for (i = 0; i < 16; i++)
        {
            v = *src++;
            if ((v & 0xf000) == 0)
            {
                v &= 0x0fff;
                v |= 0x1000;
            }
            if ((v & 0x0f00) == 0)
            {
                v &= 0xf0ff;
                v |= 0x0100;
            }
            if ((v & 0x00f0) == 0)
            {
                v &= 0xff0f;
                v |= 0x0010;
            }
            if ((v & 0x000f) == 0)
                v = (v & 0xfff0) | 0x0001;
            *a2++ = v;
        }
    }
    if (a3 <= 14)
        LoadCoPalette(a1, a3);
    else
        CpuFastSet(gUnknown_084A0090[a1 % 24].palette + GetLoadedCoPalette(a1 % 24) * 16, (void *)a3, 8);
}
asm(".global sub_08043E8C\n.thumb_set sub_08043E8C, LoadCoMiniPortraitOpaque\n");
