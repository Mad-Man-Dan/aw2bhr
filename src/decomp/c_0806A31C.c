#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A31C.
 * sub_0806A31C @ 0x0806A31C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
/* A 0x58-tick cutscene script driven off `0x58 - unk2c`, so the switch counts
 * UP while the field counts down. Four beats, each starting a text/portrait
 * pair (StartIntroCoSlide) and its graphics proc (StartIntroCoNameWobble); the first beat is
 * the only one that does not clear the previous 0x08581108 procs first.
 *
 * The tail is the usual `tick or break`, but the break arm also clears
 * 0x08581108 -- that call is NOT part of the switch even though three arms
 * make it too. */
struct Unk6A31CProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void IntroT3_IDLE_0806A31D(struct Unk6A31CProc *proc)
{
    switch (0x58 - proc->unk2c)
    {
    case 0:
        StartIntroCoSlide(0, 0xb, 0x3c, 0x46, 0x15, 0, 0, 1, proc);
        StartIntroCoNameWobble(0xb, 0x68, 0x78, 0x15, 0x280, 3, proc);
        break;
    case 0x16:
        Proc_EndEach(gUnknown_08581108);
        StartIntroCoSlide(1, 0xc, 0xb4, 0xaa, 0x15, 0, 1, 1, proc);
        StartIntroCoNameWobble(0xc, 8, 0x78, 0x15, 0x280, 3, proc);
        break;
    case 0x2c:
        Proc_EndEach(gUnknown_08581108);
        StartIntroCoSlide(0, 0xd, 0x3c, 0x46, 0x15, 0, 0, 1, proc);
        StartIntroCoNameWobble(0xd, 0x48, 0x78, 0x15, 0x280, 3, proc);
        break;
    case 0x42:
        Proc_EndEach(gUnknown_08581108);
        StartIntroCoSlide(1, 0xe, 0xb4, 0xaa, 0x15, 0, 1, 1, proc);
        StartIntroCoNameWobble(0xe, 8, 0x78, 0x15, 0x280, 3, proc);
        break;
    }

    if (proc->unk2c != 0)
    {
        proc->unk2c--;
    }
    else
    {
        Proc_EndEach(gUnknown_08581108);
        Proc_Break(proc);
    }
}

asm(".global sub_0806A31C\n.thumb_set sub_0806A31C, IntroT3_IDLE_0806A31D\n");
