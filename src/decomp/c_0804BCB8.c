#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804BCB8.
 * sub_0804BCB8 @ 0x0804BCB8
 */

/* Builds a SetFigureSlide motion request on the stack for one (group, slot) and
 * submits it, but only for the side currently on screen. Under the bit0 /
 * !bit6 combination -- the same guard c_0804E584.c uses two functions away --
 * the step and the frame count are both dropped to zero, i.e. the motion is
 * applied instantly.
 *
 * The record is a `struct Unk56E28` local, which is what produces the broken
 * address chain (`mov r0, sp; strh; adds r0, #2; strh; add r0, sp, #4; ...`):
 * taking the struct's address forces the stores through a register base rather
 * than sp displacements. Three of its seven members are literal zeros sharing
 * one `movs r1, #0`. */
void StartFigureEntrySlide(u16 a, u16 b, s16 c, u16 d)
{
    struct Unk56E28 r;

    if (a == gUnknown_0300450C)
    {
        if (gUnknown_03004504.bit0)
        {
            if (!gUnknown_03004504.bit6)
            {
                c = 0;
                d = 0;
            }
        }

        r.unk00 = a;
        r.unk02 = b;
        r.unk04 = c;
        r.unk06 = 0;
        r.unk08 = 0;
        r.unk0a = 0;
        r.unk0c = d;
        SetFigureSlide(&r);
    }
}
asm(".global sub_0804BCB8\n.thumb_set sub_0804BCB8, StartFigureEntrySlide\n");
