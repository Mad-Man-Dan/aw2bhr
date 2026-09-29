#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080733A0.
 * sub_080733A0 @ 0x080733A0
 */

#include "proc.h"
struct Unk80733A0
{
    /* 0x00 */ u8 filler_00[0x68];
    /* 0x68 */ u8 unk_68;
};

void SetHeaderBannerTransitionFrames(int arg)
{
    struct Unk80733A0 * proc = Proc_Find(ProcScr_MainMenuPutSelectModeSprite);

    proc->unk_68 = arg;
}
asm(".global sub_080733A0\n.thumb_set sub_080733A0, SetHeaderBannerTransitionFrames\n");
