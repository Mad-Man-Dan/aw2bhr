#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080154C4.
 * sub_080154C4 @ 0x080154C4, sub_08015504 @ 0x08015504
 */

/* Set one OBJ attribute bit on the slot's stashed attributes: fetch the eight
 * bytes into a stack local with CopySlotSpriteAttrs, poke one bitfield, hand them
 * back by value to SetSlotSpriteAttrs. SetSlotSpriteFlicker is the SAME SOURCE one bit
 * along (`bpp`, `lsls #5`, mask ~0x20).
 *
 * The `movs r0,#0x11; rsbs r0,r0,#0` mask is the bitfield-store tell: it is a
 * 32-bit ~0x10, which is what an `o.mosaic = b` assignment produces. Spelling
 * the same thing by hand through a `u8 *` folds the mask to `movs r0,#0xef`
 * and comes out two bytes short.
 *
 * Both parameters are DECLARED NARROW even though the prologue is
 * copy-then-narrow. That shape is normally the `int`-with-a-cast tell, but it
 * is also what agbcc emits for a declared-narrow parameter whose pseudo lives
 * across a call -- assign_parms' incoming copy and the PROMOTE_MODE extension
 * stop fusing. Measured against `(int, int)` and `(int, u8)`; see the note in
 * include/unknown-functions.h.
 */
void SetSlotSpriteHidden(s16 a, u8 b)
{
    struct OamData o;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&o);
    o.mosaic = b;
    SetSlotSpriteAttrs(a, *(struct UnkVec *)&o);
}
asm(".global sub_080154C4\n.thumb_set sub_080154C4, SetSlotSpriteHidden\n");

/* SetSlotSpriteHidden one bit along -- see the comment there. The bit is 13 of the
 * first attribute word, i.e. `struct OamData`'s `bpp`, NOT `vFlip`: vFlip is
 * bit 29 and lives in byte 3, where CruiserPart2_Init/BattleshipPart2_Init write it.
 */
void SetSlotSpriteFlicker(s16 a, u8 b)
{
    struct OamData o;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&o);
    o.bpp = b;
    SetSlotSpriteAttrs(a, *(struct UnkVec *)&o);
}
asm(".global sub_08015504\n.thumb_set sub_08015504, SetSlotSpriteFlicker\n");
