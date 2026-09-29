#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067B6C.
 * sub_08067B6C @ 0x08067B6C, sub_08067B90 @ 0x08067B90
 */

#include "proc.h"
struct Unk67B90Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
};

/* Clears the 0x1000-byte tilemap at 0x0600E000 with a BIOS fast FILL: bit 24
 * of 0x01000400 is the fill flag and the low bits the word count, so the
 * source is a single stack word rather than a buffer. Same idiom as
 * src/decomp/c_080688E4.c, down to the `u32` local.
 *
 * The local is written AFTER the SetDefaultColorEffects call, which is the source order
 * -- agbcc had no reason to sink a constant store past a call otherwise. */
void IntroSlidePanel_Cleanup(void)
{
    u32 zero;

    SetDefaultColorEffects();
    zero = 0;
    CpuFastSet(&zero, (void *)0x0600E000, 0x01000400);
}
asm(".global sub_08067B6C\n.thumb_set sub_08067B6C, IntroSlidePanel_Cleanup\n");

/* Loads one variant's tiles and tilemap. Both tables are indexed by the same
 * proc field, which is re-`ldr`ed for the second lookup because Decompress
 * clobbers memory; the two `lsls #2` are therefore not a missed CSE.
 *
 * The destinations are spelled differently on purpose: the first is a raw
 * VRAM address the ROM stores as a bare `.word 0x06001400` with no relocation,
 * so `(void *)0x06001400` per the c_080116E8.c/c_0803A174.c precedent, while
 * the second goes through the gBG1TilemapBuffer tilemap POINTER and is a real
 * relocation plus a deref. */
void IntroSlidePanel_LoadGraphics(struct Unk67B90Proc *proc)
{
    Decompress(gUnknown_08581050[proc->unk2c], (void *)0x06001400);
    Decompress(gUnknown_0858105C[proc->unk2c], gBG1TilemapBuffer);
    BG_EnableSyncBG1();
}
asm(".global sub_08067B90\n.thumb_set sub_08067B90, IntroSlidePanel_LoadGraphics\n");
