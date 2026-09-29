#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080790D0.
 * sub_080790D0 @ 0x080790D0
 */

/* MATCHED, wave 87 (W87-B).
 *
 * Wave 86 (W86-F) took this from 50.0% to 92.0% size-exact with the twin
 * MatchSummaryPanel_LoadGraphics's "scale i, never the pointer" construct, leaving 18 bytes: the
 * two loop-invariant bases of loop 1 were emitted in the opposite order and
 * therefore traded registers.  Three things closed the rest, and the order
 * matters because each one only became visible once the previous was in place:
 *
 * 1. The two bases are SOURCE-LEVEL WALKERS, not compiler givs.  `src` and
 *    `dst` are bound before the inner loop and stepped by `src += 0x400;` /
 *    `dst += 0x100;` in the body.  This alone put SRC in the ROM's r5 and DST
 *    in r4.  Every attempt to reach the same order by BINDING the base to a
 *    local while the body still re-derived it -- W86-F's outer-body
 *    `q = (u8 *)(off + (int)p);`, the same bind in the inner for-init, and
 *    binding the whole address `q = (u8 *)(off + (int)p + j * 0x400);` inside
 *    the body -- is BYTE-IDENTICAL to the plain draft: cse folds a bind whose
 *    value the body recomputes.  A walker cannot be folded, because the
 *    increment makes the local the only carrier of the value.
 *
 * 2. `k = i * 0x800;` as its own statement ahead of `src`.  The ROM emits
 *    `lsls r0,r7,#0xb` at the TOP of the outer body and its consumer
 *    `adds r4,r0,r1` five insns later; that split is cse reusing a pseudo a
 *    separate earlier statement produced, and it is also what pushes p's copy
 *    off r0 onto the ROM's `mov r1,r8`.
 *
 * 3. The inner counter is DESCENDING IN THE SOURCE.  With `for (j = 0; j < 8;
 *    j++)` check_dbra_loop rewrites the init to `j = 7` but re-emits it
 *    IMMEDIATELY BEFORE the loop, so it always lands last in the preheader.
 *    The ROM has `movs r6,#7` in the MIDDLE, between `src` and `dst` -- only a
 *    source-level `j = 7` stays where it is written.  Addresses stay ascending
 *    because they are walkers and do not depend on j.
 *
 * Loop 2 keeps W86-F's scaled-index form; loops, pool and tail unchanged. */
void ResultsScreen_LoadGraphics(void)
{
    u8 *p;
    u8 *src;
    u8 *dst;
    int i;
    int j;
    int k;
    int off;

    Decompress(gUnknown_08227F3C, gUnknown_0200FC50);
    for (i = 0, p = gUnknown_0200FC50, off = 0; i < 4; i++)
    {
        k = i * 0x800;
        src = (u8 *)(off + (int)p);
        for (j = 7, dst = (u8 *)(0x06012800 + k); j >= 0; j--)
        {
            CpuFastSet(src, dst, 0x40);
            dst += 0x100;
            src += 0x400;
        }
        off += 0x100;
    }
    for (i = 0; i < 4; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (i * 2 + 0xa5) * 0x40, (void *)(0x06015B00 + i * 0x100), 0x20);
        CpuFastSet(gUnknown_0200FC50 + (i * 2 + 0xb5) * 0x40, (void *)(0x06015B80 + i * 0x100), 0x20);
    }
    CpuFastSet(gUnknown_02012790, (void *)0x06015F00, 0x10);
    CpuFastSet(gUnknown_02012790 + 0x400, (void *)0x06015F40, 0x10);
    ApplyPaletteExt(gUnknown_0822AA80, 0x280, 0x40);
}
asm(".global sub_080790D0\n.thumb_set sub_080790D0, ResultsScreen_LoadGraphics\n");
