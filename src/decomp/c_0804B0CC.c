#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B0CC.
 * sub_0804B0CC @ 0x0804B0CC, sub_0804B10C @ 0x0804B10C
 */

/* The pointer global is reloaded before each `strb` but NOT between the +0x58
 * word store and the +0x60 byte store, which is exactly what naming the global
 * on every line produces: a byte store may alias anything, so agbcc drops the
 * cached pointer after each one, while the word store does not force a reload.
 * Do not bind it to a local -- that costs the four reloads.
 *
 * `lsls r1, r1, #0x18; lsrs r1, r1, #0x18` at entry is PROMOTE_MODE on a
 * declared `u8` second parameter; the first is never narrowed, so `int`. */
void StartNameEntryMode0(int a, u8 b)
{
    sub_080152EC(gUnknown_084C3D9C, 0);

    gUnknown_030044E0->unk5c = 0;
    gUnknown_030044E0->unk5e = b;
    gUnknown_030044E0->unk5f = b * 2;
    gUnknown_030044E0->unk58 = a;
    gUnknown_030044E0->unk60 = b * 8;
}
asm(".global sub_0804B0CC\n.thumb_set sub_0804B0CC, StartNameEntryMode0\n");

/* StartNameEntryMode0's twin; the only difference in the whole function is the literal
 * stored into unk5c, so that byte is the pair's discriminator. */
void StartNameEntryMode1(int a, u8 b)
{
    sub_080152EC(gUnknown_084C3D9C, 0);

    gUnknown_030044E0->unk5c = 1;
    gUnknown_030044E0->unk5e = b;
    gUnknown_030044E0->unk5f = b * 2;
    gUnknown_030044E0->unk58 = a;
    gUnknown_030044E0->unk60 = b * 8;
}
asm(".global sub_0804B10C\n.thumb_set sub_0804B10C, StartNameEntryMode1\n");
