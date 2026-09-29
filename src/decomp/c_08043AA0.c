#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043AA0.
 * sub_08043AA0 @ 0x08043AA0, sub_08043AC0 @ 0x08043AC0, sub_08043AFC @ 0x08043AFC, sub_08043B14 @ 0x08043B14, sub_08043B44 @ 0x08043B44, sub_08043B60 @ 0x08043B60, sub_08043BA4 @ 0x08043BA4, sub_08043BC8 @ 0x08043BC8, sub_08043BF8 @ 0x08043BF8
 */

/* GetLoadedCoPalette's result reaches sub_08043AC0's third parameter with a bare
 * `adds r2, r0, #0` and no re-narrowing, which is what forced GetLoadedCoPalette's
 * return type from `u8` to `int` (wave 28, W28-B).
 */
void sub_08043AA0(int a, int b)
{
    sub_08043AC0(a % 24, b, GetLoadedCoPalette(a % 24));
}

/* Loads one 16-colour palette out of the slot's palette run.
 *
 * The DOUBLE `% 24` is real: the ROM calls __modsi3 twice in a row on the same
 * value, the second taking the first's result.  Every caller has already
 * reduced the index and this reduces it again.  The OUTER one has to be the
 * subscript's own and the inner one a separate statement -- written as a single
 * `a % 24 % 24` subscript the `ldr r4` of the table address is scheduled ahead
 * of BOTH calls, where the ROM has it between them (8 bytes).
 *
 * `(u16)(b * 0x20)` and not `b << 5`: `lsls #0x15; lsrs #0x10` is a net left
 * shift of five under a u16 truncation, which shorten_binary_op folds into two
 * instructions for a MULT_EXPR and not for a shift.
 */
void sub_08043AC0(int a, int b, int c)
{
    int i = a % 24;

    ApplyPaletteExt(gUnknown_084A0090[i % 24].palette + c * 16, (u16)(b * 0x20), 0x20);
}

void sub_08043AFC(int a, int b)
{
    sub_08043BC8(a, b);
    sub_08043BF8(a, b);
}

/* Decompresses the slot's +0x04 blob into OBJ VRAM at tile `b & 0x3ff`.
 * 0x06010000 is the OBJ tile base and 0x20 the bytes per 4bpp tile.
 */
void LoadCoNameGraphic(int a, int b)
{
    Decompress(gUnknown_084A0090[a].nameGraphic, (void *)(0x06010000 + (b & 0x3ff) * 32));
}
asm(".global sub_08043B14\n.thumb_set sub_08043B14, LoadCoNameGraphic\n");

void sub_08043B44(int a)
{
    ApplyPaletteExt(gUnknown_080F6164, (u16)((a + 0x10) * 0x20), 0x20);
}

/* A PutSpriteExt front end.  The two masked ORs look like no-ops and are:
 * `xh | x` where `xh` is `(u16)(x & ~0x1ff)` is just x.  The sibling wrappers
 * next door (sub_08043FD8, sub_0804402C) build the same two words as
 * `(x & 0x1ff) | (u16)(x & ~0x1ff)` and `((y - k) & 0xff) | (u16)(y & ~0xff)`,
 * i.e. they replace the coordinate field of a packed coordinate/flags word and
 * keep the flag bits; this one passes the coordinate straight through and so
 * leaves the low mask off.
 *
 * The two halves must be LOCALS, not sub-expressions of the arguments.  Spelled
 * inline as `x | (u16)(x & ~0x1ff)` agbcc finishes argument 2 before starting
 * argument 3, so the first `orrs` lands between the two mask/truncate pairs;
 * the ROM does both truncations first and then both `orrs`, and it costs the
 * second mask a different scratch register.  12 bytes.
 */
void sub_08043B60(int x, int y, u32 oam2, u32 layer)
{
    u16 xh = x & ~0x1ff;
    u16 yh = y & ~0xff;

    PutSpriteExt(layer, xh | x, yh | y, gUnknown_084A0730, oam2);
}

void sub_08043BA4(int a, int b, int c)
{
    sub_08043BC8(a, b);
    sub_08043BF8(a, b);
    sub_08043AA0(a, c + 0x10);
}

/* +0x00 of the slot record points at a two-element table of Decompress
 * sources; this loads element [0] and sub_08043BF8 element [1].
 */
void sub_08043BC8(int a, int b)
{
    Decompress(gUnknown_084A0090[a].fullBody[0], (void *)(0x06010000 + (b & 0x3ff) * 32));
}

void sub_08043BF8(int a, int b)
{
    Decompress(gUnknown_084A0090[a].fullBody[1], (void *)(0x06010000 + ((b + 0x80) & 0x3ff) * 32));
}
