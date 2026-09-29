#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E698.
 * sub_0806E698 @ 0x0806E698, sub_0806E6B4 @ 0x0806E6B4
 */

#include "proc.h"
/* `adds r1, r4, #0` names the parent directly: it is this starter's own first
 * parameter. That parameter is also dereferenced at +0x5c, so the parent is a
 * proc of the same shape as the one being started. */
struct Unk6E698Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x5c);
    /* 5c */ s32 unk5c;
};

void SoundRoomCoSwap_StartSlideOut(struct Unk6E698Proc * parent)
{
    struct Unk6E698Proc * proc = Proc_StartBlocking(gUnknown_08582B74, parent);

    proc->unk5c = parent->unk5c;
}
asm(".global sub_0806E698\n.thumb_set sub_0806E698, SoundRoomCoSwap_StartSlideOut\n");

/* Byte-identical to StartSoundRoomCoSlideIn, which starts the same script. */
void SoundRoomCoSwap_StartSlideIn(ProcPtr parent)
{
    Proc_StartBlocking(gUnknown_08582B94, parent);
}
asm(".global sub_0806E6B4\n.thumb_set sub_0806E6B4, SoundRoomCoSwap_StartSlideIn\n");
