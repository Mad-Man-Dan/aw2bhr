#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08042998.
 * sub_08042998 @ 0x08042998
 */

#include "map.h"

/*
 * sub_08042998 -- join the active unit with the unit on the target tile.
 *
 * The active unit is gUnknown_030040D8; the other unit is the one on the
 * tile at gUnknown_03003100.pos. The two are merged into the active unit:
 *   - HP: each unit's displayed HP (hp / 10, rounded up) is added. Anything
 *     above 10 is passed to sub_08025B58 for the current player, multiplied
 *     by sub_08042C9C's per-type value -- most likely the funds refund the
 *     game gives for excess HP. HP is then set to the total * 10.
 *   - unk05_3 is copied from the other unit; unk06_7 keeps the larger value.
 *   - Ammo and fuel are added and capped at the unit type's maximum.
 *   - With saving disabled and fog off, sub_08025B80 is called with
 *     gUnknown_03004074, which is then cleared.
 * Finally the other unit is removed (type = 0) and sub_080424E4 is called.
 *
 * Why the C looks odd:
 *   - The second HP term is a `?:` expression, and the ammo total is a u8
 *     `sum`. Both change which values the compiler reloads; the original
 *     reads maxAmmo again for the cap, which only this spelling reproduces.
 *   - Ammo goes through the local `q`, but fuel is written through
 *     gUnknown_030040D8 directly. Using `q` for both does not match.
 */

void sub_08042998(void)
{
    struct Unit *u;
    struct Unit *q;
    int a;
    u8 n;
    u16 m;
    u8 sum;

    u = &gUnknown_08499594[((struct Map *)gUnknown_08499590)->unitUnk[((struct Map *)gUnknown_08499590)->rowOffset[gUnknown_03003100.pos.unk02] + gUnknown_03003100.pos.unk00]];
    if (u->hp != 0)
        a = Div(u->hp - 1, 10) + 1;
    else
        a = 0;

    n = ((struct Unit *)gUnknown_030040D8)->hp != 0 ? a + 1 + Div(((struct Unit *)gUnknown_030040D8)->hp - 1, 10) : a;

    if (n > 10)
    {
        m = gUnknown_030033EC;
        sub_08025B58(m, sub_08042C9C(m, ((struct Unit *)gUnknown_030040D8)->type) * (n - 10));
        n = 10;
    }

    ((struct Unit *)gUnknown_030040D8)->hp = n * 10;
    ((struct Unit *)gUnknown_030040D8)->unk05_3 = u->unk05_3;
    if (((struct Unit *)gUnknown_030040D8)->unk06_7 < u->unk06_7)
        ((struct Unit *)gUnknown_030040D8)->unk06_7 = u->unk06_7;

    q = ((struct Unit *)gUnknown_030040D8);
    sum = q->ammo + u->ammo;
    if (sum > gUnknown_085D5ABC[q->type].maxAmmo)
        q->ammo = gUnknown_085D5ABC[q->type].maxAmmo;
    else
        q->ammo = sum;

    if (gPlaySt.savingEnabled == 0 && gPlaySt.fog == 0)
    {
        sub_08025B80(((struct Unit *)gUnknown_030040D8), gUnknown_03004074);
        gUnknown_03004074 = 0;
    }

    sum = ((struct Unit *)gUnknown_030040D8)->fuel + u->fuel;
    if (sum > gUnknown_085D5ABC[((struct Unit *)gUnknown_030040D8)->type].maxFuel)
        ((struct Unit *)gUnknown_030040D8)->fuel = gUnknown_085D5ABC[((struct Unit *)gUnknown_030040D8)->type].maxFuel;
    else
        ((struct Unit *)gUnknown_030040D8)->fuel = sum;
    u->type = 0;
    sub_080424E4();
}
