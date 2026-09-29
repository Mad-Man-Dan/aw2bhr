#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801348C.
 * sub_0801348C @ 0x0801348C, sub_08013510 @ 0x08013510
 */

#include "hardware.h"

/* The key-state update: `keys` is the raw pad bitmask (RefreshKeySt hands it
 * `~REG_KEYINPUT & 0x3ff`), `st` is gUnknown_03002090. Opposing D-pad pairs
 * cancel first (0x30 = Left|Right, 0xc0 = Up|Down).
 *
 * The parameter is s16 and that is readable: entry is ONE `lsls #0x10` feeding
 * BOTH an `lsrs` (PROMOTE_MODE's zero-extension of the incoming halfword) and
 * an `asrs` (the sign-extension `keys & 0x30` needs to read it as int). A u16
 * parameter has no `asrs` at all.
 *
 * But every use AFTER the two cancels reads the value back out of ->unk08
 * rather than out of `keys`: the ROM's `bics`, both compares and the repeat
 * store all run off the ZERO-extended register, which CSE supplies from the
 * `strh` two instructions earlier. Spelling those uses as `keys` costs an
 * `lsls`/`asrs` pair each, because `keys` is signed and ->unk08 is not -- that
 * is the whole difference between a near-miss and this.
 *
 * The `ldr r0,[r0]` shared by both repeat-reload arms is agbcc merging the
 * common tail of an if/else whose arms differ only in which global they name.
 */
void RefreshKeyStFromKeys(struct Unk03002090 *st, s16 keys)
{
    if ((keys & 0x30) == 0x30)
        keys ^= 0x30;

    if ((keys & 0xc0) == 0xc0)
        keys ^= 0xc0;

    st->unk0e = st->unk08;
    st->unk08 = keys;
    st->unk0a = st->unk08 & ~st->unk0e;
    st->unk0c = st->unk08 & ~st->unk0e;

    if (st->unk08 != 0 && st->unk08 == st->unk0e)
    {
        if (--st->unk10 == 0)
        {
            st->unk0a = st->unk08;
            st->unk10 = gUnknown_03002F94;
        }
    }
    else
    {
        st->unk10 = gUnknown_030030C8;
    }

    st->unk00 = st->unk08;
    st->unk04 = st->unk0c;
    st->unk02 = st->unk0a;
    st->unk06 = st->unk0e;
}
asm(".global sub_0801348C\n.thumb_set sub_0801348C, RefreshKeyStFromKeys\n");

/* The once-per-frame key poll. Normally reads the pad, but when
 * gPlaySt.unk32 selects the replay mode AND IsLinkKeySyncMode reports 1
 * the mask comes out of the replay stream instead (sub_080303C8) and
 * LinkApplyKeySyncToKeySts advances it.
 *
 * `v` is a u16 local, not two separate calls to RefreshKeyStFromKeys: the ROM's TWO
 * copies of the 0x3ff pool word (`adds r0,r2,#0; adds r4,r0,#0`) before the
 * `bics` are what a merged variable costs -- r4 is written in both arms, so the
 * constant cannot be ANDed into it in place. The single-arm spelling in
 * AgbMain's own `~REG_KEYINPUT & 0x3ff` has one copy, not two.
 */
void RefreshKeySt(void)
{
    u16 v;

    if (gPlaySt.savingEnabled != 0 && IsLinkKeySyncMode() == 1)
    {
        v = sub_080303C8();
        LinkApplyKeySyncToKeySts();
    }
    else
    {
        v = ~REG_KEYINPUT & 0x3ff;
    }

    RefreshKeyStFromKeys(&gUnknown_03002090, v);
}
asm(".global sub_08013510\n.thumb_set sub_08013510, RefreshKeySt\n");
