#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037DC8.
 * sub_08037DC8 @ 0x08037DC8
 */

void sub_08037DC8(void)
{
    SetupBackgrounds(gUnknown_0849D16C);
    EnableVBlankInterrupt();
    FlushLCDControl();
    CpuCopyAuto(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
    CpuCopyAuto(gBG1TilemapBuffer, (void *)0x0600F000, 0x800);
    CpuCopyAuto(gBG2TilemapBuffer, (void *)0x06007800, 0x800);
    CpuCopyAuto(gBG3TilemapBuffer, (void *)0x0600F800, 0x800);
    CpuCopyAuto(gUnknown_080A0F38, (void *)0x06000020, 0x200);
    ApplyPaletteExt(gUnknown_080A1138, 0x20, 0x20);
    LoadCursorSpriteGraphics();
    LoadBg1WindowFrame(0);
}
