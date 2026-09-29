#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D5A0.
 * sub_0802D5A0 @ 0x0802D5A0, sub_0802D5B8 @ 0x0802D5B8, sub_0802D5CC @ 0x0802D5CC
 */

/* A two-call forwarder: the graphic goes to DecompressWindowFrameTiles and the palette pair
 * to ApplyWindowFramePalette. Not `g(f(...))` -- DecompressWindowFrameTiles is void and ApplyWindowFramePalette's
 * arguments are this function's own 2nd and 3rd, materialised out of r4/r5
 * across the first call. */

void LoadWindowFrameGraphics(void *a1, int a2, int a3)
{
    DecompressWindowFrameTiles(a1);
    ApplyWindowFramePalette(a2, a3);
}
asm(".global sub_0802D5A0\n.thumb_set sub_0802D5A0, LoadWindowFrameGraphics\n");

/* `Decompress(GetWindowFrameTileData(), dest)` -- one statement. The accessor's result
 * is already in r0 where Decompress's first parameter wants it, so the only
 * instruction the nesting costs is restoring the saved destination into r1.
 * GetWindowFrameTileData returns gUnknown_080D3FE4 (src/decomp/c_08037250.c). */

void DecompressWindowFrameTiles(void *a1)
{
    Decompress(GetWindowFrameTileData(), a1);
}
asm(".global sub_0802D5B8\n.thumb_set sub_0802D5B8, DecompressWindowFrameTiles\n");

/* `lsls #0x15; lsrs #0x10` is a NET LEFT SHIFT OF FIVE under a (u16) cast, not
 * a mask plus a shift, and it has to be written as the MULTIPLY `(u16)(a2 *
 * 0x20)` so shorten_binary_op folds it into the two instructions the ROM has
 * -- the same spelling src/decomp/c_08037750.c and c_08043AA0.c already carry
 * for this callee. The `ApplyPalettes` macro in include/hardware.h is NOT
 * usable here: it expands to a plain `0x20 * (num)` in `int`, which is one
 * `lsls #5`.
 *
 * `lsls r0,#5` on the first argument is 32 BYTES per row, i.e. 16 u16 entries
 * -- an index on gUnknown_080D4188, declared `u16 []` in wave 29. */

void ApplyWindowFramePalette(int a1, int a2)
{
    ApplyPaletteExt(&gUnknown_080D4188[a1 * 16], (u16)(a2 * 0x20), 0x20);
}
asm(".global sub_0802D5CC\n.thumb_set sub_0802D5CC, ApplyWindowFramePalette\n");
