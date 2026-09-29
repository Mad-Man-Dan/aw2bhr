#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026704.
 * sub_08026704 @ 0x08026704, sub_0802672C @ 0x0802672C, sub_08026768 @ 0x08026768
 */

/* Advances a player/army slot index and wraps 5 -> 1, returning the first slot
 * for which IsPlayerAliveAndActive is true. A do/while: the increment happens before the
 * first test, so the value passed in is never itself tested. The wrap compares
 * `== 5` exactly, so a value above 5 never wraps -- reproduced as written. */
u16 GetNextActiveArmy(int a)
{
    u16 i;

    i = a;
    do
    {
        i++;
        if (i == 5)
            i = 1;
    } while (!IsPlayerAliveAndActive(i));

    return i;
}
asm(".global sub_08026704\n.thumb_set sub_08026704, GetNextActiveArmy\n");

/* The GetNextActiveArmy twin (see work/GetNextActiveArmy): the same wrap-at-5 retry over
 * IsPlayerAliveAndActive, but seeded from gUnknown_030033EC instead of a parameter, and
 * answering whether the slot it landed on is EARLIER than the one it started
 * from -- i.e. whether the search wrapped past the end.
 *
 * gUnknown_08090A70 in the asm is NOT a global: the ROM word at 0x08090A70
 * holds 0x030033EC, so it is agbcc's own -fforce-addr address constant for
 * gUnknown_030033EC.  Note this block also contains the OTHER case --
 * gUnknown_08090A84 is a genuine ROM byte table -- so the 0x0809xxxx prefix
 * decides nothing on its own; check baserom.gba per symbol.
 *
 * The global is re-read after the loop rather than reused because the
 * IsPlayerAliveAndActive call clobbers it, and the compare is `blo`, unsigned.
 * The `movs #0` / `movs #1` split across an unconditional `b` is the
 * if/else-return spelling, not a returned comparison. */
bool8 IsCurrentArmyLastInTurnOrder(void)
{
    u16 i;

    i = gUnknown_030033EC;

    do
    {
        i++;
        if (i == 5)
            i = 1;
    } while (!IsPlayerAliveAndActive(i));

    if (i < gUnknown_030033EC)
        return 1;
    else
        return 0;
}
asm(".global sub_0802672C\n.thumb_set sub_0802672C, IsCurrentArmyLastInTurnOrder\n");

/* The same wrap-at-5 retry as IsCurrentArmyLastInTurnOrder, but the counter IS the global:
 * gUnknown_030033EC is incremented in memory each pass, and reaching 5 calls
 * IncrementDayCount and resets it to 1.
 *
 * The `ldrb r0,[r4]` feeding IsPlayerAliveAndActive is agbcc narrowing the u16 global's
 * load to that callee's declared `u8` parameter, not a separate byte field --
 * the same halfword is read `ldrh` two instructions earlier.
 *
 * Here the pool word is an ordinary inline `=gUnknown_030033EC`, where
 * IsCurrentArmyLastInTurnOrder's is a -fforce-addr .rodata word for the same symbol. */
void AdvanceToNextActiveArmy(void)
{
    do
    {
        gUnknown_030033EC++;

        if (gUnknown_030033EC == 5)
        {
            IncrementDayCount();
            gUnknown_030033EC = 1;
        }
    } while (!IsPlayerAliveAndActive(gUnknown_030033EC));
}
asm(".global sub_08026768\n.thumb_set sub_08026768, AdvanceToNextActiveArmy\n");
