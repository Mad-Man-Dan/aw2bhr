#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064500.
 * sub_08064500 @ 0x08064500
 */

/* MATCHED, wave 46 (W46-K). 172/172 bytes.
 *
 * ON PROMOTION this function needs, like its siblings DrawUpArrow and
 * RulesScreenDrawUpArrow:
 *     "rodata": ["0x0816E0B8", "0x0816E0BC"]
 *
 * Those two words are agbcc's own -fforce-addr address-constant pool, NOT
 * globals the source names. The splitter invented the two gUnknown_<addr>
 * names for them; the matched sibling DrawUpArrow is promoted with the
 * adjacent pair 0x0816E0B0 / 0x0816E0B4 from the same run. try_match reports
 * `relocs: name different symbols that resolve to the same address`, which is
 * the documented false-mismatch case, not a defect. Do NOT go looking for a
 * spelling that names gUnknown_0816E0B8 -- the declaration that exists at
 * include/unknown-globals.h:14117 is a splitter artefact, and referencing it
 * would be wrong.
 *
 * WHAT CLOSED IT, after three attempts across waves 33/34 stalled at 97.1%:
 * the last four bytes were `lsls r0,r0,#1` and `ldr r1,[pc,#48]` transposed --
 * the ROM scales the DivRem result BEFORE loading the table base.
 *
 * Waves 33/34 looked for an OPERAND-ORDER lever, and there is none to find.
 * The C front end's build_binary_op rewrites BOTH `ptr + int` and `int + ptr`
 * through pointer_int_sum(ptr, int) before any tree survives, so
 * `gUnknown_08239F84[i]`, `i[gUnknown_08239F84]`, `&gUnknown_08239F84[i]`,
 * `gUnknown_08239F84 + i` and `i + gUnknown_08239F84` are all the SAME tree,
 * and expand_expr does op0 (the base) before op1. Wave 34's `i[arr]` probe was
 * correct and its conclusion -- "the commutation is done in the front end" --
 * was right; what it missed is that the fix is not an operand order at all.
 *
 * The lever is a STATEMENT BOUNDARY, and it has to fall on the SCALED value.
 * Wave 34 bound the raw DivRem result to an `int` and correctly reported no
 * change: the `lsls` then still sits in the same statement as the `ldr` and
 * still loses the race. Binding `result * 2` puts the shift in the earlier
 * statement, and the base load lands after it.
 */
void DrawDownArrow(int a1, int a2)
{
    int i;

    if (gUnknown_03000604 == gGameClock - 1)
    {
        a1 = (gUnknown_03000608 + a1) / 2;
        a2 = (gUnknown_0300060A + a2) / 2;
    }

    DrawOamObject(0x44, a1 & 0x1ff, a2 & 0xff, 0, 0);

    gUnknown_03000604 = gGameClock;
    gUnknown_03000608 = a1;
    gUnknown_0300060A = a2 + 1;

    i = DivRem(Div(gGameClock, 4), 0x10) * 2;
    ApplyPaletteExt((u16 *)((u8 *)gUnknown_08239F84 + i), 0x358, 2);
}
asm(".global sub_08064500\n.thumb_set sub_08064500, DrawDownArrow\n");
