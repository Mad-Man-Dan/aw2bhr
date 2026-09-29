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
void sub_0803D788(void)
{
    sub_0801237C();
    gUnknown_030030E0.bits.effect = 3;
    gUnknown_03001FFC = 0x1f;
    sub_08011B18();
    sub_080366C4(DefaultMainLoopCallback);
    sub_080366D0(DefaultVBlankCallback);
    sub_08012C58(gUnknown_0849D16C);
    sub_0801295C();
    Decompress(gUnknown_0823A3D4,
               (void *)(0x06000000 + gUnknown_0300251C.bits.chr_block * 0x4000));
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);
    sub_080130C8(gBG3TilemapBuffer, 0, 0x800);
    ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
    sub_08013B1C();
    sub_08011C68(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
    sub_08011C68(gBG1TilemapBuffer, (void *)0x0600F000, 0x800);
    sub_08011C68(gBG2TilemapBuffer, (void *)0x06007800, 0x800);
    sub_08011C68(gBG3TilemapBuffer, (void *)0x0600F800, 0x800);
    sub_08022A34();
    sub_0801B780(0);
    sub_0801A5B0(0);
    ApplyPaletteExt(gUnknown_0809165C, 0x140, 0x20);
}
