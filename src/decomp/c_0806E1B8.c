#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E1B8.
 * sub_0806E1B8 @ 0x0806E1B8
 */

#include "proc.h"
/* Draws a label sprite and then the proc's counter as two decimal digits, tens
 * first, each digit's OAM script index being `digit * 2`.
 *
 * BOTH digits are named locals. With the tens digit left inline in the second
 * PutSprite's argument list the `* 2` shift is emitted BEFORE the OAM blob's
 * pool word instead of after it -- size-exact, four bytes different, and the
 * only thing separating the two spellings. */
struct Unk0806E1B8Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void SoundRoomGalleryNumber_Loop(struct Unk0806E1B8Proc *proc)
{
    int lo;
    int hi;

    PutSprite(0, 8, 0x10, gUnknown_0816E7F0, 0);

    lo = DivRem(proc->unk58, 10);
    hi = Div(proc->unk58, 10);

    PutSprite(0, 0x18, 8, gUnknown_0816E7F8, hi * 2);
    PutSprite(0, 0x20, 8, gUnknown_0816E7F8, lo * 2);
}
asm(".global sub_0806E1B8\n.thumb_set sub_0806E1B8, SoundRoomGalleryNumber_Loop\n");
