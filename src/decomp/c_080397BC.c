#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080397BC.
 * sub_080397BC @ 0x080397BC, sub_080397CC @ 0x080397CC, sub_080397DC @ 0x080397DC, sub_080397F4 @ 0x080397F4, sub_08039820 @ 0x08039820
 */

#include "proc.h"
struct Unk397BCProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x54);
    /* 0x54 */ int unk54;
    /* 0x58 */ int unk58;
};
struct Unk397CCProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x54);
    /* 0x54 */ int unk54;
};

/* The proc is ActivateCoPower's third argument. `adds r2, r0, #0` BEFORE either
 * load is the whole evidence: r2 is the third argument register, and a
 * two-argument call keeps the base in r0 and moves the first argument in last
 * instead. */

void CoPowerSequence_Activate(struct Unk397BCProc *proc)
{
    ActivateCoPower(proc->unk54, proc->unk58, proc);
}
asm(".global sub_080397BC\n.thumb_set sub_080397BC, CoPowerSequence_Activate\n");

/* No `adds rN, r0, #0` here, unlike its two neighbours: the base stays in r0
 * to the end, so this call really does take one argument. The
 * `lsls #0x18; lsrs #0x18` is sub_08043DAC's declared u8 parameter narrowing
 * the int field. */

void CoPowerSequence_PlayMusic(struct Unk397CCProc *proc)
{
    sub_08043DAC(proc->unk54);
}
asm(".global sub_080397CC\n.thumb_set sub_080397CC, CoPowerSequence_PlayMusic\n");

/* Two statements, not a nest: r0 is overwritten by the pool `ldr` between the
 * calls, so nothing survives from sub_08022A34. */

void sub_080397DC(void)
{
    sub_08022A34();
    sub_0801A5B0(gUnknown_030033EC);
}

/* The record for the current army -- gUnknown_030033EC indexes
 * gPlayers[] at stride 0x3c -- supplies sub_08019818's terrain byte.
 * The entry `lsls #0x10; lsrs #0x10` is the u16 parameter's own declaration:
 * sub_08019818's first parameter is u16 too, so nothing narrows it again. */

void sub_080397F4(u16 a)
{
    sub_08019818(a, gPlayers[gUnknown_030033EC].co, 0);
}

/* In mode 1 the scripted line (sub_08039850) is tried first and the random
 * line (ShowRandomCoPowerQuote) is the fallback; in every other mode the random line is
 * all there is. `lsls r0, r0, #0x18` before the `cmp` is sub_08039850's u8
 * return being re-narrowed at the call site. */

void sub_08039820(ProcPtr proc)
{
    if (gPlaySt.gameMode == 1)
    {
        if (sub_08039850(proc) == 0)
            ShowRandomCoPowerQuote(proc);
    }
    else
    {
        ShowRandomCoPowerQuote(proc);
    }
}
