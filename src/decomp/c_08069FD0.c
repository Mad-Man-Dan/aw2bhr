#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08069FD0.
 * sub_08069FD0 @ 0x08069FD0
 */

/* The near-twin of LoadIntroScreenGraphicsWithBg1: same seven steps, but the first blob goes to
 * 0x06008000 instead of 0x06000000, the gBG1TilemapBuffer blob is absent, and
 * there is no BG_EnableSyncBG1 call. 0x06000000 is reachable as `0xc0 << 19` and
 * 0x06008000 is not, which is why that one destination is a pool word here and
 * a shifted immediate there. */
void LoadIntroScreenGraphics(void)
{
    int zero;

    zero = 0;
    CpuFastSet(&zero, gUnknown_08580E60, 0x01000400);
    Decompress(gUnknown_08184FF4, (void *)0x06008000);
    Decompress(gUnknown_08185F0C, (void *)0x0600C000);
    ApplyPaletteExt(gUnknown_081866D8, 0xc0, 0x20);
    Decompress(gUnknown_0818633C, gBG2TilemapBuffer);
    Decompress(gUnknown_08186460, gUnknown_08580E60);
    BG_EnableSyncBG2();
    RegisterDataMove(gUnknown_08580E60, (void *)0x0600F000, 0x1000);
}
asm(".global sub_08069FD0\n.thumb_set sub_08069FD0, LoadIntroScreenGraphics\n");
