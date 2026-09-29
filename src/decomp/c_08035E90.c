#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035E90.
 * sub_08035E90 @ 0x08035E90
 */

#include "proc.h"
/* Fires the help line for the proc's current mode (unk36, the same
 * gUnknown_0849CD88 row index StartMoveSlideMoveSound and PlayMoveSlideStopSfxNoStep use).
 *
 * The two `PlayMusicOrSfx(row->unk1a)` tails are written TWICE in the ROM, once
 * for case 0x18's fall-through and once for the default arm -- they are
 * separate blocks that cross-jumping could not merge because case 0x18 reaches
 * its copy from an inner `if`.  Same finding as src/decomp/c_08035F68.c.
 *
 * `case 2:` falls into `case 1:` -- the only difference is the +2 it adds to
 * the id, which is what the r5 that is otherwise 0 carries.
 *
 * gUnknown_0849CD88 is named honestly: agbcc force-addrs it into this unit's
 * own .rodata word, which is the ROM's `ldr r2,=<word>; ldr r1,[r2]` pair, and
 * the reload of that word in each arm is the ROM's too. */
struct Unk35E90Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x30);
    /* 0x30 */ struct Unit *unk30;
    /* 0x34 */ STRUCT_PAD(0x34, 0x36);
    /* 0x36 */ u8 unk36;
    /* 0x37 */ STRUCT_PAD(0x37, 0x39);
    /* 0x39 */ u8 unk39;
};

void PlayMoveSlideMoveSfx(ProcPtr procArg)
{
    struct Unk35E90Proc *proc = procArg;
    int a;
    int b;
    u16 t;

    a = 0;
    b = 0;

    if (gUnknown_0849CD88[proc->unk36].unk1e == 0x8000)
        a = proc->unk39 & 1;

    switch (proc->unk36)
    {
    case 2:
        b = 2;
    case 1:
        if (gUnknown_0849CD88[proc->unk36].unk1a != 0)
        {
            t = GetMoveSlideFootstepSfx(proc);

            if (t != 0)
                PlayMusicOrSfx(t + a + b);
        }
        break;

    case 0x18:
        if (proc->unk30 != NULL && (proc->unk30->flags & 0x20))
        {
            PlayMusicOrSfx(0x38);
            break;
        }

        if (gUnknown_0849CD88[proc->unk36].unk1a != 0)
            PlayMusicOrSfx(gUnknown_0849CD88[proc->unk36].unk1a);
        break;

    default:
        if (gUnknown_0849CD88[proc->unk36].unk1a != 0)
            PlayMusicOrSfx(gUnknown_0849CD88[proc->unk36].unk1a);
        break;
    }
}
asm(".global sub_08035E90\n.thumb_set sub_08035E90, PlayMoveSlideMoveSfx\n");
