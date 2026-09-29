#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080763C0.
 * sub_080763C0 @ 0x080763C0
 */

#include "hardware.h"

/* The shift is `lsrs`, not `asrs`, so the masked value is UNSIGNED at the
 * source level -- gGameClock is a declared `s32` and `(g & 0x3F) / 4`
 * gives the arithmetic shift instead. The `(u32)` cast is what the ROM says.
 *
 * The index MUST be hoisted to its own local. Left inline in the subscript,
 * agbcc hoists BOTH address pool words to the top of the function, ahead of
 * the gGameClock load -- 17 of 52 bytes wrong.
 *
 * `gPal[0x157]` and NOT `gPal + 0x157`: the subscript keeps the symbol and the
 * 0x2AE as two separate pool words the way the ROM has them, while the pointer
 * form lets agbcc bake the addend into a single `gPal+0x2ae` word.
 *
 * The SOURCE ARRAY IS BOUND TO ITS OWN POINTER LOCAL, AND THE BINDING SITS
 * AFTER `i`'s. That is the whole of the last 4-byte diff, and it is one fact,
 * not two: `expand_assignment` forces the destination's address first, so
 * written as the single statement `gPal[0x157] = gUnknown_081D22A4[i]` agbcc
 * creates &gPal's pseudo before &gUnknown_081D22A4's and emits the two `ldr`s
 * -- and hence the two pool words -- in that order, the opposite of the ROM.
 * Binding `src` moves &gUnknown_081D22A4's creation into its own statement,
 * ahead of the assignment. Its POSITION among the declarations is what fixes
 * the rest: declared before `i` the `ldr` hoists to the very first instruction
 * of the function, ahead of the gGameClock load; declared after `i` it
 * lands exactly where the ROM has it. */
void CycleWorldMapCalloutColor(void)
{
    int i = ((u32)gGameClock & 0x3F) / 4;
    u16 *src = gUnknown_081D22A4;

    gPal[0x157] = src[i];
    EnablePaletteSync();
}
asm(".global sub_080763C0\n.thumb_set sub_080763C0, CycleWorldMapCalloutColor\n");
