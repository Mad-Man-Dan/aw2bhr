#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080688A8.
 * sub_080688A8 @ 0x080688A8, sub_080688C8 @ 0x080688C8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* The opening line of src/decomp/c_080688E4.c on its own:
 * proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++). The post-increment is on a
 * u8 global, so the lsls #0x18; lsrs #0x18 after the strb is the argument's
 * own truncation and not a cast at the call. */
struct Unk688A8Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2C */ int unk2c;
};
#include "proc.h"
/* The countdown half of the pair IntroT3_080688A9 arms: tick unk2c down and break
 * once it reaches 0. int from the ldr/str, matching IntroT3_080688A9's own store
 * of GetIntroSceneDuration's int result.
 *
 * The unconditional b over the Proc_Break arm is the if/else -- the ROM tests
 * != 0 and falls into the decrement, so the break is the else. */
struct Unk688C8Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2C */ int unk2c;
};

void IntroT3_080688A9(struct Unk688A8Proc *proc)
{
    proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++);
}

void IntroT3_IDLE_080688C9(struct Unk688C8Proc *proc)
{
    if (proc->unk2c != 0)
        proc->unk2c--;
    else
        Proc_Break(proc);
}

asm(".global sub_080688A8\n.thumb_set sub_080688A8, IntroT3_080688A9\n"
    ".global sub_080688C8\n.thumb_set sub_080688C8, IntroT3_IDLE_080688C9\n");

extern void IntroT3_08068AC5(void);
extern void IntroT3_IDLE_08068BE5(void);
extern void IntroT3_080688E5(void);
extern void IntroT3_IDLE_08068A01(void);
extern void IntroT3_08068E61(void);
extern void IntroT3_IDLE_08069045(void);
extern void IntroT3_08069155(void);
extern void IntroT3_IDLE_080691BD(void);
extern void IntroT3_0806938D(void);
extern void IntroT3_IDLE_080694ED(void);
extern void IntroT3_08069865(void);
extern void IntroT3_IDLE_080699E9(void);
extern void IntroT3_08069B6D(void);
extern void IntroT3_IDLE_08069B91(void);
extern void IntroT3_0806A055(void);
extern void IntroT3_IDLE_0806A219(void);
extern void IntroT3_0806A2F9(void);
extern void IntroT3_IDLE_0806A31D(void);
extern void IntroT3_0806A445(void);

struct ProcCmd CONST_DATA ProcScr_IntroT3[] =
{
    PROC_START_BUGGED(ProcScr_IntroT0, 5),
    PROC_YIELD,
    PROC_CALL(IntroT3_080688A9),
    PROC_REPEAT(IntroT3_IDLE_080688C9),
    PROC_CALL(IntroT3_08068AC5),
    PROC_REPEAT(IntroT3_IDLE_08068BE5),
    PROC_CALL(IntroT3_080688E5),
    PROC_REPEAT(IntroT3_IDLE_08068A01),
    PROC_CALL(IntroT3_08068E61),
    PROC_REPEAT(IntroT3_IDLE_08069045),
    PROC_CALL(IntroT3_08069155),
    PROC_REPEAT(IntroT3_IDLE_080691BD),
    PROC_CALL(IntroT3_0806938D),
    PROC_REPEAT(IntroT3_IDLE_080694ED),
    PROC_CALL(IntroT3_08069865),
    PROC_REPEAT(IntroT3_IDLE_080699E9),
    PROC_CALL(IntroT3_08069B6D),
    PROC_REPEAT(IntroT3_IDLE_08069B91),
    PROC_CALL(IntroT3_0806A055),
    PROC_REPEAT(IntroT3_IDLE_0806A219),
    PROC_CALL(IntroT3_0806A2F9),
    PROC_REPEAT(IntroT3_IDLE_0806A31D),
    PROC_CALL(IntroT3_0806A445),
    PROC_END,
};

asm(".global gUnknown_08581500\n.set gUnknown_08581500, ProcScr_IntroT3\n");
