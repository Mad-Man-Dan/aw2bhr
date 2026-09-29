#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080719D4.
 * sub_080719D4 @ 0x080719D4
 */

/* Zeroes two words and seeds +0x64 with GetMapLock()'s frame snapshot --
 * the same pairing the gUnknown_08614014 note in include/unknown-globals.h
 * records for StartPartialGameLock, which stashes the identical value at the
 * identical offset. `adds r4,#0x64` before the `strh` is forced: 0x64 is past
 * the 6-bit halfword displacement, so the base has to move.
 *
 * unk64 is `s16` by inheritance from that note, not proved here -- a `u16`
 * field is byte-identical for a bare store of an `int` return. */
struct Unk80719D4
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 filler_60[0x04];
    /* 0x64 */ s16 unk64;
};

void DigitPicker_Init(struct Unk80719D4 *proc)
{
    proc->unk58 = 0;
    proc->unk5c = 0;
    proc->unk64 = GetMapLock();
}
asm(".global sub_080719D4\n.thumb_set sub_080719D4, DigitPicker_Init\n");
