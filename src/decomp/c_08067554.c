#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067554.
 * sub_08067554 @ 0x08067554, sub_08067564 @ 0x08067564
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
struct Unk67554Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
    /* 60 */ int unk60;
    /* 64 */ u16 unk64;
};
#include "hardware.h"
struct Unk67564Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x60);
    /* 60 */ int unk60;
    /* 64 */ u16 unk64;
};

/* The init half of the IntroT0_IDLE_08067565 "wait 0x14 frames, then A/START" proc:
 * +0x60 is the countdown IntroT0_IDLE_08067565 decrements and +0x64 the result flag it
 * sets. Zero is computed once into r2 and reused for all three of its stores,
 * which is ordinary constant CSE and not a memset. +0x64 needs its own `adds`
 * because `strh`'s immediate form tops out at 62. */
void IntroT0_08067555(struct Unk67554Proc *proc)
{
    proc->unk58 = 0;
    proc->unk5c = 0;
    proc->unk60 = 0x14;
    proc->unk64 = 0;
}

/* "Hold the message up for +0x60 more frames, then let A or START dismiss it."
 * IntroT0_08067555 is the matching init and 0x14 the frame count it arms.
 *
 * Key mask 9 is A|START, read off `gpKeySt->pressed` (+0x04) and not `pressed`
 * (+0x08) -- the offset in the ROM is unambiguous even though a dismiss
 * usually reads the edge. gpKeySt is upstream's own symbol from hardware.h;
 * there is no gUnknown_ at 0x03002EE0 and one cannot be created.
 *
 * The unconditional `b` after the decrement is the if/else, not a `return`:
 * the else arm is the whole key test. */
void IntroT0_IDLE_08067565(struct Unk67564Proc *proc)
{
    if (proc->unk60 != 0)
    {
        proc->unk60--;
    }
    else if (gpKeySt->pressed & 9)
    {
        FadeOutMusicDefault();
        proc->unk64 = 1;
        Proc_Break(proc);
    }
}

asm(".global sub_08067554\n.thumb_set sub_08067554, IntroT0_08067555\n"
    ".global sub_08067564\n.thumb_set sub_08067564, IntroT0_IDLE_08067565\n");

extern void IntroT0_IDLE_080675A1(void);
extern void IntroT0_08067629(void);

struct ProcCmd CONST_DATA ProcScr_IntroT0[] =
{
    PROC_CALL(IntroT0_08067555),
    PROC_REPEAT(IntroT0_IDLE_08067565),
    PROC_REPEAT(IntroT0_IDLE_080675A1),
    PROC_SLEEP(36),
    PROC_LABEL(0),
    PROC_CALL(IntroT0_08067629),
    PROC_END,
};

asm(".global gUnknown_08580F24\n.set gUnknown_08580F24, ProcScr_IntroT0\n");
