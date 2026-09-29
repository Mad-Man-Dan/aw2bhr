#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033EC8.
 * sub_08033EC8 @ 0x08033EC8, sub_08033F1C @ 0x08033F1C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* MATCHED byte-for-byte, wave 43 (W43-C), in four attempts.
 * (comment trimmed for this re-verification; see work/sub_08033EC8/sub_08033EC8.c) */

struct Unk33EC8Proc
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ u8 unk30[0x03];
    /* 0x33 */ u8 filler_33[0x04];
    /* 0x37 */ u8 unk37;
};
#include "proc.h"
/* MATCHED, wave 43 (W43-C), in two attempts.
 *
 * NEEDS ITS .rodata POOL WORD PLACED. Add to this function's
 * data/promoted.json entry:
 *     "rodata": ["0x08090D80"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py before building.
 * 0x08090D80 holds 0x0849BC30, i.e. &gUnknown_0849BC30 -- agbcc's own
 * -fforce-addr address constant. The honest spelling reproduces it here
 * because this loop body is under enough register pressure (r8, r9 and sl all
 * in use) that agbcc spills the address by itself. Its neighbour 0x08090D7C
 * is the SAME construct for LinkC3_IDLE_08033EC9 and does NOT behave this
 * way; see that function's note, which is the useful half of this pair.
 *
 * PROMOTION WARNING -- NON-CONSECUTIVE POOL WORDS. 0x08090D7C, 0x08090D80 and
 * 0x08090D8C are pool words of this one translation unit, but 0x08090D84 and
 * 0x08090D88 sitting between them belong to sub_080344F0 and RunMapStateMachine,
 * which are promoted separately and reach them by name. split_rodata.py
 * requires a unit's claimed words to be CONSECUTIVE, so this unit and
 * ResetRulesAfterCampaignMap's may need to be carved or split together.
 *
 * Binding `gUnknown_0849BC30[proc->unk30[i]]` to `p` is load-bearing, and is
 * the same single fact as in LinkC3_IDLE_08033EC9: written inline the argument
 * expressions are emitted in the wrong order (the 0x1ff mask and the `& 0xff`
 * ahead of the table lookup), and it ALSO flipped r3/r5 between the two
 * branches. One binding fixed both, which is why the r3/r5 mirror was never a
 * separate problem worth chasing.
 *
 * `((const u16 *)gUnknown_0849BC38)[i]` is a CAST AT THIS USE, not evidence
 * that the object should be retyped. The array is declared `const u8[]` and
 * the promoted c_08033FFC.c depends on `gUnknown_0849BC38[2]` compiling to a
 * `ldrb` of byte 2; widening the declaration would turn that into `ldrh` of
 * byte 4 and silently regress two already-matched functions. The object is
 * three u16s and the u8 readers take their low halves -- both views are
 * consistent on little-endian. */

struct Unk33F1CProc
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ u8 unk30[0x03];
    /* 0x33 */ u8 filler_33[0x03];
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 filler_37[0x01];
    /* 0x38 */ u16 unk38[0x03];
    /* 0x3e */ u8 filler_3e[0x0e];
    /* 0x4c */ u32 unk4c;
};

void LinkC3_IDLE_08033EC9(struct Unk33EC8Proc *proc)
{
    int i;

    proc->unk37 = 0;

    for (i = 0; i < 3; i++)
    {
        const u8 *const *tbl = &gUnknown_08090D7C;
        u16 *const *p = gUnknown_0849BC30[proc->unk30[i]];

        PutSpriteExt(0, 0x60, (*tbl)[i * 2], p[i], (i + 1) << 12);
    }
}

void LinkC3_IDLE_08033F1D(struct Unk33F1CProc *proc)
{
    int i;
    int t;

    for (i = 0; i < 3; i++)
    {
        u16 *const *p;

        if (proc->unk36 == i)
        {
            proc->unk38[i] = 0x60;
            t = Interpolate(5, ((const u16 *)gUnknown_0849BC38)[i],
                ((const u16 *)gUnknown_0849BC38)[1], proc->unk4c, 0x10);
        }
        else
        {
            proc->unk38[i] = Interpolate(1, 0x60, 0xf0, proc->unk4c, 0x10);
            t = ((const u16 *)gUnknown_0849BC38)[i];
        }

        p = gUnknown_0849BC30[proc->unk30[i]];

        PutSpriteExt(0, proc->unk38[i] & 0x1ff, t & 0xff, p[i], (i + 1) << 12);
    }

    if (proc->unk4c > 0x10)
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }
    else
    {
        proc->unk4c++;
    }
}

asm(".global sub_08033EC8\n.thumb_set sub_08033EC8, LinkC3_IDLE_08033EC9\n"
    ".global sub_08033F1C\n.thumb_set sub_08033F1C, LinkC3_IDLE_08033F1D\n");
