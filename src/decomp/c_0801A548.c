#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A548.
 * sub_0801A548 @ 0x0801A548, sub_0801A57C @ 0x0801A57C, sub_0801A5B0 @ 0x0801A5B0, sub_0801A5E0 @ 0x0801A5E0
 */

#include "hardware.h"

/* Slot 0 means "no army", so it passes 0; otherwise it passes that army's
 * palette index, one less than the stored unk1a.
 *
 * `lsls #4; subs; lsls #2` is the * 0x3c stride of struct PlayerStruct, which is
 * the size the header already records -- so this is plain array indexing off
 * the dereferenced pointer, not a hand-built offset. */
void ApplyArmyWindowFramePalette(u16 a1)
{
    if (a1 == 0)
        ApplyWindowFramePalette(0, 8);
    else
        ApplyWindowFramePalette(gPlayers[a1].teamColor - 1, 8);
}
asm(".global sub_0801A548\n.thumb_set sub_0801A548, ApplyArmyWindowFramePalette\n");

/* Loads army a1's palette into OBJ palette slot 0x160. gUnknown_0810E6E0 is
 * declared `u8 []`, so `(unk1a + 4) * 0x20` is already the byte offset and the
 * ROM's single `lsls #5` falls out directly; the cast to ApplyPaletteExt's
 * `u16 *` is byte-neutral. 0x160 is built `movs #0xb0; lsls #1`. */
void LoadArmyObjPalette(u16 a1)
{
    ApplyPaletteExt((u16 *)(gUnknown_0810E6E0 + (gPlayers[a1].teamColor + 4) * 0x20),
                    0x160, 0x20);
}
asm(".global sub_0801A57C\n.thumb_set sub_0801A57C, LoadArmyObjPalette\n");

/* `lsls #0x1c; lsrs #0x1e` off a whole-word `ldr` of the BG1 control shadow is
 * extract_bit_field on bits 2..3, i.e. struct BgCnt's `chr_block` -- and
 * hardware.h already records that a bitfield read emits `ldr` plus a shift pair
 * whatever the container width, so the word load says nothing against the u16
 * union. Scaling it by 0x4000 is the character-block stride, which puts the
 * destination at 0x06006C00 inside whichever block BG1 is using. */
void LoadBg1WindowFrame(u16 a1)
{
    DecompressWindowFrameTiles((void *)(gUnknown_030030B4.bits.chr_block * 0x4000 + 0x06006C00));
    ApplyArmyWindowFramePalette(a1);
}
asm(".global sub_0801A5B0\n.thumb_set sub_0801A5B0, LoadBg1WindowFrame\n");

/* The same BG1 chr_block address LoadBg1WindowFrame computes, handed to the clearing
 * variant instead. */
void LoadBg1WindowFrameDefaultPalette(void)
{
    LoadWindowFrameGraphics((void *)(gUnknown_030030B4.bits.chr_block * 0x4000 + 0x06006C00), 0, 8);
}
asm(".global sub_0801A5E0\n.thumb_set sub_0801A5E0, LoadBg1WindowFrameDefaultPalette\n");
