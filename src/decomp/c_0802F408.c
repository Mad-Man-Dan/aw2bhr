#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F408.
 * sub_0802F408 @ 0x0802F408
 */

#include "hardware.h"

/* The link watchdog: it reads and clears the driver record's status halfword,
 * and either bumps or resets a stall counter depending on whether bit 3 is
 * clear in BOTH that status and REG_SIOCNT. It answers TRUE while the counter
 * is still under 11.
 *
 * EVERY REDUNDANT LOAD HERE IS THE VOLATILE TELL, not a missing CSE: the
 * `= 0` on unk02, the `++` on unk1f and the `= 0` on unk1f each carry a dead
 * load of the destination immediately in front of the store, which is what
 * settled unk00, unk01 and unk04 of this same struct in waves 30 and 31.
 *
 * BOTH TESTS ARE SPELLED AGAINST THE ROM'S BRANCH, not against the reading
 * that is easier to say. The first is `!(v & 8) && !(REG_SIOCNT & 8)` with the
 * `++` as the THEN arm, because agbcc puts the then arm inline and the ROM has
 * the `++` inline behind two `bne`s. The second is `> 0xa` returning FALSE with
 * TRUE in the `else`: agbcc REVERSES a condition whose arms are both `return`s,
 * so the spelling that reads `<= 0xa` first produces `bls` over the TRUE block
 * where the ROM has `bhi` over the FALSE one. Writing the test the other way
 * round is what puts them back. */
bool8 SioIsConnectionAlive(void)
{
    u16 v;

    v = gUnknown_0849B018->unk02;
    gUnknown_0849B018->unk02 = 0;

    if (!(v & 8) && !(REG_SIOCNT & 8))
        gUnknown_0849B018->unk1f++;
    else
        gUnknown_0849B018->unk1f = 0;

    if (gUnknown_0849B018->unk1f > 0xa)
        return FALSE;
    else
        return TRUE;
}
asm(".global sub_0802F408\n.thumb_set sub_0802F408, SioIsConnectionAlive\n");
