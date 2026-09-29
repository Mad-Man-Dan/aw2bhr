#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080254AC.
 * sub_080254AC @ 0x080254AC
 */

/* MATCHED (wave 39, W39-D). Needs its .rodata pool word PLACED at promotion:
 *
 *   "rodata": ["0x08090A4C"]
 *
 * then tools/split_rodata.py and tools/gen_lds.py before building. trymatch
 * reports `relocs: name different symbols that resolve to the same address`
 * for it, which is the wave-18 honest-spelling flow and not a difference in
 * the ROM. gUnknown_08090A4C is agbcc's own -fforce-addr word holding
 * &gUnknown_030032C0 (already documented in include/unknown-globals.h's
 * 0x080909E8-0x08090A5C run), so the source names gUnknown_030032C0 directly
 * and the build places the word.
 *
 * Two passes over the same 12-byte gUnits records, and the shape of
 * each bound is what fixes the types:
 *   - pass 1 counts `i <= 0x32` against a CONSTANT, so the compare is unsigned
 *     (`bhi`/`bls`) and the `lsls #16; lsrs #16` after every increment is the
 *     u16 counter truncating -- i is u16.
 *   - pass 2 counts against `(s16)gUnknown_030032C0 + 1`, an int, so the same
 *     u16 counter is compared SIGNED (`bge`/`blt`). One variable, two compare
 *     kinds, both falling out of the bound's type. The `ldrsh` with a register
 *     offset is the (s16) cast on the u16 global, per the wave-37 rule that
 *     sign_extend (mem:HI) always folds to ldrsh.
 *
 * The bound in pass 2 is re-read at the bottom of every iteration because the
 * hit path STORES to gUnknown_030032C0 and that kills the load -- do not hoist
 * it. The same mechanism re-reads gUnknown_03003F2C in the shared tail.
 *
 * Both passes spell the hit path and the `return` in full; jump.c cross-jumps
 * the two into the single shared tail block at 0x0802554A that recomputes
 * `&gUnits[i + gUnknown_03003F2C]`. Do not try to author that block.
 *
 * `gUnknown_030032C0 = 0` on the out-of-range branch compiles to `strh r3,[r1]`
 * reusing the flag test's AND result, which is 0 on that path -- ordinary CSE,
 * not a spelling.
 */

struct Unit *GetNextReadyUnit(void)
{
    u16 i;

    for (i = gUnknown_030032C0 + 1; i <= 0x32; i++)
    {
        if (gUnits[i + gUnknown_03003F2C].type != 0
            && !(gUnits[i + gUnknown_03003F2C].flags & 1))
        {
            gUnknown_030032C0 = i;
            if ((s16)i > 0x31)
                gUnknown_030032C0 = 0;
            return &gUnits[i + gUnknown_03003F2C];
        }
    }

    for (i = 1; i < (s16)gUnknown_030032C0 + 1; i++)
    {
        if (gUnits[i + gUnknown_03003F2C].type != 0
            && !(gUnits[i + gUnknown_03003F2C].flags & 1))
        {
            gUnknown_030032C0 = i;
            if ((s16)i > 0x31)
                gUnknown_030032C0 = 0;
            return &gUnits[i + gUnknown_03003F2C];
        }
    }

    return 0;
}
asm(".global sub_080254AC\n.thumb_set sub_080254AC, GetNextReadyUnit\n");
