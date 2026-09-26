#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063980.
 * sub_08063980 @ 0x08063980
 */

#include "hardware.h"

/* Sets the VCOUNT compare value in REG_DISPSTAT bits 8-15, leaving bits 0-7
 * as they are.
 *
 * The low half has to be read through the NON-volatile REG_DISPSTAT_LO.  With
 * a `vu8` read the byte lands in a plain pseudo and combine's commutative
 * canonicalisation reorders the `orr` -- see hardware.h and the "orr operand
 * order" note in docs/agbcc-codegen.md.
 */

void sub_08063980(int vcount)
{
    u16 v;

    v = REG_DISPSTAT_LO;
    v |= vcount << 8;
    REG_DISPSTAT = v;
}
