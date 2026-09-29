#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BD5C.
 * sub_0807BD5C @ 0x0807BD5C
 */

#include "hardware.h"
#include "proc.h"

/* MATCHED, wave 48 (W48-E). Byte-for-byte, relocs match.
 *
 * The two dead stores (`j = 0x06010080;` and `i = 0x060103c0;`) are NOT
 * decoration and must not be tidied away -- together they are the whole
 * difference between 94.0% and a match. Each gives the OTHER loop's counter a
 * reference inside this loop, which drops the dbra countdown below the
 * induction variables in allocno priority so it lands in r6 -- where the
 * original leaves it -- instead of stealing r4 or r5. The first was found by
 * decomp-permuter; the second is its mirror. See the "Cross-referencing the
 * OTHER loop's counter" chapter of docs/agbcc-codegen.md.
 *
 * Progression, so nobody re-walks it:
 *   one shared `int i;`         202 bytes (an extra `adds r5, r4, #0`)
 *   `int j, i;`, one per loop   200 bytes, 94.0%, register fields only
 *   + the two dead stores       MATCH
 *
 * The source loops are ASCENDING despite the ROM's `subs/cmp/bge`, confirmed
 * rather than assumed: each counter init sits LAST in its preheader. A clean
 * positive for W48-C's loop-direction test on an outermost loop.
 *
 * The two negative pool words (0xFFFFF980 = -0x680, 0xFFFFFD80 = -0x280) are
 * real subtractions from gUnknown_02010450 -- whatever object that symbol sits
 * in extends backwards from it, and the -0x280 run ends exactly at the symbol.
 */
void MissionTitleName_LoadGraphics(void)
{
    int j, i;

    Decompress(gUnknown_0822DC08, gUnknown_0200FC50);

    for (i = 0; i < 3; i++)
    {
        j = 0x06010080;
        CpuFastSet(gUnknown_0200FC50 + i * 0x80, (void *)(0x06010000 + i * 0x100), 0x20);
        CpuFastSet(gUnknown_0200FC50 + 0x400 + i * 0x80, (void *)(j + i * 0x100), 0x20);
    }

    CpuFastSet(gUnknown_02010450, (void *)0x06010300, 0x20);

    for (j = 0; j < 10; j++)
    {
        i = 0x060103c0;
        CpuFastSet(gUnknown_02010450 - 0x680 + j * 0x40, (void *)(0x06010380 + j * 0x80), 0x10);
        CpuFastSet(gUnknown_02010450 - 0x280 + j * 0x40, (void *)(i + j * 0x80), 0x10);
    }

    ApplyPaletteExt(gUnknown_0822DE80, 0x200, 0x20);
}
asm(".global sub_0807BD5C\n.thumb_set sub_0807BD5C, MissionTitleName_LoadGraphics\n");
