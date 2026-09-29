#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067ED0.
 * sub_08067ED0 @ 0x08067ED0
 */

#include "proc.h"
/* NINE parameters -- four in registers and five at [sp,#0x20] through
 * [sp,#0x30] over a 32-byte prologue -- of which four are narrow: a1, a7 and
 * a8 are `u8` and a2 is `u16`, all four narrowed in the prologue with
 * PROMOTE_MODE pairs before the first call.
 *
 * The last is Proc_Start's parent. The proc's +0x2a and +0x29 are written from
 * ONE address (`adds r0,#0x2a; strb; subs r0,#1; strb`), which is the walking
 * form two byte members at adjacent offsets always take. */
struct Unk08581108Proc
{
    /* 0x00 */ u8 filler_00[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 filler_2b[0x01];
    /* 0x2c */ s32 unk2c;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3c */ u8 unk3c;
    /* 0x3d */ u8 unk3d;
    /* 0x3e */ u8 filler_3e[0x02];
    /* 0x40 */ s32 unk40;
};

void StartIntroCoSlide(u8 a1, u16 a2, int a3, int a4, int a5, int a6, u8 a7, u8 a8,
                  ProcPtr parent)
{
    struct Unk08581108Proc *p;

    LoadCoFullBodyPart0(a2, gUnknown_08581104[a1]);
    LoadCoPalette(a2, 0x12);

    p = Proc_Start(gUnknown_08581108, parent);

    p->unk2a = a1;
    p->unk29 = a2;
    p->unk2c = a3;
    p->unk30 = a4;
    p->unk34 = a5;
    p->unk38 = a6;
    p->unk3c = a7;
    p->unk3d = a8;
    p->unk40 = 0;
}
asm(".global sub_08067ED0\n.thumb_set sub_08067ED0, StartIntroCoSlide\n");
