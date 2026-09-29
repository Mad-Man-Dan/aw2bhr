#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080110EC.
 * sub_080110EC @ 0x080110EC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
struct Unk80110ECProc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
};

/* sub_08011054's fade-down twin: same body with the flag sense inverted and
 * the two seeds 0x10 / 0x1000 instead of 0 / 0. */
void FadeFromBlack_OnInit(struct Unk80110ECProc *proc)
{
    if (gUnknown_03002B5C == 0)
    {
        Proc_End(proc);
        return;
    }

    gUnknown_03002B5C = 0;
    InitFadeBlend();
    gUnknown_030030E0.bits.effect = 3;
    gUnknown_03001FFC = 0x10;
    proc->unk64 = 0x100;
    proc->unk66 = 0x1000;
}

asm(".global sub_080110EC\n.thumb_set sub_080110EC, FadeFromBlack_OnInit\n");

extern void FadeFromCommon_OnLoop(void);
extern void FadeScreenLines_CB_080111AD(void);
extern void FadeScreenLines_IDLE_0801117D(void);
extern void WipeToBlack_Init(void);
extern void WipeToBlack_Loop(void);
extern void Wipe_End(void);
extern void WipeFromBlack_Init(void);
extern void WipeFromBlack_Loop(void);

struct ProcCmd CONST_DATA ProcScr_DesignRoomFadeIn[] =
{
    PROC_CALL(FadeFromBlack_OnInit),
    PROC_YIELD,
    PROC_REPEAT(FadeFromCommon_OnLoop),
    PROC_END,
};

struct ProcCmd CONST_DATA ProcScr_FadeScreenLines[] =
{
    PROC_2A,
    PROC_ONEND(FadeScreenLines_CB_080111AD),
    PROC_REPEAT(FadeScreenLines_IDLE_0801117D),
    PROC_END,
};

struct ProcCmd CONST_DATA ProcScr_SomeFade[] =
{
    PROC_CALL(WipeToBlack_Init),
    PROC_SLEEP(1),
    PROC_REPEAT(WipeToBlack_Loop),
    PROC_REPEAT(Wipe_End),
    PROC_END,
};

struct ProcCmd CONST_DATA ProcScr_FadeLoadMap[] =
{
    PROC_CALL(WipeFromBlack_Init),
    PROC_SLEEP(1),
    PROC_REPEAT(WipeFromBlack_Loop),
    PROC_REPEAT(Wipe_End),
    PROC_END,
};

asm(".global ProcScr_FadeScreenRelated\n.set ProcScr_FadeScreenRelated, ProcScr_FadeLoadMap\n"
    ".global gUnknown_0848925C\n.set gUnknown_0848925C, ProcScr_DesignRoomFadeIn\n"
    ".global gUnknown_0848927C\n.set gUnknown_0848927C, ProcScr_FadeScreenLines\n"
    ".global gUnknown_0848929C\n.set gUnknown_0848929C, ProcScr_SomeFade\n"
    ".global gUnknown_084892C4\n.set gUnknown_084892C4, ProcScr_FadeLoadMap\n");
