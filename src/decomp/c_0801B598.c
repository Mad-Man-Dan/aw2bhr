#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B598.
 * sub_0801B598 @ 0x0801B598, sub_0801B5C0 @ 0x0801B5C0, sub_0801B5E8 @ 0x0801B5E8, sub_0801B618 @ 0x0801B618, sub_0801B648 @ 0x0801B648, sub_0801B66C @ 0x0801B66C
 */

/* One of six wrappers at 0x0801B598 .. 0x0801B66C that gate a call on
 * gUnknown_0200CD0C == 1 and return a constant 1 when it is not armed.
 *
 * The arm that CALLS is the branch target (`beq _0801B5B0`), and that is what
 * fixes the source: agbcc lays the ELSE arm inline and branches to the THEN
 * arm, so the condition is `== 1` with the call as the then-arm. Written the
 * other way round -- `if (g != 1) return 1;` as a guard -- agbcc emits the
 * mirror image, `bne` to the constant. Both spellings are 40 bytes, so the size
 * does not discriminate and only the branch sense does.
 *
 * TWO PARAMETERS, not one, and the second is invisible except as register
 * pressure: the ROM narrows `a` into r2, skipping r1, because r1 is already
 * carrying `b` through to the callee. A one-parameter draft puts the narrowed
 * value in r1 and misses by exactly those 2 bytes. See SetFlashTimerIntr's note in
 * include/unknown-functions.h.
 *
 * Both shift pairs are real and independent: `lsls #0x18; lsrs #0x18` on entry
 * is the u8 parameter, `lsls #0x10; lsrs #0x10` after the call is agbcc
 * re-narrowing a u16-returning callee. */
u16 SetFlashTimerIntrIfPresent(u8 a, void (**b)(void))
{
    if (gUnknown_0200CD0C == 1)
        return SetFlashTimerIntr(a, b);
    else
        return 1;
}
asm(".global sub_0801B598\n.thumb_set sub_0801B598, SetFlashTimerIntrIfPresent\n");

/* The nullary member of the 0x0801B598 gate family. `bl _call_via_r0` -- the
 * pointer in r0, the FIRST scratch register -- is the readout that the call
 * takes no arguments; see gUnknown_03005C80 in include/unknown-globals.h.
 *
 * Unlike its siblings the pool word here is dereferenced (`ldr r0, =sym` then
 * `ldr r0, [r0]`), so gUnknown_03005C80 really is a function pointer VARIABLE
 * and not one of the odd-address literals this block also carries. */
u16 EraseFlashChipIfPresent(void)
{
    if (gUnknown_0200CD0C == 1)
        return gUnknown_03005C80();
    else
        return 1;
}
asm(".global sub_0801B5C0\n.thumb_set sub_0801B5C0, EraseFlashChipIfPresent\n");

/* The one-argument member of the 0x0801B598 gate family: `bl _call_via_r1`,
 * with r0 already holding the forwarded parameter, so r1 is the first free
 * scratch and the call takes exactly one argument.
 *
 * The literal pool sitting in the MIDDLE of the function, before the call arm,
 * is ordinary agbcc output -- the early `b` to the epilogue opened the gap. */
u16 EraseFlashSectorIfPresent(u16 a)
{
    if (gUnknown_0200CD0C == 1)
        return gUnknown_03005C84(a);
    else
        return 1;
}
asm(".global sub_0801B5E8\n.thumb_set sub_0801B5E8, EraseFlashSectorIfPresent\n");

/* The two-argument member of the 0x0801B598 gate family: `bl _call_via_r2`
 * puts the callee pointer in r2 because r0 and r1 are both taken, so the call
 * takes two arguments even though only one of them costs an instruction.
 *
 * `b` is never touched -- it rides r1 from this function's own frame straight
 * into the callee -- which is why nothing here types it beyond `int`. */
u16 ProgramFlashSectorIfPresent(u16 a, int b)
{
    if (gUnknown_0200CD0C == 1)
        return gUnknown_03005C74(a, b);
    else
        return 1;
}
asm(".global sub_0801B618\n.thumb_set sub_0801B618, ProgramFlashSectorIfPresent\n");

/* The member of the 0x0801B598 gate family whose un-armed arm returns its
 * SECOND PARAMETER rather than a constant: `adds r0, r1, #0` where the others
 * have `movs r0, #1`. That is what proves r1 is a real parameter here.
 *
 * It is also what settles the return type, and through it the callee's. The
 * function returns `int` because one arm returns an `int` parameter; the other
 * arm returns VerifyFlashSector's result with NO re-narrowing, and agbcc re-narrows
 * a narrow-returning callee at every call site, so VerifyFlashSector returns `int`
 * too. Read on its own a tail call proves nothing about the callee's width --
 * it is the other arm that makes this one decisive. */
int VerifyFlashSectorIfPresent(u16 a, int b)
{
    if (gUnknown_0200CD0C == 1)
        return VerifyFlashSector(a, b);
    else
        return b;
}
asm(".global sub_0801B648\n.thumb_set sub_0801B648, VerifyFlashSectorIfPresent\n");

/* The void member of the 0x0801B598 gate family, and the one that measures its
 * callee's ARITY.
 *
 * It forwards only two arguments, yet it spends `push {r4, lr}` and
 * `adds r4, r1, #0` parking its second parameter in a callee-saved register
 * before narrowing its first into r1. Probed against 2-, 3- and 4-argument
 * declarations of ReadFlash, only FOUR reproduces that: at two or three, r2
 * (and r3) are free, the narrowed value lands there, and nothing is saved.
 *
 * So pass-through arguments, invisible as instructions, are visible as register
 * pressure once they occupy the last free scratch -- the direct-call analogue
 * of the `_call_via_rN` tell. The third and fourth parameters are `int` because
 * that is what costs no instruction; nothing here types them further. */
void ReadFlashIfPresent(u16 a, int b, int c, int d)
{
    if (gUnknown_0200CD0C == 1)
        ReadFlash(a, b, c, d);
}
asm(".global sub_0801B66C\n.thumb_set sub_0801B66C, ReadFlashIfPresent\n");
