#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808789C.
 * sub_0808789C @ 0x0808789C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Family F025: `push {lr}; ldr r0,[r0,#0x54]; bl f; pop {r0}; bx r0`.
 *
 * The strongest-evidenced member of the family, because the loop closes: the
 * ROM holds `PROC_CALL(PutEnemyCoMinimug_0808789D)` at 0x08616DA0, which is INSIDE the
 * script ProcScr_PutEnemyCoMinimug -- and sub_08087B74, the callee here, is the
 * function that does `Proc_Find(ProcScr_PutEnemyCoMinimug)` and `str r5,[r0,#0x54]`.
 * So +0x54 of this proc is written by the callee and read back by this
 * wrapper, and it holds sub_08087B74's own `int` parameter (used there as an
 * offset into gUnknown_02027F74 + 4, not as a pointer -- hence `int` and not
 * `void *`). */

struct Unk0808789CProc
{
    PROC_HEADER;
    STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
};

void PutEnemyCoMinimug_0808789D(struct Unk0808789CProc *proc)
{
    sub_08087B74(proc->unk54);
}

asm(".global sub_0808789C\n.thumb_set sub_0808789C, PutEnemyCoMinimug_0808789D\n");

extern void PutEnemyCoMinimug_IDLE_080878A9(void);

struct ProcCmd CONST_DATA ProcScr_PutEnemyCoMinimug[] =
{
    PROC_YIELD,
    PROC_CALL(PutEnemyCoMinimug_0808789D),
    PROC_REPEAT(PutEnemyCoMinimug_IDLE_080878A9),
    PROC_END,
};

asm(".global gUnknown_08616D94\n.set gUnknown_08616D94, ProcScr_PutEnemyCoMinimug\n");
