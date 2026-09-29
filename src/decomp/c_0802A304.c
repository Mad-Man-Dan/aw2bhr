#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A304.
 * sub_0802A304 @ 0x0802A304
 */

/* A callback handed to ResupplyUnitWithAnimation beside sub_0802A2E4, so its parameter is
 * the same object: a struct Unit unit record. It is NOT spelled that
 * way here on purpose. The loop reads the bytes at +7 and +8 through a
 * VARIABLE index, which the shared struct cannot express (they are the
 * separate scalars unk07/unk08 there), and widening them into a member array
 * would reshape a struct several promoted functions already read. The local
 * view below is byte-identical and touches nothing shared; the evidence that
 * unk07/unk08 are one cargo array of unit ids is recorded on the struct in
 * include/unknown-globals.h.
 *
 * The loop BOUND is the whole chain gUnknown_085D5ABC[p->unk00].unk14[0], and
 * it is re-evaluated at the bottom of every iteration -- the two calls kill
 * the load, so nothing is hoisted and nothing may be bound to a local.
 * `&gUnknown_085D5ABC[0].unk14` is the piece that IS common, and CSE parks it
 * in sb as `base + 0x14` rather than folding 0x14 into the ldr displacement.
 *
 * Counter is u8 (`lsls #0x18; lsrs #0x18` renarrow) and both compares come
 * out unsigned (`blo`/`bhs`) because both sides are zero-extended bytes. */

struct Unk2A304
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[0x06];
    /* 0x07 */ u8 unk07[2];
};

int CalcCargoResupplyCost(void *arg)
{
    struct Unk2A304 *p = arg;
    int sum;
    u8 i;

    sum = 0;

    for (i = 0; i < gUnknown_085D5ABC[p->unk00].transportTable[0]; i++)
    {
        if (p->unk07[i] != 0)
        {
            sum += ResupplyUnitAmmo(&gUnits[p->unk07[i]], 0);
            sum += ResupplyUnitFuel(&gUnits[p->unk07[i]], 0);
        }
    }

    return sum;
}
asm(".global sub_0802A304\n.thumb_set sub_0802A304, CalcCargoResupplyCost\n");
