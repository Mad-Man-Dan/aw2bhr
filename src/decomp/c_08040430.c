#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040430.
 * sub_08040430 @ 0x08040430, sub_0804046C @ 0x0804046C
 */

#include "proc.h"
/* FIVE parameters, the fifth on the stack and the Proc_StartBlocking parent.
 * unk4a packs the last two as `hi << 12 | lo`, the same OAM-shaped
 * `tile | pal << 12` halfword struct Unk0801C210's +0x22 carries -- and
 * SiloMissileFall_Init next door copies this very field into that slot. */
struct Unk4046CProc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x16];
    /* 4a */ u16 unk4a;
};

/* Loads one tile/palette pair for a unit sprite: the tiles go to OBJ VRAM at
 * 0x06010000 + tile * 0x20, the palette to OBJ palette slot (pal + 0x10).
 *
 * The palette index is `(u16)((pal + 0x10) * 0x20)` and NOT a mask plus a
 * shift. `adds #0x10; lsls #0x15; lsrs #0x10` is `(u32)x << 21 >> 16`, a net
 * left shift of FIVE; the apparent 11-bit mask is only the u16 truncation of
 * the product. It must also be spelled as a MULTIPLY -- ApplyPaletteExt's
 * second parameter is `u32`, so the (u16) is a real conversion, and
 * shorten_binary_op folds `x * 0x20` into the two instructions the ROM has
 * where `x << 5` needs three.
 *
 * The tile index really is a mask plus a shift, and the two are distinguishable
 * here: `ands` against a pool-loaded 0x3FF is an instruction of its own, where
 * the truncation form emits nothing. */
void LoadSiloMissileGraphics(int tile, int pal)
{
    Decompress(gUnknown_08111000, (void *)(0x06010000 + (tile & 0x3FF) * 0x20));
    ApplyPaletteExt(gUnknown_08111D74, (u16)((pal + 0x10) * 0x20), 0x20);
}
asm(".global sub_08040430\n.thumb_set sub_08040430, LoadSiloMissileGraphics\n");

void StartSiloMissileLaunch(int a, int b, int c, int d, ProcPtr parent)
{
    struct Unk4046CProc *proc = Proc_StartBlocking(gUnknown_0849FB8C, parent);

    proc->unk2c = a;
    proc->unk30 = b;
    proc->unk4a = (d << 12) | c;
}
asm(".global sub_0804046C\n.thumb_set sub_0804046C, StartSiloMissileLaunch\n");
