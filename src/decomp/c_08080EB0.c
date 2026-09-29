#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080EB0.
 * sub_08080EB0 @ 0x08080EB0
 */

#include "proc.h"

/* WAVE 76: MATCHED. A fixed-r1 comparison copy gives the parameter the ROM's
 * call-clobbered register without extending its lifetime across either call.
 * Configured try_match is byte-for-byte exact at 52/52 with matching relocs.
 *
 * PARKED at 50 of 52 bytes (wave 30, W30-D).
 *
 * Every instruction below matches the ROM, in order, including both pool words
 * and their placement. The ONLY difference is a register-allocation artefact:
 * the ROM opens `adds r1, r0, #0` and does both `cmp` against r1, while this
 * body keeps the parameter in r0 and compares r0 directly. 2 bytes.
 *
 * Ruled out by probe (all compiled, none produced the copy):
 *   - `if (Proc_Find(x)) return 1;` falling through to a shared `return type`
 *     -- makes `type` live across the `bl`, so it lands in r4 and the ROM's
 *     `push {lr}` becomes `push {r4, lr}`. This is what fixes the shape as
 *     else-if with a per-branch return.
 *   - an uninitialised `int result` assigned in the branches -- gcc
 *     cross-jumps the two `bl Proc_Find` sites and uses r4.
 *   - `int t = type;` comparing t and returning t -- gcc emits the copy but
 *     INVERTED (compares r0, restores `add r0, r1, #0` at the tail) and
 *     cross-jumps the calls.
 *   - `int t = type;` comparing t and returning `type` -- coalesced, no copy.
 *   - `switch (type)` with and without a default -- both compares hoisted to
 *     the top as a decision list, which the ROM does not do.
 *   - a local `ProcPtr proc` holding the call result, and `?:` instead of `!=`.
 *
 * The ROM's shape needs the parameter's pseudo to LOSE r0 to the return value
 * without becoming live across a call, and no source spelling tried reproduces
 * that. Note the fall-through returns the PARAMETER (r0 is untouched when type
 * is neither 1 nor 2), which is why `return type;` is written here rather than
 * `return 0;` -- a literal 0 costs a `movs r0, #0` the ROM does not have.
 */
int IsCoPowerScriptRunning(int type)
{
    register int t asm("r1") = type;
    if (t == 1)
        return Proc_Find(gUnknown_08616794) != NULL;

    if (t == 2)
        return Proc_Find(gUnknown_08616844) != NULL;

    return type;
}
asm(".global sub_08080EB0\n.thumb_set sub_08080EB0, IsCoPowerScriptRunning\n");
