#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049928.
 * sub_08049928 @ 0x08049928
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Family F032. sub_08019260 takes no argument either (src/decomp/c_08019260.c
 * is `f(void)`), so the untouched r0 across the `bl` is a dead incoming value
 * and not an argument.
 *
 * sub_08019260 was promoted returning `int`, which cannot produce the ROM's
 * `lsls r0, r0, #0x18`; it is `bool8`, and the evidence is the whole ROM rather
 * than this call -- all twenty `bl sub_08019260` sites narrow to eight bits and
 * two of them keep the value with a trailing `lsrs #0x18`. A `bool8` local here
 * would reach the same bytes without touching the callee, so this one site
 * proves nothing on its own. Retyped in c_08019260.c and re-verified there. */

void ShopScreen_WaitMessageEnd(ProcPtr proc)
{
    if (sub_08019260() == 0)
        Proc_Break(proc);
}

asm(".global sub_08049928\n.thumb_set sub_08049928, ShopScreen_WaitMessageEnd\n");
