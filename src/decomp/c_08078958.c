#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078958.
 * sub_08078958 @ 0x08078958
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* Installs one gUnknown_0200C528 list script. StartEventScript returns the slot it
 * allocated, and `pop {r0}; bx r0` here discards it -- so this is void and the
 * call is a bare statement. The script is ROM data reached only as an address,
 * hence `const u8 []` and a clean pool word.
 */

void WM_ConfirmExit_08078959(void)
{
    StartEventScript(gUnknown_08615B4C);
}

asm(".global sub_08078958\n.thumb_set sub_08078958, WM_ConfirmExit_08078959\n");
