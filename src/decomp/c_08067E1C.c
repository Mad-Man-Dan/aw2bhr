#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067E1C.
 * sub_08067E1C @ 0x08067E1C
 */

#include "proc.h"
/* One frame of a sprite tween: the x comes from Interpolate clipped to 9 bits,
 * the OAM attribute word is assembled from a tile id out of
 * gUnknown_08581104[], the 0x2000 256-colour bit and a palette shifted up by
 * 10, and the whole thing goes to sub_08043C28. Breaks on the last frame.
 *
 * `movs r1,#0x94; lsls r1,#3` is the literal 0x4a0 and `movs r5,#0x80;
 * lsls r5,#6` the literal 0x2000. */
struct Unk08067E1CProc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 filler_2b[0x01];
    /* 0x2c */ s32 unk2c;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3c */ u8 unk3c;
    /* 0x3d */ u8 filler_3d[0x03];
    /* 0x40 */ s32 unk40;
};

void IntroCoSlide_MoveLoop(struct Unk08067E1CProc *proc)
{
    int v;

    v = Interpolate(0, proc->unk2c, proc->unk30, proc->unk40, proc->unk34);

    sub_08043C28(v & 0x1ff, 0x4a0,
                 (0x2000 | gUnknown_08581104[proc->unk2a]) | (proc->unk38 << 10),
                 0, proc->unk3c);

    proc->unk40++;

    if (proc->unk40 == proc->unk34)
        Proc_Break(proc);
}
asm(".global sub_08067E1C\n.thumb_set sub_08067E1C, IntroCoSlide_MoveLoop\n");
