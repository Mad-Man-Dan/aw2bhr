#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F3D4.
 * sub_0801F3D4 @ 0x0801F3D4, sub_0801F400 @ 0x0801F400
 */

/* GetTilePoolFirstGraphic's inverse: a tile id back to its gUnknown_0848B738 row.
 *
 * THE LAST CLAUSE MUST BE A BARE `return 0;`, NOT AN `else`. With
 * `else return 0;` -- or with the test written `a <= 0x3d`, or `a < 0x3e`, or
 * `!(a > 0x3d)`, all of which were measured -- gcc lays the two constants out
 * the other way round and emits `ble` where the ROM has `bgt`. Only the
 * trailing bare return lets the function's last block fall into the epilogue in
 * the ROM's order. See the note added to docs/agbcc-codegen.md. */
int GetTilePoolForGraphic(int a)
{
    if (a > 0xbb)
        return 5;
    if (a > 0xb7)
        return 4;
    if (a > 0xab)
        return 3;
    if (a > 0x42)
        return 2;
    if (a > 0x3d)
        return 1;
    return 0;
}
asm(".global sub_0801F3D4\n.thumb_set sub_0801F3D4, GetTilePoolForGraphic\n");

/* The third column of gUnknown_0848B738 read out of a jump table -- the six
 * constants are literally the words at +8 of the six rows in baserom.gba. No
 * default: out of range, r0 is never written and the argument falls back out. */
int GetTilePoolFirstGraphic(int a)
{
    switch (a) {
    case 0:
        return 0;
    case 1:
        return 0x3e;
    case 2:
        return 0x43;
    case 3:
        return 0xac;
    case 4:
        return 0xb8;
    case 5:
        return 0xbc;
    }
}
asm(".global sub_0801F400\n.thumb_set sub_0801F400, GetTilePoolFirstGraphic\n");
