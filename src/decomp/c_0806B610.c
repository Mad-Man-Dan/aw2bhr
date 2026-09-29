#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B610.
 * sub_0806B610 @ 0x0806B610
 */

#include "proc.h"
/* A 0x3c0-frame wait counted at +0x58, ticking a counter every fourth frame and
 * clearing the tile buffer on the way out. The bound test reads the value the
 * `str` just wrote rather than reloading it. */
struct Unk6B610Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void CreditsEpilogue_ScrollBg(struct Unk6B610Proc *proc)
{
    int zero;

    if ((proc->unk58 & 3) == 0)
        gUnknown_03001418++;

    proc->unk58++;

    if (proc->unk58 > 0x3bf)
    {
        zero = 0;
        CpuFastSet(&zero, gBG0TilemapBuffer, 0x01000200);
        BG_EnableSyncBG0();
        Proc_Break(proc);
    }
}
asm(".global sub_0806B610\n.thumb_set sub_0806B610, CreditsEpilogue_ScrollBg\n");
