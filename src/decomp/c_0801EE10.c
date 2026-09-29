#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801EE10.
 * sub_0801EE10 @ 0x0801EE10, sub_0801EE80 @ 0x0801EE80
 */

/* Drains the sprite-request ring and runs each entry: an entry whose unk04 is
 * NEGATIVE holds a callback pointer tagged with bit 31, called through
 * _call_via_r6 with the tag cleared; otherwise unk04 is a data pointer handed
 * to sub_0801E9B0, and on a 0 result the u16 it addresses advances the OAM
 * cursor. The unmasked twin of FlushSpriteRequestsByTag, which dispatches on the top TWO
 * bits instead.
 *
 * TWO CALLS TO PopSpriteRequest IN THE SOURCE, ONE `bl` IN THE OUTPUT. This is the
 * whole reason the function was parked at +12 bytes for three waves, and the
 * inherited note had it backwards: the ROM's `b` to a bottom test looks like
 * the single-call `while ((e = f()) != 0)` spelling, but that spelling makes
 * agbcc put &gUnknown_03002510 in a -fforce-addr .rodata address constant and
 * hoist it into a callee-saved register at function ENTRY, costing the extra
 * push, the extra pop, the pool word and its padding. Writing the call twice
 * -- guard copy plus loop-bottom copy, exactly as FlushSpriteRequestsByTag does -- lets
 * cross-jumping merge the two blocks back into the ROM's single `bl`, and the
 * address constant does not appear at all. The six axes the wave-40 note ruled
 * out were all inside the loop or at the store; the lever was the loop's own
 * spelling.
 *
 * The two remaining small residuals that note listed both came from
 * gUnknown_03002B54 being VOLATILE (proved in wave 42, see its entry in
 * include/unknown-globals.h) -- that alone gives the `ldrh; lsls; asrs` read
 * and the operand order of the addition. `+=` versus the spelled-out form is
 * byte-identical, so the note's proposed fix for the ordering was not the
 * cause. */
void FlushSpriteRequests(void)
{
    struct Unk0200ED20 *e;
    int v;

    e = (struct Unk0200ED20 *)PopSpriteRequest();
    while (e != NULL)
    {
        v = e->unk04;
        if (v < 0)
        {
            v &= 0x7FFFFFFF;
            ((void (*)(void))v)();
        }
        else if (sub_0801E9B0(gUnknown_03002B54, e->unk00, e->unk02, (void *)v, e->unk0c, e->unk0a) == 0)
        {
            gUnknown_03002B54 = gUnknown_03002B54 + *(u16 *)v;
        }
        e = (struct Unk0200ED20 *)PopSpriteRequest();
    }
    gUnknown_03002510 = 0;
    ResetSpriteRequestQueue();
}
asm(".global sub_0801EE10\n.thumb_set sub_0801EE10, FlushSpriteRequests\n");

/* Expands a packed sprite stream into the OAM shadow: `n` entries of three u16
 * attributes each, with the caller's x/y offsets folded into attr1/attr0 and
 * the palette/tile word biased by `c`. Bails if the object counter plus `n`
 * would run past 0x10.
 *
 * Both globals it touches are volatile, and that is load-bearing rather than
 * decorative -- see their notes in include/unknown-globals.h. Here it is
 * gUnknown_03001FE4 being read twice in the guard/`dst` pair with no store
 * between; non-volatile, CSE folds the second read away.
 *
 * THE MASK LITERALS ARE SPELLED TWO DIFFERENT WAYS ON PURPOSE, and this is the
 * whole reason the constants come out right. `0xFFFFFF00` is `unsigned int` in
 * C89 and the operand is a zero-extended `ldrh`, so it folds down to 0xFF00 and
 * agbcc BUILDS it with `movs #0xff; lsls #8`. `~0xFF` is a signed -256, does
 * not fold, and emits the ROM's `.word 0xffffff00` pool entry. Same pair for
 * 0xFFFFFE00 / ~0x1FF. Writing 0xFF00 for the first one does NOT work: gcc
 * widens it back to -0x100 and reuses the pool word, which is 4 bytes short.
 *
 * The attribute word is ONE expression, not a statement with a `t` local for
 * the sum -- the ROM computes `d & 0xFF00` before `d & 0xFF`, which is source
 * order for a single expression and the reverse of what a separate `t = ...;`
 * statement produces. (This is the opposite of the sibling PutObjectListToOamShadow, where
 * the local IS required; do not carry the rule across.) */
void PutObjectListToLoOam(s16 a, s16 b, u16 *data, u16 c)
{
    u16 *dst;
    s16 n;

    n = data[0];
    if (gUnknown_03001FE4 + n > 0x10)
        return;
    dst = gUnknown_03002520 + gUnknown_03001FE4 * 4;
    data++;
    while (n != 0)
    {
        *dst++ = (data[0] & 0xFFFFFF00) | (((data[0] & 0xFF) + (b & 0xFF)) & 0xFF) | (b & ~0xFF);
        *dst++ = (data[1] & 0xFFFFFE00) | (((data[1] & 0x1FF) + (a & 0x1FF)) & 0x1FF) | (a & ~0x1FF);
        *dst = c + data[2];
        dst += 2;
        gUnknown_03001FE4++;
        data += 3;
        n--;
    }
}
asm(".global sub_0801EE80\n.thumb_set sub_0801EE80, PutObjectListToLoOam\n");
