#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806ADD0.
 * sub_0806ADD0 @ 0x0806ADD0, sub_0806ADF0 @ 0x0806ADF0, sub_0806AE18 @ 0x0806AE18
 */

struct Unk806ADD0
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 *unk30;
};
struct Unk806ADF0
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 *unk30;
};
struct Unk806AE18
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 *unk30;
};

/* First of three siblings that unpack one gUnknown_0858178C row into three
 * consecutive 0x1B00-byte destinations. `lsls #2; adds; lsls #2` is a multiply
 * by 20, i.e. the row stride, so this is an array index and not hand-rolled
 * address arithmetic. */
void CreditsIllustration_LoadTiles0(struct Unk806ADD0 *proc)
{
    Decompress(gUnknown_0858178C[proc->unk2c].unk00, proc->unk30);
}
asm(".global sub_0806ADD0\n.thumb_set sub_0806ADD0, CreditsIllustration_LoadTiles0\n");

/* The CreditsIllustration_LoadTiles0 sibling for the second blob. `movs r2, #0xd8; lsls r2, #5`
 * is the constant 0x1B00, not a shift in the source. */
void CreditsIllustration_LoadTiles1(struct Unk806ADF0 *proc)
{
    Decompress(gUnknown_0858178C[proc->unk2c].unk04, proc->unk30 + 0x1B00);
}
asm(".global sub_0806ADF0\n.thumb_set sub_0806ADF0, CreditsIllustration_LoadTiles1\n");

/* The third CreditsIllustration_LoadTiles0 sibling. `movs r2, #0xd8; lsls r2, #6` is 0x3600 --
 * twice CreditsIllustration_LoadTiles1's offset, which is what makes the three destinations
 * consecutive 0x1B00-byte blocks. */
void CreditsIllustration_LoadTiles2(struct Unk806AE18 *proc)
{
    Decompress(gUnknown_0858178C[proc->unk2c].unk08, proc->unk30 + 0x3600);
}
asm(".global sub_0806AE18\n.thumb_set sub_0806AE18, CreditsIllustration_LoadTiles2\n");
