#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075298.
 * sub_08075298 @ 0x08075298
 */

#include "proc.h"
struct Unk08075298
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x04];
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 unk36;
    /* 0x38 */ u16 unk38;
};

/* Wave 54 (W54-F): a3 and a4 retyped u16 -> s16, settled from the only caller
 * in the ROM. SnapWorldMapCursorToMarker narrows both arguments with `lsls #0x10; asrs #0x10`
 * -- a SIGNED narrowing, which a `u16` parameter cannot produce (it would emit
 * `lsls; lsrs`). Byte-neutral here: PROMOTE_MODE zero-extends every sub-word
 * parameter at entry regardless of signedness, and both are only ever `strh`-ed
 * into u16 members. a5 stays u16 -- its only call site passes a literal 0, so
 * nothing discriminates it. */

void StartDifficultyStars(ProcPtr parent, int a2, s16 a3, s16 a4, u16 a5)
{
    struct Unk08075298 *proc = Proc_Start(ProcScr_WM_MoveScope, parent);

    proc->unk34 = a3;
    proc->unk36 = a4;
    proc->unk2c = a2;
    proc->unk38 = a5;
}
asm(".global sub_08075298\n.thumb_set sub_08075298, StartDifficultyStars\n");
