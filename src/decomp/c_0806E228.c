#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E228.
 * sub_0806E228 @ 0x0806E228
 */

#include "proc.h"
/* Same script and same +0x58 = arg + 1 as StartSoundRoomGalleryNumber, which starts the proc
 * instead of finding it. */
struct Unk6E228Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ s32 unk58;
};

void sub_0806E228(s32 a)
{
    ((struct Unk6E228Proc *)Proc_Find(gUnknown_08582AF4))->unk58 = a + 1;
}
