#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802EB28.
 * sub_0802EB28 @ 0x0802EB28
 */

#include "hardware.h"

/* The link handshake's per-frame step. gUnknown_03000574 is the state: 0 opens
 * the port and waits for SI to go low, 1 waits for the id exchange, anything
 * else just re-reports the last result.
 *
 * Two spellings are load-bearing:
 *
 *   - `/ 16` and not `>> 4`. Both are one `asrs`, but they build the AND with
 *     the operands the other way round: `(v & 0x30) >> 4` ties the AND's
 *     output to v's register (`ands r1, r0; asrs r0, r1, #4`) while the
 *     division ties it to the mask's (`ands r0, r1; asrs r0, r0, #4`), which
 *     is what the ROM has. agbcc only reaches the bare shift because
 *     nonzero_bits proves `v & 0x30` non-negative.
 *   - `int v` and not `u16 v`. A u16 keeps the mask in HImode, which zero-
 *     extends the `v & 4` store and turns this `asrs` into an `lsrs`.
 *
 * gUnknown_0300055C and gUnknown_03000564 are both volatile; see their notes
 * in unknown-globals.h. Three of the reloads here are the only evidence in the
 * ROM that 0x03000564 is. */
int SioPollingMsg(void)
{
    int v;

    switch (gUnknown_03000574)
    {
    case 0:
        REG_RCNT = 0;
        REG_SIODATA8 = ~gUnknown_03003F6C->unk04;
        REG_SIOCNT = gUnknown_03000560 | 0x6000;
        v = REG_SIOCNT;
        if (v & 8)
        {
            gUnknown_0300055C = v & 4;
            if (gUnknown_0300055C != 0)
                gUnknown_0300055C = 0xF0;
            gUnknown_02023894 = 0;
            gUnknown_03000574 = 1;
        }
        break;

    case 1:
        v = REG_SIOCNT;
        if (gUnknown_02023894 != 0 && !(v & 0x40))
        {
            gUnknown_0300055C = (v & 0x30) / 16;
            gUnknown_03000564 &= ~3;
            gUnknown_03000564 |= 2;
            if ((gUnknown_03000564 & 0xC000) == 0xC000)
                gUnknown_03000570 = 2;
            gUnknown_03000574 = 2;
            return gUnknown_0300055C;
        }
        REG_SIODATA8 = ~gUnknown_03003F6C->unk04;
        if (gUnknown_0300055C != 0)
            REG_SIOCNT = gUnknown_03000560 | 0x6000;
        else
            REG_SIOCNT = gUnknown_03000560 | 0x6080;
        break;

    default:
        return gUnknown_0300055C;
    }

    return -1;
}
asm(".global sub_0802EB28\n.thumb_set sub_0802EB28, SioPollingMsg\n");
