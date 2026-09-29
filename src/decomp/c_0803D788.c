#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D788.
 * sub_0803D788 @ 0x0803D788
 */

#include "hardware.h"

/* The screen-setup half of the 0x0803D770 group: BG3 char base comes out of
 * gUnknown_0300251C.bits.chr_block, and the four gUnknown_084995xx buffers get
 * the same 0x800-byte copy, which is why the length lives in r4 across all
 * four calls rather than being rebuilt. */
void SaveScreen_Init(void)
{
    ResetWindowShadows();
    gUnknown_030030E0.bits.effect = 3;
    gUnknown_03001FFC = 0x1f;
    sub_08011B18();
    sub_080366C4(DefaultMainLoopCallback);
    sub_080366D0(DefaultVBlankCallback);
    SetupBackgrounds(gUnknown_0849D16C);
    EnableVBlankInterrupt();
    Decompress(gUnknown_0823A3D4,
               (void *)(0x06000000 + gUnknown_0300251C.bits.chr_block * 0x4000));
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);
    sub_080130C8(gBG3TilemapBuffer, 0, 0x800);
    ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
    BG_EnableSyncBG3();
    CpuCopyAuto(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
    CpuCopyAuto(gBG1TilemapBuffer, (void *)0x0600F000, 0x800);
    CpuCopyAuto(gBG2TilemapBuffer, (void *)0x06007800, 0x800);
    CpuCopyAuto(gBG3TilemapBuffer, (void *)0x0600F800, 0x800);
    LoadCursorSpriteGraphics();
    InitTextTileCache(0);
    LoadBg1WindowFrame(0);
    ApplyPaletteExt(gUnknown_0809165C, 0x140, 0x20);
}
asm(".global sub_0803D788\n.thumb_set sub_0803D788, SaveScreen_Init\n");
