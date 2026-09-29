#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806974C.
 * sub_0806974C @ 0x0806974C
 */

#include "proc.h"
struct Unk0806974CProc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ s16 unk2a[3];
    /* 0x30 */ s16 unk30[3];
    /* 0x36 */ u8 unk36;
};

void StartIntroParallaxScroll(ProcPtr parent)
{
    struct Unk0806974CProc *proc = Proc_Start(gUnknown_08581420, parent);
    int i;

    for (i = 0; i <= 2; i++) {
        proc->unk2a[i] = gUnknown_08581430[i][0];
        proc->unk30[i] = gUnknown_08581430[i][1];
    }

    proc->unk36 = 1;
}
asm(".global sub_0806974C\n.thumb_set sub_0806974C, StartIntroParallaxScroll\n");
