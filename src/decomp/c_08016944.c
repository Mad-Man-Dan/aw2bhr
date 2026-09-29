#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016944.
 * sub_08016944 @ 0x08016944, sub_08016974 @ 0x08016974, sub_080169A4 @ 0x080169A4
 */

/* The hide half of a hide/show pair with ClearSlotSpriteDoubleSize, and the same template as
 * src/decomp/c_080154C4.c: fetch the slot's stashed OBJ attributes into a stack
 * local with CopySlotSpriteAttrs, poke one bit, hand the eight bytes back by value to
 * SetSlotSpriteAttrs. The bit is attr0's 9 -- `ldrb [r5,#1]; orrs #2` -- which is why
 * struct OamData's old 2-bit `affineMode` had to be split into `affineEnable`
 * and `doubleSize`; the 2-bit field costs eight bytes extracting and
 * re-inserting the pair. See the note on the split in unknown-globals.h. */
void SetSlotSpriteDoubleSize(int a)
{
    struct OamData o;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&o);
    o.doubleSize = 1;
    SetSlotSpriteAttrs(a, *(struct UnkVec *)&o);
}
asm(".global sub_08016944\n.thumb_set sub_08016944, SetSlotSpriteDoubleSize\n");

/* SetSlotSpriteDoubleSize's other half -- the SAME source with the bit cleared. The
 * `movs r0,#3; rsbs r0,r0,#0; ands` mask is the one-bit-field clear: a 32-bit
 * ~2, where a hand-rolled `*(u8 *)&o + 1 &= ~2` folds to a bare `movs #0xfd`
 * and comes out two bytes short. */
void ClearSlotSpriteDoubleSize(int a)
{
    struct OamData o;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&o);
    o.doubleSize = 0;
    SetSlotSpriteAttrs(a, *(struct UnkVec *)&o);
}
asm(".global sub_08016974\n.thumb_set sub_08016974, ClearSlotSpriteDoubleSize\n");

/* sub_0801E968's twin -- the same dispatch through
 * gUnknown_0200E438[...].unk44, minus the stash into gUnknown_03000550. Two
 * arguments, read off `bl _call_via_r2` (gcc parks the pointer in the first
 * free scratch register). The `u32` member has to be cast back to the pointer
 * type; see the note on gUnknown_03000550 in include/unknown-globals.h. */
void sub_080169A4(s16 a, void *b)
{
    void (*f)(s16, void *);

    if (a == -1)
        return;
    f = (void (*)(s16, void *))gUnknown_0200E438[gUnknown_03001470[a].unk26].unk44;
    if (f == NULL)
        return;
    f(a, b);
}
