#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035FA8.
 * sub_08035FA8 @ 0x08035FA8
 */

#include "proc.h"
/* Third reader of the gUnknown_0849CD88 row keyed by the proc's unk36, after
 * StartMoveSlideMoveSound and PlayMoveSlideMoveSfx. Dispatches on the row's unk1e sentinel:
 * 0 and 0x8000 do nothing, 0x4000 installs the row's unk20 pointer into the
 * gUnknown_0849BDE8 entry's unk18, and anything else is a help id.
 *
 * `cmp #0x4000; beq; bgt; cmp #0; beq; b` is gcc's three-case dispatch TREE,
 * not a chain of ifs.
 *
 * THE TWO EMPTY CASES MUST SHARE ONE LABEL AND BE WRITTEN FIRST -- that is 3
 * bytes and it was measured this wave. `case 0: case 0x8000: break;` as the
 * first arm emits the 0x8000 leaf as `bne default; b break`, which is the ROM;
 * writing them as two separate empty arms, or moving either after the 0x4000
 * arm, flips that leaf to `beq break; b default`. Same size, different bytes.
 *
 * The (int) cast on unk20 is the only wart: Unk0849CD88.unk20 is `void *`
 * (wave 36) and Unk03001470.unk18 is `int`, and both are independently
 * documented as "very likely an address". The store is a whole word either
 * way, so the cast is byte-neutral and does not settle which is right. */
struct Unk35FA8Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x36);
    /* 0x36 */ u8 unk36;
};

void PlayMoveSlideStopSfxNoStep(ProcPtr procArg)
{
    struct Unk35FA8Proc *proc = procArg;

    switch (gUnknown_0849CD88[proc->unk36].unk1e)
    {
    case 0:
    case 0x8000:
        break;

    case 0x4000:
        sub_080152EC(gUnknown_0849BDE8, 0)->unk18
            = (int)gUnknown_0849CD88[proc->unk36].unk20;
        break;

    default:
        PlayMusicOrSfx(gUnknown_0849CD88[proc->unk36].unk1e);
        break;
    }
}
asm(".global sub_08035FA8\n.thumb_set sub_08035FA8, PlayMoveSlideStopSfxNoStep\n");
