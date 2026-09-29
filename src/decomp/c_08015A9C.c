#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015A9C.
 * sub_08015A9C @ 0x08015A9C, sub_08015B94 @ 0x08015B94
 */

/* MATCHED.
 *
 * Collect every pending slot (unk00 set and unk12 bit 0 set) into a 30-byte
 * stack list, selection-sort the list ascending by `unk14 & 0x7f`, then run
 * each one through RunSlotScriptFrame -- re-testing unk00, because an earlier
 * RunSlotScriptFrame may have torn its slot down.
 *
 * Frame is 0x28: list[30] rounded to 32, plus the two 4-byte slots the sort
 * loop spills at +0x20 (i + 1) and +0x24 (count - 1).
 *
 * THE LOCAL DECLARATION ORDER IS LOAD-BEARING AND IS THE WHOLE LAST 4 BYTES.
 * With `list, count, i, k, t` the two spill slots come out SWAPPED (count - 1
 * at +0x20, i + 1 at +0x24) and nothing else in 248 bytes differs -- 98.4%,
 * size exact, same instructions in the same order. Declaring `count` after `k`
 * flips them. gcc assigns spill slots in ascending pseudo-register number and
 * named locals are numbered in declaration order, so moving a declaration is
 * the direct lever on which spilled value lands in which slot. Found by
 * decomp-permuter in 55 iterations; the permuter changed NOTHING ELSE.
 * Written up in docs/agbcc-codegen.md.
 *
 * Three other things that had to be right and were not obvious:
 *   - `for (i = 0, count = 0; ...)`, not `count = 0;` on its own line. The
 *     chained form materialises the zero once, in i's register, and copies it
 *     (`movs r4,#0; mov sl,r4`); the separate statement takes three
 *     instructions.
 *   - `list[count] = i; count++;` as two statements. `list[count++] = i`
 *     computes and narrows the increment BEFORE the store and reorders four
 *     instructions.
 *   - The swap must be the plain `t = list[i]; list[i] = list[k]; list[k] = t;`
 *     with the comparison indexing `list[i]`/`list[k]` directly. Binding
 *     `x = list[i]; y = list[k];` ahead of the compare emits both list loads
 *     first where the ROM interleaves each with its own `unk14` read.
 *
 * One variable serves all three loops. A separate `j` for the sort's outer
 * loop puts it in the caller-saved r2 instead of the ROM's r4; `i` is live
 * across `bl RunSlotScriptFrame` in the last loop, which is what forces the
 * callee-saved register the sort loop also uses.
 */
void RunPendingSlotScripts(void)
{
    u8 list[30];
    u8 i;
    u8 k;
    u8 count;
    u8 t;

    for (i = 0, count = 0; i < 30; i++)
    {
        if (gUnknown_03001470[i].unk00 != 0
            && (gUnknown_03001470[i].unk12 & 1))
        {
            list[count] = i;
            count++;
        }
    }

    if (count == 0)
        return;

    if (count != 1)
    {
        for (i = 0; i < count - 1; i++)
        {
            for (k = i + 1; k < count; k++)
            {
                if ((gUnknown_03001470[list[i]].unk14 & 0x7f)
                    > (gUnknown_03001470[list[k]].unk14 & 0x7f))
                {
                    t = list[i];
                    list[i] = list[k];
                    list[k] = t;
                }
            }
        }
    }

    for (i = 0; i < count; i++)
    {
        if (gUnknown_03001470[list[i]].unk00 != 0)
            RunSlotScriptFrame(list[i]);
    }
}
asm(".global sub_08015A9C\n.thumb_set sub_08015A9C, RunPendingSlotScripts\n");

/* MATCHED.
 *
 * "Is any slot still pending?" -- the loop half of RunAllSlotScripts's and
 * sub_08015994's `do/while` condition. Structurally c_08015BD0.c's scan with a
 * different predicate.
 *
 * `movs r0,#1; b` inside the loop and `movs r0,#0` on fallthrough is the
 * two-return spelling, not a returned comparison: a returned `!=` would go
 * through do_store_flag and arrive with no unconditional branch.
 *
 * bool8 rather than u8: both callers narrow the result `lsls r0,r0,#0x18` and
 * only test it.
 */
bool8 AnySlotScriptPending(void)
{
    u8 i;

    for (i = 0; i < 30; i++)
    {
        if (gUnknown_03001470[i].unk00 != 0
            && (gUnknown_03001470[i].unk12 & 1))
            return TRUE;
    }

    return FALSE;
}
asm(".global sub_08015B94\n.thumb_set sub_08015B94, AnySlotScriptPending\n");
