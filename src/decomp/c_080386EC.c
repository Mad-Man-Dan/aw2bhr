#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080386EC.
 * sub_080386EC @ 0x080386EC
 */

/* MATCHED (wave 57, W57-C).
 *
 * Truncates the gUnknown_0849D5F8 move stack back to `a` entries and recomputes
 * the running fuel cost of every entry that survives. The loop body is ONE
 * statement and it is work/PushMovePathStep's body verbatim, with that function's
 * pushed (x, y) pair replaced by the stack's own unk2c[i] / unk20[i].
 *
 * The `?:` inside `costs` is a real branch, and that is why gUnknown_0849D5F8
 * is loaded TWICE in the body: CSE cannot carry the first load across it. The
 * `+ 1` on the gPlayers subscript rides in the 0x59 / 0x5a
 * displacements (0x3c + 0x1d, 0x3c + 0x1e), the same one-based indexing
 * work/PushMovePathStep and src/decomp/c_080211DC.c record for that array.
 *
 * TWO SPELLINGS ARE LOAD-BEARING, both isolated with compile_probe:
 *   - `(n = (s8)a)` INSIDE the comparison. Writing the obvious
 *     `unk45 = (s8)a - 1;` shortens through convert_to_integer into a QImode
 *     subtract on the RAW parameter, so agbcc has to copy r0 before the
 *     sign-extension overwrites it (`adds r4, r0, #0`, +2 bytes) and then
 *     emits a three-operand `subs r0, r4, #1`. The ROM subtracts from the
 *     EXTENDED value in place. Binding the extension to an int keeps the
 *     subtraction in SImode; binding it in its own statement instead puts the
 *     shifts ahead of the gUnknown_0849D5F8 load, so it has to happen inside
 *     the compare.
 *   - `p` and `idx` in the loop body, IN THAT ORDER, before `stack`. The ROM
 *     loads the struct pointer, sign-extends i, and only then does
 *     `adds rB, #0x38`; without `idx` the `+0x38` lands ahead of the shifts.
 *     `p` must NOT be reused for the unk2c/unk20 reads below the `?:` -- the
 *     ROM reloads gUnknown_0849D5F8 there, because the branch broke the CSE.
 *
 * The pool literals 0x1432 and 0x417A are member offsets on the
 * gUnknown_08499590 map, not addresses: too large for a THUMB 8-bit immediate,
 * so agbcc parks each in the pool and adds it to the map base. The local
 * struct + cast is what preserves the `(map + K) + idx` association; see the
 * gUnknown_08499590 comment in include/unknown-globals.h.
 *
 * trymatch reports `relocs: name different symbols that resolve to the same
 * address` at +0x148 (gUnknown_085D3E20 vs gUnknown_085D3DD0+0x50) -- the
 * documented false mismatch; 0x085D3E20 IS
 * &gUnknown_085D3DD0[0].unk38[0].unk18[0] (0x38 + 0x18 == 0x50). */
void TruncateMovePath(int a)
{
    struct Unk0849D5F8 *p;
    struct Map *map;
    s8 *costs;
    s8 *stack;
    s8 *cur;
    s8 *prev;
    s8 i;
    int n;
    int idx;
    int c;

    if (gUnknown_0849D5F8->unk45 < (n = (s8)a))
        return;

    gUnknown_0849D5F8->unk45 = n - 1;
    gUnknown_0849D5F8->unk38[gUnknown_0849D5F8->unk45] = gUnknown_0849D5F8->unk38[0xc];

    for (i = 1; i <= gUnknown_0849D5F8->unk45; i++)
    {
        p = gUnknown_0849D5F8;
        idx = i;
        stack = p->unk38;
        cur = &stack[idx];
        prev = &stack[idx - 1];

        costs = gUnknown_085D3DD0[gPlaySt.coAbilities
                    ? gPlayers[(gUnknown_03003F38 >> 6) + 1].co
                    : 1]
                .power[gPlayers[(gUnknown_03003F38 >> 6) + 1].coMode]
                .movementChart[gPlaySt.weather];

        map = gMap;

        c = (map->terrain[map->rowOffset[gUnknown_0849D5F8->unk2c[i]]
                        + gUnknown_0849D5F8->unk20[i]] & 0x1f)
            + gUnknown_085D5ABC[gUnknown_030040D8->unk00].movementType * 32;

        *cur = *prev - costs[c];
    }
}
asm(".global sub_080386EC\n.thumb_set sub_080386EC, TruncateMovePath\n");
