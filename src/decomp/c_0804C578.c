#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C578.
 * sub_0804C578 @ 0x0804C578
 */

/* The table lookup indexes the PARAMETER, not a re-read of gUnknown_0300453C:
 * the ROM scales r0 -- still holding the zero-extended argument -- with
 * `lsls r1, r0, #1` after the `strh`, and never reloads the global.
 *
 * The trailing `bl SpawnWholeFigure` has no argument setup because r0 already holds
 * that same value; the declared `void SpawnWholeFigure(u16)` and the two matched
 * F086 wrappers beside it (src/decomp/c_0804C488.c) fix it as an argument.
 *
 * u16 parameter by PROMOTE_MODE: `lsls #0x10; lsrs #0x10` at entry. */
void sub_0804C578(u16 a)
{
    gUnknown_0300453C = a;
    gUnknown_0300451C = gUnknown_08552148[a];
    SpawnWholeFigure(a);
}
