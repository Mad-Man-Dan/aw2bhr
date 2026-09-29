#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F27C.
 * sub_0806F27C @ 0x0806F27C
 */

#include "proc.h"
struct Unk0806F27CProc
{
    /* 0x00 */ u8 filler_00[0x3c];
    /* 0x3c */ ProcPtr unk3c[6];
};

void SoundRoomMusicPage_End(struct Unk0806F27CProc *proc)
{
    int i;

    sub_080733B8();
    Proc_EndEach(gUnknown_08582B2C);
    Proc_EndEach(gUnknown_08582C24);

    for (i = 0; i <= 5; i++)
        Proc_End(proc->unk3c[i]);

    Proc_EndEach(gUnknown_08582BE4);
    EndBgScrollAnimator();
}
asm(".global sub_0806F27C\n.thumb_set sub_0806F27C, SoundRoomMusicPage_End\n");
