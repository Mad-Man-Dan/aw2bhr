#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033C68.
 * sub_08033C68 @ 0x08033C68
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
/* MATCHED at 608/608 bytes with zero differences, wave 74 (W74-I).
 *
 * The previous 98.0% draft differed only in a three-way rotation of spill-slot
 * immediates.  `(p = 0, i + 0xc)` in the first SetObjAffine argument is the
 * source lever: the dead assignment changes first-reference/pseudo association
 * without emitting an instruction, placing `i + 0xc` between the gSinLut
 * force-address pseudo and the two LICM member-address pseudos.  The existing
 * p binding is load-bearing and remains semantic: both arms below overwrite p
 * before its first read.
 *
 * A separate `oam` local does not reproduce this.  Defining an uninitialised
 * int local in the same argument made the function 596 bytes (-12, 6.6%);
 * u16/u8 variants made it 600 bytes (-8, 7.4%).  Static-inline identity and
 * add-12 helpers made it 620 bytes (+12, 15.3%).
 *
 * Everything else in this function was settled by the earlier probes:
 *   - proc layout is the SAME proc as sub_08033B3C (matched this wave) and
 *     c_08033EC8.c's pair: unk30[3]/unk33[3] cursor bytes, unk36 cursor,
 *     unk3e, int unk40[3], u32 unk4c.
 *   - `Div(gSinLut[0x40] * 16, 0x100)` etc: the `lsls #4` is the multiply and
 *     the `lsls #16; asrs #16` after each Div is SetObjAffine's `s16`
 *     parameter narrowing, which comes free from the prototype.
 *   - 0x100 is written as a literal in both the Div divisor and the
 *     `| 0x100` on gUnknown_0849BC38[i * 2]; agbcc CSEs it into sl by itself.
 *   - `gUnknown_0849BC38[i * 2]` on the u8 declaration, NOT a u16 cast --
 *     this is the `ldrb` reader the object's note in unknown-globals.h says
 *     must not be widened (c_08033FFC.c depends on it).
 *   - the unk4c <= 4 / <= 8 tests are UNSIGNED (`bhi`), i.e. u32.
 *   - `stm r3!` in the reset loop is strength_reduce's auto-increment giv and
 *     must not be authored.
 */

struct Unk33C68Proc
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ u8 unk30[0x03];
    /* 0x33 */ u8 unk33[0x03];
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 filler_37[0x01];
    /* 0x38 */ u16 unk38[0x03];
    /* 0x3e */ u16 unk3e;
    /* 0x40 */ int unk40[0x03];
    /* 0x4c */ u32 unk4c;
};

void LinkC3_IDLE_08033C69(struct Unk33C68Proc *proc)
{
    int i;

    for (i = 0; i < 3; i++)
    {
        u16 *const *p;

        SetObjAffine((p = 0, i + 0xc),
                     Div(gSinLut[0x40] * 16, 0x100),
                     Div(-gSinLut[0] * 16, 0x100),
                     Div(gSinLut[0] * 16, 0x100),
                     Div(gSinLut[0x40] * 16, 0x100));

        if (proc->unk30[i] != proc->unk33[i])
        {
            if (proc->unk4c <= 4)
                proc->unk3e = Interpolate(0, 0x100, 0x10, proc->unk4c, 4);
            else if (proc->unk4c <= 8)
                proc->unk3e = Interpolate(0, 0x10, 0x100, proc->unk4c - 4, 4);

            if (proc->unk30[i] != 0)
                SetObjAffine(i + 0xc,
                             Div(gSinLut[0x40] * 16, 0x100),
                             Div(-gSinLut[0] * 16, proc->unk3e),
                             Div(gSinLut[0] * 16, 0x100),
                             Div(gSinLut[0x40] * 16, proc->unk3e));

            if (proc->unk33[i] != 0)
                SetObjAffine(i + 0xc,
                             Div(gSinLut[0x40] * 16, 0x100),
                             Div(-gSinLut[0] * 16, proc->unk3e),
                             Div(gSinLut[0] * 16, 0x100),
                             Div(gSinLut[0x40] * 16, proc->unk3e));
        }

        if (proc->unk4c <= 4)
            p = gUnknown_0849BC30[proc->unk33[i]];
        else
            p = gUnknown_0849BC30[proc->unk30[i]];

        PutSpriteExt(0, ((i + 0xc) << 9) | 0x60,
                     gUnknown_0849BC38[i * 2] | 0x100,
                     p[i], (i + 1) << 12);
    }

    proc->unk4c++;

    if (proc->unk4c > 8)
    {
        proc->unk4c = 0;

        for (i = 0; i < 3; i++)
        {
            proc->unk40[i] = 0;
            proc->unk33[i] = proc->unk30[i];
        }

        LinkC4_StartCursorProc(proc->unk36, proc);
        LinkScreenSetMessage(0, gUnknown_0849BC3E[proc->unk36], 0x40);
        Proc_Break(proc);
    }
}

asm(".global sub_08033C68\n.thumb_set sub_08033C68, LinkC3_IDLE_08033C69\n");
