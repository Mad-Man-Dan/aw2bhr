#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F150.
 * sub_0801F150 @ 0x0801F150, sub_0801F178 @ 0x0801F178
 */

/* Fills one gUnknown_0200F920 header and then loads its palette. The fourth
 * parameter is `int`, not `u8` -- see the note in include/unknown-functions.h;
 * the `strb` at +4 is the member truncating. The `strh` at +8 lands on
 * unk08[0].unk00, the first entry of the array src/decomp/c_0801F114.c
 * sweeps. */
void InitTilePool(int a, void *b, u16 c, int d)
{
    gUnknown_0200F920[a].unk00 = b;
    gUnknown_0200F920[a].unk04 = d;
    gUnknown_0200F920[a].unk05 = 0;
    gUnknown_0200F920[a].unk08[0].unk00 = c;
    LoadTilePoolPalette(a, d);
}
asm(".global sub_0801F150\n.thumb_set sub_0801F150, InitTilePool\n");

/* `lsls #0x15; lsrs #0x10` is a NET LEFT SHIFT OF FIVE under a `(u16)` cast,
 * and it must be spelled as the MULTIPLY `b * 0x20`: shorten_binary_op folds a
 * MULT_EXPR into the pair, where `b << 5` needs a third instruction. Both
 * parameters are `int` -- a `u8` second one splits the pair into `lsls #0x18`
 * at entry and `lsrs #0x13` here. */
void LoadTilePoolPalette(int a, int b)
{
    ApplyPaletteExt(gUnknown_0848B738[a].unk04, (u16)(b * 0x20), 0x20);
}
asm(".global sub_0801F178\n.thumb_set sub_0801F178, LoadTilePoolPalette\n");
