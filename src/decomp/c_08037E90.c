#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037E90.
 * sub_08037E90 @ 0x08037E90
 */

void sub_08037E90(void)
{
    sub_08012C58(gUnknown_0849D16C);
    sub_0801295C();
    sub_080128D0();
    sub_08011C68(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
    sub_08011C68(gBG1TilemapBuffer, (void *)0x0600F000, 0x800);
    sub_08011C68(gBG2TilemapBuffer, (void *)0x06007800, 0x800);
    sub_08011C68(gBG3TilemapBuffer, (void *)0x0600F800, 0x800);
    sub_08022A34();
    sub_080366C4(DefaultMainLoopCallback);
    sub_080366D0(DefaultVBlankCallback);
}
