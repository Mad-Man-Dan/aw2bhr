#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034534.
 * SendActionCommand @ 0x08034534, sub_08034598 @ 0x08034598
 */

#include "hardware.h"

/* The full builder of the LinkQueueCommand command block: sub_080344B4 is this one
 * with the id fixed at 8 and the +2..+5 cursor snapshot dropped, and
 * sub_0803446C is the +2..+5 snapshot on its own. The store order is the
 * source's -- +0 first, then the two cursor pairs, then +1/+6/+7 -- and it is
 * not reorderable, which is what fixes this as one statement per line. */
void SendActionCommand(int a, u8 b, u8 c, u8 d)
{
    struct Unit *unit = &gUnits[b];

    gUnknown_030044B0[0] = a;
    gUnknown_030044B0[2] = gUnknown_03003100.pos.unk00;
    gUnknown_030044B0[3] = gUnknown_03003100.pos.unk02;
    gUnknown_030044B0[4] = gUnknown_03003F24.pos.unk00;
    gUnknown_030044B0[5] = gUnknown_03003F24.pos.unk02;
    gUnknown_030044B0[1] = b;
    gUnknown_030044B0[6] = c;
    gUnknown_030044B0[7] = d;
    gUnknown_030044B0[0x12] = unit->fuel;
    PackPathNibbles(gUnknown_03003110, gUnknown_030044B0 + 0xc);
    LinkQueueCommand(gUnknown_030044B0);
}

/* The shared per-frame tail of sub_08034394 and sub_080343D8. */
void sub_08034598(void)
{
    HandleMoveMapCursor();
    sub_08023518();
    HandleMoveCameraWithMapCursor(4);

    if (gpKeySt->pressed & 2)
        sub_0802FA64();

    sub_0802776C(3);
}

asm(".global sub_08034534\n.thumb_set sub_08034534, SendActionCommand\n");
