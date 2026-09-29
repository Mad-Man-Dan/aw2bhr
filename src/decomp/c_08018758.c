#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018758.
 * sub_08018758 @ 0x08018758
 */

/* A gUnknown_0200C528 list-script handler: allocates a gUnknown_03001470 slot
 * seeded from the script's own +0x14 halfword, arms it, and hands the script
 * over to EventCb_ClearWhenTextBoxesEnd.
 *
 * The +0x14 read is BOUND TO A LOCAL, and that is the whole difference between
 * this and a 4-byte-wrong candidate: written inline as the fourth argument it
 * is evaluated AFTER the gUnknown_08499588 deref, which puts that symbol's pool
 * word first. The local pulls the slot address chain to the front, which is
 * also where the shared `a * 0x18` product wants to be.
 *
 * `unk00 * 0x1000` for the fifth argument, not a shift: it reaches a `u16`
 * parameter, so the product is masked, and agbcc folds
 * `(x << 12) & 0xffff` into `lsls #0x1c; lsrs #0x10`.
 *
 * The byte at +0x38 of the slot is written through a cast: the halfword member
 * declared there is `strh`-written by four promoted files (c_080151B0.c,
 * c_08015E04.c, c_08029FC4.c, c_0802A690.c), so this `strb` of 1 touches only
 * its low half and the member is not retyped. `adds r0, #0x38` rather than a
 * displaced `strb` because 0x38 is past strb's 5-bit range. */
bool8 EventOp_ShowTextFromSlot(s16 a)
{
    struct Unk03001470 *s;
    u16 t;

    t = gUnknown_0200C528[a].unk14;
    s = StartTextBox(7, 1, gUnknown_08499588, t,
                     gUnknown_03002F08.unk00 * 0x1000, 0x100);
    s->unk3c = UploadEventTilemap;
    *(u8 *)&s->unk38 = 1;
    gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)EventCb_ClearWhenTextBoxesEnd;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08018758\n.thumb_set sub_08018758, EventOp_ShowTextFromSlot\n");
