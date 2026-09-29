#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805A95C.
 * sub_0805A95C @ 0x0805A95C
 */

/* Publishes gUnknown_0857680F[gUnknown_030040D8->unk00] to gUnknown_030046AC,
 * except that a unit whose class byte reads 4 and whose three-bit +0x09 field
 * is clear publishes 0 instead.
 *
 * Both gUnknown_0857680F and gUnknown_030040D8 are read on BOTH sides of the
 * merge, so agbcc force-addr's each into this unit's own .rodata and reaches
 * them through the three-deep `ldr =word; ldr; ldr` chain. That is why the
 * splitter invented gUnknown_0816D9A4 and gUnknown_0816D9A8 for this function:
 * the ROM words at those two addresses hold 0x0857680F and 0x030040D8, i.e.
 * the ADDRESSES of the two real objects, checked in baserom.gba. Neither is an
 * object of its own and neither is a pointer-to-pointer -- naming the objects
 * honestly reproduces the whole chain, and the build places the two words.
 *
 * gUnknown_030046AC, by contrast, is touched once per arm and gets an ORDINARY
 * inline pool word in each, which is why the same symbol appears twice in the
 * literal pool. Both arms recompute the class-byte lookup rather than sharing
 * it: a non-const global's MEM does not survive the merge.
 *
 * The `strb r1` in the first arm stores the register the `ands` left at zero
 * -- the constant 0 is already there, so `= 0` costs no `movs`.
 *
 * +0x09 of struct Unk030040D8 is unk07[2]: unk07 is the five-byte member array
 * at +0x07 that c_0805DB0C.c also indexes. */
void AiUpdateNeededPropertyKind(void)
{
    if (gUnknown_0857680F[gUnknown_030040D8->unk00] == 4
     && (gUnknown_030040D8->unk07[2] & 7) == 0)
        gUnknown_030046AC = 0;
    else
        gUnknown_030046AC = gUnknown_0857680F[gUnknown_030040D8->unk00];
}
asm(".global sub_0805A95C\n.thumb_set sub_0805A95C, AiUpdateNeededPropertyKind\n");
