#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AADC.
 * sub_0806AADC @ 0x0806AADC
 */

struct Unk806AADC
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ int unk4c;
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u16 unk52;
};

/* `lsls #0x15; lsrs #0x10` is a net LEFT shift of five under a (u16) cast,
 * i.e. `(u16)(n * 0x20)` -- and it has to be written as a multiply, because
 * shorten_binary_op folds a MULT into the narrow mode where a shift needs a
 * third instruction. The second transfer's `lsls #5` carries no cast and is
 * an ordinary int shift, so the two are not the same expression. */
void CreditsMissionLine_LoadRankGraphics(struct Unk806AADC *proc)
{
    ApplyPaletteExt(gUnknown_085816F0[proc->unk4c].unk04,
                    (u16)((proc->unk50 + 0x10) * 0x20), 0x20);
    Decompress(gUnknown_085816F0[proc->unk4c].unk00,
               (void *)(proc->unk52 * 0x20 + 0x06011200));
}
asm(".global sub_0806AADC\n.thumb_set sub_0806AADC, CreditsMissionLine_LoadRankGraphics\n");
