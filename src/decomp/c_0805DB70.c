#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805DB70.
 * sub_0805DB70 @ 0x0805DB70
 */

/* AiDeliberateCoPower @ 0x0805DB70, 308 bytes.
 *
 * gUnknown_0816DA4C is not an object: the ROM word at 0x0816DA4C holds
 * 0x030033EC, agbcc's own -fforce-addr address constant for
 * gUnknown_030033EC. Naming the global honestly reproduces the double load.
 *
 * `n` is ONE local assigned twice -- the terrain difference and then Div's
 * quotient. That is what the `adds r3, r0, #0` after the `bl Div` records: a
 * pseudo whose live range already covers the whole address chain, so the chain
 * gets r0-r2 instead of r1-r3. Two separate locals coalesce the copy away and
 * shift every register in the chain by one.
 *
 * gUnknown_085D3DD0[..].unk1c is a function pointer called through
 * `bl _call_via_r1`, and the veneer's register index is what fixes its arity at
 * one argument.
 */
void AiDeliberateCoPower(void)
{
    u8 (*fn)(int);
    int v;
    int n;

    if ((u8)IsSuperCoPowerReady(gUnknown_030033EC))
    {
        fn = gUnknown_085D3DD0[gPlayers[gUnknown_030033EC].co].unk1c;
        if (fn != 0 && fn(2) == 1)
        {
            if (gPlaySt.savingEnabled != 0)
                SendActionCommand(0x10, 0, gUnknown_030033EC, 0);
            PayForCoPower(gUnknown_030033EC, 2);
            return;
        }
    }

    if (IsCoPowerReady(gUnknown_030033EC))
    {
        v = GetCoPowerCost(gUnknown_030033EC);
        n = GetSuperCoPowerCost(gUnknown_030033EC) - v;
        n = Div(n * gUnknown_085D3DD0[gPlayers[gUnknown_030033EC].co].unk17,
                100);
        v += n;
        if (v >= (int)GetCoPowerCharge(gUnknown_030033EC)
            && GetNextRandomNumber() % 100
                   < gUnknown_085D3DD0[gPlayers[gUnknown_030033EC].co].unk18)
        {
            fn = gUnknown_085D3DD0[gPlayers[gUnknown_030033EC].co].unk1c;
            if (fn != 0 && fn(1) == 1)
            {
                if (gPlaySt.savingEnabled != 0)
                    SendActionCommand(0xf, 0, gUnknown_030033EC, 0);
                PayForCoPower(gUnknown_030033EC, 1);
            }
        }
    }
}
asm(".global sub_0805DB70\n.thumb_set sub_0805DB70, AiDeliberateCoPower\n");
