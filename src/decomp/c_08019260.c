#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019260.
 * sub_08019260 @ 0x08019260
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* `bool8` and not the `int` this was promoted with. Every one of the twenty
 * `bl IsAnyEventScriptRunning` sites in asm/ narrows the result to eight bits, and two of
 * them (`lsls #0x18; lsrs r4,#0x18` and `lsrs r1,#0x18`) KEEP the value rather
 * than truth-testing it, which the bare-`lsls` truth test would not have done.
 * A `bool8` local at one call site is byte-identical to a `bool8` callee, so
 * one site proves nothing -- twenty do. The body is unaffected: it returns the
 * literals 0 and 1, so nonzero_bits is already <= 1 and gcc drops the return
 * narrowing. Re-verified byte-exact with trymatch after the change. */
bool8 IsAnyEventScriptRunning(void)
{
    s16 i;

    for (i = 0; i < 10; i++)
    {
        if (gUnknown_0200C528[i].unk00 != NULL)
            return 1;
    }
    return 0;
}

asm(".global sub_08019260\n.thumb_set sub_08019260, IsAnyEventScriptRunning\n");
