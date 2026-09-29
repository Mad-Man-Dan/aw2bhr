#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E210.
 * sub_0806E210 @ 0x0806E210
 */

#include "proc.h"
/* r1 is never written before the `bl`, so the parent is passed straight
 * through from this function's own second parameter. */
struct UnkE210Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ u32 unk58;
};

void StartSoundRoomGalleryNumber(int a, ProcPtr parent)
{
    ((struct UnkE210Proc *)Proc_Start(gUnknown_08582AF4, parent))->unk58 = a + 1;
}
asm(".global sub_0806E210\n.thumb_set sub_0806E210, StartSoundRoomGalleryNumber\n");
