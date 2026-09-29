#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039674.
 * sub_08039674 @ 0x08039674, sub_080396F4 @ 0x080396F4
 */

#include "proc.h"
#include "hardware.h"
/* Starts the gUnknown_0849D6D4 script as a child, seeds both it and the
 * gUnknown_0849D7FC script with the same terrain byte, then arms the blend and
 * the fade counters.
 *
 * gPlayers[i].unk1d is read TWICE -- the ROM CSEs the ELEMENT ADDRESS
 * into r1 and issues two `ldrb r2,[r1,#0x1d]`, which is two source reads of one
 * member, not one read held in a register.
 *
 * The single `movs r6,#0` serves both `proc->unk64 = 0` and
 * `gUnknown_03002020 = 0`, which is what puts the zero in a callee-saved
 * register; the two stores are far apart but the literal is written twice.
 *
 * The two blend bytes are one `ldrb`/`strb` pair each because agbcc merges
 * adjacent bitfield stores landing in the same byte: `ands #0x3f; orrs #0x40`
 * is `effect = 1` (a 2-bit field at bit 6, so the clear is part of it) and the
 * `orrs #1` beside it is target1_enable_bg0. */
struct Unk39674Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x54);
    /* 0x54 */ int unk54;
    /* 0x58 */ STRUCT_PAD(0x58, 0x64);
    /* 0x64 */ s16 unk64;
};
struct Unk396F4Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x54);
    /* 0x54 */ int unk54;
    /* 0x58 */ STRUCT_PAD(0x58, 0x64);
    /* 0x64 */ s16 unk64;
};

void CoPowerIntro_Init(struct Unk39674Proc *proc)
{
    struct Unk39674Proc *p;
    int i;

    i = proc->unk54;
    proc->unk64 = 0;

    p = Proc_Start(gUnknown_0849D6D4, proc);
    p->unk54 = gPlayers[i].co;

    StartCoPowerPanel(gPlayers[i].co, proc);
    SetMapLayersDefault();

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg0 = 1;
    gUnknown_030030E0.bits.target2_enable_bg1 = 1;
    gUnknown_030030E0.bits.target2_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;

    PlayMusic(0xd0);
}
asm(".global sub_08039674\n.thumb_set sub_08039674, CoPowerIntro_Init\n");

void CoPowerIntro_FadeInLoop(struct Unk396F4Proc *proc)
{
    if (proc->unk64++ > 2)
    {
        gUnknown_03002020++;
        gUnknown_03002B28--;
        proc->unk64 = 0;
    }

    if (gUnknown_03002020 == 8)
    {
        Proc_Break(proc);
        StartCoPowerNameBanner(GetArmyCoPowerName(proc->unk54));
        Proc_BreakEach(gUnknown_0849D6D4);
    }
}
asm(".global sub_080396F4\n.thumb_set sub_080396F4, CoPowerIntro_FadeInLoop\n");
