#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034938.
 * sub_08034938 @ 0x08034938
 */

/* MATCHED byte-for-byte, wave 50 (W50-J).
 *
 * The guard is a short-circuit `||` chain with the FAILURE arm written first:
 * each true condition branches forward onto the `gUnknown_030032D8 = 2` block,
 * and the last condition (`!=`) branches onto the else. Written as
 * `if (a && b && c) {...} else {...}` the two arms come out the other way
 * round. The shared `strh r0,[r1]` tail is agbcc cross-jumping the two stores
 * to gUnknown_030032D8, which is why the pool word for it appears twice.
 *
 * gPlaySt.unk30 is loaded through an address held in r4 rather than
 * a displacement because +0x30 is outside `ldrb`'s imm5 range (max 31).
 *
 * FIRST LOOP NEEDS THE `v` BINDING, SECOND ONE MUST NOT HAVE IT. Written as
 * one `&&` chain the first loop reloads `unk11` for the assignment
 * (`ldrb r1,[r0,#0x11]` ... `ldrb r5,[r0,#0x11]`); the ROM loads once into r0
 * and copies with `adds r5,r0,#0`. The second loop has no assignment, so the
 * single load is already what the `&&` produces and adding a local there
 * changes nothing. That asymmetry is the whole difference between this and a
 * near-miss.
 *
 * `gPlayers` is the pointer-to-array global, so the plain `[i]`
 * subscript is what emits the `ldr rN,=sym; ldr rN,[rN]` pair -- see its note
 * in unknown-globals.h; it is on wave 50's pool-word ALLOWLIST and is NOT a
 * -fforce-addr word. `(i*16 - i)*4` is agbcc's 60-byte stride multiply. */

void MapState_CheckTurnLimit(void)
{
    u8 i;
    u8 best;

    if (gPlaySt.turnLimit == 0 || !IsCurrentArmyLastInTurnOrder()
        || gPlaySt.turnLimit != gUnknown_03004080)
    {
        gUnknown_030032D8 = 2;
    }
    else
    {
        best = 0;

        for (i = 1; i <= 4; i++)
        {
            if (IsPlayerAliveAndActive(i))
            {
                u8 v = gPlayers[i].captures;

                if (best < v)
                    best = v;
            }
        }

        for (i = 1; i <= 4; i++)
        {
            if (IsPlayerAliveAndActive(i) && best > gPlayers[i].captures)
                RecordArmyDefeat(i, 0x20);
        }

        FinalizeMatchResult();
        gUnknown_030032D8 = 0x12;
    }
}
asm(".global sub_08034938\n.thumb_set sub_08034938, MapState_CheckTurnLimit\n");
