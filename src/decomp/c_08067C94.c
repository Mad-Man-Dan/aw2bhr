#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067C94.
 * sub_08067C94 @ 0x08067C94
 */

#include "proc.h"
/* Slides a sprite in from one of two directions until the frame counter runs
 * out, then keeps drawing it at the position it stopped at. Both arms store
 * the interpolated x to unk2c and derive y from a SIGNED quarter of it.
 *
 * THE TWO ARMS ARE SPELLED DIFFERENTLY AND THAT IS THE READOUT. The first
 * binds the Interpolate result to a local -- `adds r1,r0,#0; str r1,[r4,#0x2c]`
 * keeps the value in r0 for the `- 0x80` and copies it for the store -- while
 * the second stores straight through and lets cse feed the divide from the
 * same register (`str r0,[r4,#0x2c]; cmp r0,#0`). Giving both arms the local
 * puts the copy in both and is two bytes over. */
struct Unk08067C94Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s32 unk2c;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3c */ s32 unk3c;
};

void IntroSlideSprite_Loop(struct Unk08067C94Proc *proc)
{
    int v;

    if (proc->unk38 > proc->unk3c)
    {
        if (proc->unk34 != 0)
        {
            v = Interpolate(4, 0xf0, 0, proc->unk3c, proc->unk38);
            proc->unk2c = v;
            proc->unk30 = (v - 0x80) / 4 + 0x58;
        }
        else
        {
            proc->unk2c = Interpolate(4, -0x80, 0x70, proc->unk3c, proc->unk38);
            proc->unk30 = 0x58 - proc->unk2c / 4;
        }

        proc->unk3c++;
    }

    PutSprite(0, proc->unk2c, proc->unk30, gUnknown_085810A8, 0);
}
asm(".global sub_08067C94\n.thumb_set sub_08067C94, IntroSlideSprite_Loop\n");
