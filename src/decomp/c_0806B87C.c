#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B87C.
 * sub_0806B87C @ 0x0806B87C, sub_0806B910 @ 0x0806B910, sub_0806B980 @ 0x0806B980
 */

#include "hardware.h"
#include "proc.h"
/* Starts the alpha-fade child and seeds it from the parent's +0x2c parity: the
 * two ternaries are one test each, both reloading +0x2c rather than sharing it.
 * The middle four statements are the same fade preamble as
 * src/decomp/c_0806B828.c, including `unk58 >> 3` computed once and reused for
 * the 0xe subtraction.
 *
 * The tail is the `.raw` half of gUnknown_030030E0, not `.bits`: the two target
 * GROUPS go through one HImode read-modify-write with pool masks
 * (`ldrh; ldr =0xFFE0; ands; orrs; ldr =0xE0FF; ands; orrs; strh`), which five
 * 1-bit field writes cannot produce -- a 5-bit group at bit 0 sits inside byte
 * 0, so get_best_mode would pick QImode and emit `ldrb`/`strb`. `effect` a few
 * lines above IS `.bits`, and is `ldrb` accordingly. This is the same split
 * documented for the 0806717C/08067300/0806A4DC family in hardware.h, and this
 * function is another member of it. The two raw writes merge into a single
 * ldrh/strh pair because nothing between them touches this object. */
struct Unk6B87CParent
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u16 unk2c;
};
struct Unk6B87CChild
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};
/* A four-case timeline on +0x30. The `(u8)` cast on StartCreditsIllustration's result is
 * what produces the `lsl #0x18` before the test: the shared declaration says
 * `int`, and its only other caller discards the value, so the cast lives here
 * rather than in the header.
 *
 * +0x30 is UNSIGNED. Its width and its use are identical either way, but the
 * switch's binary-search tree branches with `bhi`, and a signed control
 * expression emits `bgt` there -- one byte, and the only thing in this function
 * that discriminates the two.
 *
 * The 0x1e0 arm and the fall-through share one `str` to +0x30 because the
 * compiler cross-jumps them; only the two Proc_Break paths skip the store. */
struct Unk6B910Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u8 filler_2c[0x04];
    /* 0x30 */ u32 unk30;
};

void StartCreditsFadeProc(ProcPtr parent)
{
    struct Unk6B87CChild *proc = Proc_Start(gUnknown_085819C4, parent);

    proc->unk58 = (((struct Unk6B87CParent *)parent)->unk2c & 1) ? 0 : 0x70;
    proc->unk5c = (((struct Unk6B87CParent *)parent)->unk2c & 1) ? 1 : -1;
    proc->unk60 = 0;

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = proc->unk58 >> 3;
    gUnknown_03002B28 = 0xe - (proc->unk58 >> 3);
    gUnknown_03001FFC = 0;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xffe0) | 2;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xe0ff) | 0x800;
}
asm(".global sub_0806B87C\n.thumb_set sub_0806B87C, StartCreditsFadeProc\n");

void CreditsMissionList_Loop(struct Unk6B910Proc *proc)
{
    switch (proc->unk30)
    {
    case 0:
        StartCreditsMissionLine(proc);
        break;

    case 0x1d0:
        if ((u8)StartCreditsIllustration(proc) == 0)
        {
            Proc_Break(proc);
            return;
        }
        break;

    case 0x1d8:
        if (gUnknown_0202F214[proc->unk2a].unk00 == 0)
        {
            Proc_Break(proc);
            return;
        }
        StartCreditsFadeProc(proc);
        break;

    case 0x1e0:
        proc->unk30 = 0;
        return;
    }

    proc->unk30++;
}
asm(".global sub_0806B910\n.thumb_set sub_0806B910, CreditsMissionList_Loop\n");

/* Two independent tests on the same word: the `ldr` is issued once and the
 * address stays in r2 across both, which is why the second test reloads the
 * value but not the address. */
void CreditsBgScroll_Loop(void)
{
    if (gGameClock & 1)
    {
        gUnknown_03002F18++;
        gUnknown_03001400--;
        gUnknown_0300200C++;
    }

    if ((gGameClock & 3) == 0)
        gUnknown_03002000--;
}
asm(".global sub_0806B980\n.thumb_set sub_0806B980, CreditsBgScroll_Loop\n");
