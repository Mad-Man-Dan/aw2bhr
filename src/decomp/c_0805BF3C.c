#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805BF3C.
 * sub_0805BF3C @ 0x0805BF3C, sub_0805BFDC @ 0x0805BFDC
 */

/* Byte 0x09 of the gUnknown_030040D8 record is written here as a 3-BIT
 * BITFIELD at bits 3..5, not as a hand-rolled mask: `movs r0,#7; ands r5,r0;
 * lsls r2,r5,#3` ahead of the member load, then a clear with -0x39
 * (`movs r0,#0x39; rsbs r0,r0,#0`) -- store_bit_field's shape, and exactly the
 * insert that include/unknown-globals.h documents for struct Unk02028360's
 * 3-bit fields. Spelling it as `m = (m & ~0x38) | ((d & 7) << 3)` on the
 * declared `u8 unk07[5]` gives `mov r0,#0xc7` instead, because
 * shorten_binary_op narrows the AND to QImode, and it also emits the member
 * load BEFORE the shift. The byte is inside struct Unk030040D8's unk07[] and
 * three promoted functions read that array, so the bitfield view is kept
 * FILE-LOCAL rather than reshaping the shared declaration.
 *
 * The same map byte is loaded TWICE, once as `ldrb; lsls #0x18; asrs #0x18`
 * and once as `movs r2,#0; ldrsb r2,[r0,r2]`; that is two source reads of one
 * expression, not a CSE. Both divisions are SIGNED (`__divsi3`).
 *
 * The final compare is a HImode one: `lsls #0x10` on BOTH operands with no
 * `asrs` and a signed `ble`. That is shorten_compare on two `(s16)`-typed
 * operands -- the sign-extension halves fold into the comparison and only the
 * left shifts survive. The `lsls #0x10; lsrs #0x10` on the first quotient is
 * separate: it is that value's conversion to the `u16` local. */

struct Unk5BFDCCtl
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 3;
               u8 unk09_6 : 2;
};

/* The 25-byte zeroed stack table is a per-terrain dispatch index: sub_0808B6C4
 * is the tree's memset and only 0x19 of the 0x1c bytes the frame reserves are
 * cleared. Four entries are then overwritten, and agbcc walks the address
 * incrementally (+1, +1, +5) because CSE rewrites each constant stack address
 * as an offset from the previous one -- plain array subscripts produce that on
 * their own, no pointer spelling required.
 *
 * The two `_call_via_r3` sites are NOT cross-jumped, so this has to be written
 * as two call statements rather than one call after an if/else that picks the
 * pointer. gUnknown_030046D4 is volatile, which is why the index is re-read
 * from memory in the else arm instead of reusing the byte just stored. */

void sub_0805BF3C(int x, int y, u16 *out)
{
    u8 buf[28];
    struct Unit *e;

    sub_0808B6C4(buf, 0, 0x19);
    buf[1] = 8;
    buf[2] = 8;
    buf[7] = 8;
    buf[9] = 9;

    e = &gUnits[gUnknown_030040D8->unk07[1]];
    gUnknown_030046D4 = e->type;

    if (gUnknown_030040D8->unk07[4] > 7)
        gUnknown_030040D8->unk07[4] = 1;

    if (e->unk0b != 0)
        gUnknown_085768B8[e->unk0b](x, y, out);
    else
        gUnknown_085768B8[buf[gUnknown_030046D4]](x, y, out);
}

void AiTryRideInsteadOfWalk(int x, int y, int a3, int a4)
{
    int v;
    u16 t;

    v = (s8)gUnknown_03003340[y][x];
    if (v <= GetUnitMovementWithCoBonus(gUnknown_030033EC, gUnknown_030040D8->unk00) * 2)
        return;

    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03, a3,
                      0x78, 0);
    MapMarkHalo(0x79);

    t = ((s8)gUnknown_03003340[y][x] * 3) / GetUnitMovementWithCoBonus(gUnknown_030033EC, a3);

    if ((s16)((v * 4) / GetUnitMovementWithCoBonus(gUnknown_030033EC, gUnknown_030040D8->unk00))
        <= (s16)t)
        return;

    ((struct Unk5BFDCCtl *)gUnknown_030040D8)->unk09_3 = a4;
    AiBoardTransport();
}
asm(".global sub_0805BFDC\n.thumb_set sub_0805BFDC, AiTryRideInsteadOfWalk\n");
