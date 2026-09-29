#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015F68.
 * sub_08015F68 @ 0x08015F68, sub_08015FA8 @ 0x08015FA8
 */

/* SlotOp_CallHandler's conditional twin: the handler's byte-wide result decides
 * whether the cursor advances at all. `lsls #0x18; lsrs #0x18` before the
 * `cmp #1` is the re-narrowing agbcc applies to a narrow-returning callee, so
 * the handler returns a byte and the comparison is against 1 exactly.
 *
 * The advance and the `return TRUE` are the arm UNDER the `if` -- they sit
 * after the literal pool and the `beq` jumps to them -- so the guard is
 * written positively rather than as an early `return FALSE`. The handler is
 * bound to a local for the same ordering reason as SlotOp_CallHandler. */
bool8 sub_08015F68(u8 a)
{
    u8 (*handler)(struct Unk03001470 *) =
        *(u8 (**)(struct Unk03001470 *))gUnknown_03001470[a].unk04;

    if (handler(&gUnknown_03001470[a]) == 1) {
        gUnknown_03001470[a].unk04 = (const u8 *)gUnknown_03001470[a].unk04 + 8;
        return TRUE;
    }
    return FALSE;
}

/* sub_08015F68 with the sense reversed and the test cheaper: a bare
 * `lsls #0x18; cmp r0,#0` with no `lsrs`, which is all a zero test on a
 * byte-wide return needs. The cursor advances only while the handler keeps
 * reporting false. */
bool8 sub_08015FA8(u8 a)
{
    bool8 (*handler)(struct Unk03001470 *) =
        *(bool8 (**)(struct Unk03001470 *))gUnknown_03001470[a].unk04;

    if (handler(&gUnknown_03001470[a]) == 0) {
        gUnknown_03001470[a].unk04 = (const u8 *)gUnknown_03001470[a].unk04 + 8;
        return TRUE;
    }
    return FALSE;
}
