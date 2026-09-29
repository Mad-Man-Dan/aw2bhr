#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C1C8.
 * sub_0806C1C8 @ 0x0806C1C8, sub_0806C1E4 @ 0x0806C1E4
 */

#include "proc.h"
struct Unk6C1C8Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u32 unk2c;
    /* 0x30 */ u8 filler_30[0x24];
    /* 0x54 */ u16 *unk54;
};
#include "hardware.h"

/* Two of PutSprite's five arguments come out of the proc and the other three
 * are literals; the `movs r0, #0` feeding the stack slot is materialised before
 * the `movs r0, #1` that overwrites it, which is the ordinary
 * stack-argument-first sequence and not evidence of an extra parameter. */
void CreditsResultSprite_DrawLoop(struct Unk6C1C8Proc *proc)
{
    PutSprite(1, proc->unk2c, 0, proc->unk54, 0);
}
asm(".global sub_0806C1C8\n.thumb_set sub_0806C1C8, CreditsResultSprite_DrawLoop\n");

/* The twin of src/decomp/c_0806E740.c's SoundRoomCycleArrowPalette, on a different table and
 * a different gPal slot -- but it needs a SECOND local, and that is the whole
 * difference between the two.
 *
 * `lsrs #1; lsls #1` is not a mask clearing bit 0: counted as
 * `(u32)x >> 1 << 1` it is the /2 fused with the u16 array's own scale, i.e.
 * the byte offset of element x/2. The `(u32)` cast is load-bearing --
 * gGameClock is `s32` and without it the divide is `asrs`.
 *
 * The literal pool is ordered (gUnknown_081A3D84, gGameClock, gPal), and
 * because agbcc's expand_assignment computes the DESTINATION address first, the
 * one-statement spelling emits gPal FIRST. Binding the table base moves that
 * reference into an earlier statement (SoundRoomCycleArrowPalette's fix, which lands gPal in
 * the middle); binding the INDEX as well pushes gPal to last AND moves the
 * `lsrs` ahead of gPal's pool load, which is the only arrangement that
 * reproduces this function. Both locals are required; either one alone is a
 * near miss of one instruction's position. */
void CreditsRank_CycleBlinkPalette(void)
{
    const u16 *tbl = gUnknown_081A3D84;
    u32 i = ((u32)gGameClock & 0x1F) / 2;

    gPal[0x12C] = tbl[i];
    EnablePaletteSync();
}
asm(".global sub_0806C1E4\n.thumb_set sub_0806C1E4, CreditsRank_CycleBlinkPalette\n");
