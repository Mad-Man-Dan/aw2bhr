#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080EE4.
 * sub_08080EE4 @ 0x08080EE4, sub_08080EF8 @ 0x08080EF8
 */

/* Family F031. sub_08043E18's body never reads r0 -- it opens with
 * `bl IsBlackHoleCo` -- so the parameter is DEAD inside the callee and is only
 * visible here and at sub_08043DAC, which sets r0 up with `ldrb [r0,#0x1d]`.
 * The load of gUnknown_03005970 could not survive -O2 if it fed nothing, so
 * the parameter is real however unused; see unknown-functions.h. */

void sub_08080EE4(void)
{
    sub_08043E18(gUnknown_03005970);
}

/* Family F031, the twin of sub_08080EE4 -- same global, sibling callee. Like
 * sub_08043E18, sub_08043DF4 never reads its parameter; see the note on both
 * in include/unknown-functions.h. */

void sub_08080EF8(void)
{
    sub_08043DF4(gUnknown_03005970);
}
