#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080111C8.
 * sub_080111C8 @ 0x080111C8
 */

#include "proc.h"
/*
 * StartFadeScreenLines (below) -- start the screen-line fade process and give it its
 * parameters: two buffers, two 16-bit values and a function pointer.
 * gUnknown_03001FDC is 0 while the process is being set up and 1 once it is
 * ready.
 *
 * gUnknown_03001FDC is declared in this file on purpose, not in
 * include/unknown-globals.h. src/proc.c defines it as
 * `s32 IWRAM_DATA gUnknown_03001FDC;`, a tentative definition carrying a
 * section attribute; if a plain `extern` for it is seen first -- which is what
 * happens when the declaration sits in a header that src/proc.c includes -- the
 * attribute is dropped, the symbol moves out of IWRAM and the built ROM
 * changes. A declaration in this file is seen by nothing else.
 */
extern s32 gUnknown_03001FDC;
struct Unk80111C8Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ void *unk2c;
    /* 0x30 */ void *unk30;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 unk36;
    /* 0x38 */ void (*unk38)(void);
};

void StartFadeScreenLines(void *a, void *b, u16 c, u16 d, void (*e)(void))
{
    struct Unk80111C8Proc *proc;

    gUnknown_03001FDC = 0;
    proc = Proc_Start(ProcScr_FadeScreenLines, NULL);
    proc->unk2c = a;
    proc->unk30 = b;
    proc->unk34 = c;
    proc->unk36 = d;
    proc->unk38 = e;
    gUnknown_03001FDC = 1;
}
asm(".global sub_080111C8\n.thumb_set sub_080111C8, StartFadeScreenLines\n");
