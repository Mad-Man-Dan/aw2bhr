#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057464.
 * sub_08057464 @ 0x08057464
 */

/* The debug status readout: a fixed grid of labels and values. Three column
 * origins (0x10 / 0x48+0x88 / 0xc0) crossed with eight rows 0x10..0x48, plus a
 * header line and a footer. `gUnknown_03004550` is read straight through as
 * sixteen s16 slots in row order, which is what fixes the two blocks as the
 * same eight fields printed twice. */

void DrawBattleAnimDebugReadout(void)
{
    PutAsciiStringSprites(gUnknown_08551A4C[gUnknown_03004514], gUnknown_08551A50[gUnknown_03004524], gUnknown_08136170);
    PutAsciiStringSprites(gUnknown_08551A48[gUnknown_030045AC], 0x64, gUnknown_08136174);
    PutAsciiStringSprites(0x10, 0x10, gUnknown_08136178);
    PutAsciiStringSprites(0x10, 0x18, gUnknown_08136180);
    PutAsciiStringSprites(0x10, 0x20, gUnknown_08136188);
    PutAsciiStringSprites(0x10, 0x28, gUnknown_0813618C);
    PutAsciiStringSprites(0x10, 0x30, gUnknown_08136190);
    PutAsciiStringSprites(0x10, 0x38, gUnknown_08136194);
    PutAsciiStringSprites(0x10, 0x40, gUnknown_08136198);
    PutAsciiStringSprites(0x10, 0x48, gUnknown_0813619C);
    PutAsciiStringSprites(0x48, 0x10, gUnknown_08551A60[gUnknown_03004550[0][0]]);
    PutAsciiStringSprites(0x48, 0x18, gUnknown_08551A74[gUnknown_03004550[0][1]]);
    PutAsciiStringSprites(0x48, 0x20, gUnknown_08551AD4[gUnknown_03004550[0][2]]);
    PutAsciiStringSprites(0x48, 0x28, gUnknown_08551AE0[gUnknown_03004550[0][3]]);
    PutAsciiStringSprites(0x48, 0x30, gUnknown_08551B98[gUnknown_03004550[0][4]]);
    PutDecimalNumberSprites(0x48, 0x38, gUnknown_03004550[0][5]);
    PutDecimalNumberSprites(0x48, 0x40, gUnknown_03004550[0][6]);
    PutDecimalNumberSprites(0x48, 0x48, gUnknown_03004550[0][7]);
    PutAsciiStringSprites(0x88, 0x10, gUnknown_08136178);
    PutAsciiStringSprites(0x88, 0x18, gUnknown_08136180);
    PutAsciiStringSprites(0x88, 0x20, gUnknown_08136188);
    PutAsciiStringSprites(0x88, 0x28, gUnknown_0813618C);
    PutAsciiStringSprites(0x88, 0x30, gUnknown_08136190);
    PutAsciiStringSprites(0x88, 0x38, gUnknown_08136194);
    PutAsciiStringSprites(0x88, 0x40, gUnknown_08136198);
    PutAsciiStringSprites(0x88, 0x48, gUnknown_0813619C);
    PutAsciiStringSprites(0xc0, 0x10, gUnknown_08551A60[gUnknown_03004550[1][0]]);
    PutAsciiStringSprites(0xc0, 0x18, gUnknown_08551A74[gUnknown_03004550[1][1]]);
    PutAsciiStringSprites(0xc0, 0x20, gUnknown_08551AD4[gUnknown_03004550[1][2]]);
    PutAsciiStringSprites(0xc0, 0x28, gUnknown_08551AE0[gUnknown_03004550[1][3]]);
    PutAsciiStringSprites(0xc0, 0x30, gUnknown_08551B98[gUnknown_03004550[1][4]]);
    PutDecimalNumberSprites(0xc0, 0x38, gUnknown_03004550[1][5]);
    PutDecimalNumberSprites(0xc0, 0x40, gUnknown_03004550[1][6]);
    PutDecimalNumberSprites(0xc0, 0x48, gUnknown_03004550[1][7]);
    PutAsciiStringSprites(0x50, 0x7c, gUnknown_08551B8C[gUnknown_03004540]);
}
asm(".global sub_08057464\n.thumb_set sub_08057464, DrawBattleAnimDebugReadout\n");
