#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801AD70.
 * sub_0801AD70 @ 0x0801AD70
 */

/* Rejects a slot id: 1 = reject, 0 = accept.
 *
 * sub_0801B120 returns u16 with 0xFFFF as its "none" sentinel -- its own body
 * loads that value as the POSITIVE pool constant 0x0000FFFF, which an s16
 * function returning -1 would not do -- so `lsls r0,#0x10; cmp r0,#0xFFFF0000`
 * here is combine folding the callee's re-narrow into `== 0xffff`. Reading it
 * as `(s16)result == -1` is byte-identical at this call site and wrong about
 * the callee.
 *
 * The two guards are SEPARATE statements, not `if (A || B) return 1;`. The
 * `||` spelling is not byte-identical: it makes both tests branch forward, so
 * the shared `movs r0,#1` block has no fall-through predecessor and
 * cross-jumping deletes it in favour of the loop's first reject. The ROM keeps
 * it at the top with the loop reaching it by `b _0801AD88`. See
 * docs/agbcc-codegen.md, "Where the shared `movs rD,#K` ... LANDS".
 *
 * The two arms of the flag test reach that same `return 1` by opposite routes:
 * with bit 4 clear a MATCHING id rejects, with bit 4 set a non-matching one
 * does. */
int IsSaveSlotInvalid(u8 id)
{
    int i;

    if (id == 0xff)
        return 1;

    if (FindNewestCompleteSave(id) == 0xffff)
        return 1;

    for (i = 0; i < 0x10; i++)
    {
        if ((gUnknown_0200CC38.unk20[i] & 0x10) == 0)
        {
            if (gUnknown_0200CC38.unk00[i] == id)
                return 1;
        }
        else if (gUnknown_0200CC38.unk00[i] != id)
            return 1;
    }

    return 0;
}
asm(".global sub_0801AD70\n.thumb_set sub_0801AD70, IsSaveSlotInvalid\n");
