#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040640.
 * ApplySiloDamage @ 0x08040640
 */

#include "proc.h"
/* Redraws the 13-cell diamond around (unk2c, unk30) and then re-places the
 * cursor sprite. Thirteen independent DamageUnitAtCell calls, in the ROM's order:
 * the row offsets run -2, -1, 0, +1, +2 and the column offsets fan out inside
 * each row, which is the source's order and the only thing to get right here.
 *
 * &proc->unk44 lands in r5 for the whole run because THUMB has no
 * immediate-offset `ldrsh` -- that register is the addressing mode, not a
 * source feature. */
struct Unk40640Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x10];
    /* 44 */ s16 unk44;
    /* 46 */ u8 filler_46[0x04];
    /* 4a */ u16 unk4a;
    /* 4c */ u8 filler_4c[0x08];
    /* 54 */ int unk54;
    /* 58 */ int unk58;
};

void ApplySiloDamage(struct Unk40640Proc *proc)
{
    DamageUnitAtCell(proc->unk2c,     proc->unk30 - 2, proc->unk44);
    DamageUnitAtCell(proc->unk2c - 1, proc->unk30 - 1, proc->unk44);
    DamageUnitAtCell(proc->unk2c,     proc->unk30 - 1, proc->unk44);
    DamageUnitAtCell(proc->unk2c + 1, proc->unk30 - 1, proc->unk44);
    DamageUnitAtCell(proc->unk2c - 2, proc->unk30,     proc->unk44);
    DamageUnitAtCell(proc->unk2c - 1, proc->unk30,     proc->unk44);
    DamageUnitAtCell(proc->unk2c,     proc->unk30,     proc->unk44);
    DamageUnitAtCell(proc->unk2c + 1, proc->unk30,     proc->unk44);
    DamageUnitAtCell(proc->unk2c + 2, proc->unk30,     proc->unk44);
    DamageUnitAtCell(proc->unk2c - 1, proc->unk30 + 1, proc->unk44);
    DamageUnitAtCell(proc->unk2c,     proc->unk30 + 1, proc->unk44);
    DamageUnitAtCell(proc->unk2c + 1, proc->unk30 + 1, proc->unk44);
    DamageUnitAtCell(proc->unk2c,     proc->unk30 + 2, proc->unk44);
    RebuildMapUnitLayers2();
    APProc_Create(gUnknown_08111D94,
                 proc->unk54 - gMap->scrollX,
                 proc->unk58 - gMap->scrollY,
                 proc->unk4a, 3, 1);
    PlayMusicOrSfx2(0x1C7);
}

asm(".global sub_08040640\n.thumb_set sub_08040640, ApplySiloDamage\n");
