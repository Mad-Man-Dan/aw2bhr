#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035080.
 * sub_08035080 @ 0x08035080
 */

/* MATCHED -- wave 59 (W59-B), 100/100 bytes, exit 0 from try_match.
 *
 * PROMOTION NOTE: the two pool words are this unit's own .rodata force-addr
 * words. try_match reports `relocs: name different symbols that resolve to the
 * same address`, which is a match. This function's data/promoted.json entry
 * needs   "rodata": ["0x08090E34", "0x08090E38"]   and then
 * tools/split_rodata.py + tools/gen_lds.py before the build.
 *
 * WHAT CLOSED IT, and it refutes the wave-51 note that used to be in this file.
 * Wave 51 recorded the two halves of this function as MUTUALLY EXCLUSIVE: the
 * plain array form `gUnknown_03004490[3 + i]` gave the ROM's two-level
 * force-addr word and its per-iteration reload but put the `+3` on the INDEX,
 * while the struct-cast form put the `+3` on the BASE but produced a one-level
 * pool word and let LICM hoist `base + 3` out of the loop (-8 bytes). That is
 * true only for a BOTTOM-TESTED loop. It is not a property of the two spellings.
 *
 * The real axis was the loop's ENTRY JUMP, as the wave-57 note guessed. Written
 * as `for (i = 1; i <= 4; i++)`, agbcc rotates the loop, proves the first
 * `i <= 4` true and deletes the entry `b`; the pool then moves from 0x24 to 0x44
 * and `base + 3` becomes loop-invariant, so it is hoisted. Written with the
 * entry `goto` below, the test stays at the TOP of the loop, the increment block
 * sits before it, the pool lands at 0x24 -- and `base + 3` is no longer
 * invariant-hoistable, so the struct-cast form keeps BOTH the two-level word and
 * the +3-on-base. One change, both halves. That took the draft from 96 bytes /
 * 23.0% to 100 bytes / 93.0% with the instruction stream exact.
 *
 * The remaining 7 bytes were a three-way rotation of the callee-saved registers
 * ({v, &arr word, &idx word} = {r5,r7,r6} against the ROM's {r7,r6,r5}), i.e.
 * order-correct and slot-wrong. decomp-permuter closed it in 5,392 iterations,
 * which is the case wave 37 lists as the permuter's own.
 *
 * DO NOT SIMPLIFY THE `do { i = 1; goto _test; } while (0);` LINE. It is the
 * permuter's edit and it is load-bearing: the wrapper emits its own loop notes,
 * and removing it is the 93.0% draft, not this one. The `goto` into the `for`
 * body is what holds the layout and cannot be removed either.
 */

struct Unk03004490
{
    /* 0x00 */ u8 filler_00[0x03];
    /* 0x03 */ u8 unk03[5];
};

u8 TickWeatherRoundCounter(void)
{
    u8 v;
    u8 i;

    v = ++((struct Unk03004490 *)gUnknown_03004490)->unk03[gUnknown_030033EC];

    do { i = 1; goto _test; } while (0);

    for (;;)
    {
        i++;
    _test:
        if (i > 4)
        {
            return 1;
        }

        if (i == gUnknown_030033EC)
        {
            continue;
        }
        if (!IsPlayerAliveAndActive(i))
        {
            continue;
        }
        if (v == ((struct Unk03004490 *)gUnknown_03004490)->unk03[i])
        {
            continue;
        }
        return 0;
    }
}
asm(".global sub_08035080\n.thumb_set sub_08035080, TickWeatherRoundCounter\n");
