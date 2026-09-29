#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DF94.
 * sub_0801DF94 @ 0x0801DF94, sub_0801DFE8 @ 0x0801DFE8
 */

void InitOamRequestsAndObjAffine(void)
{
    int i;

    gUnknown_03002510 = 0;
    ResetSpriteRequestQueue();
    gUnknown_03002B54 = 0x10;
    gUnknown_03001FE4 = 0;

    for (i = 0; i < 0x20; i++)
    {
        gUnknown_030024F0[i] = 0;
        SetObjAffineRecord(i, 0x100, 0x100, 0);
    }

    ClearOamShadow();
    CopyOamShadowToOam();
}
asm(".global sub_0801DF94\n.thumb_set sub_0801DF94, InitOamRequestsAndObjAffine\n");

/* Drains the sprite-request ring: pops entries with PopSpriteRequest until it
 * returns 0, dispatches each on the top two bits of the entry's tagged unk04
 * pointer, then advances the OAM cursor by the u16 the pointer addresses.
 *
 * The dispatch is agbcc's balanced switch tree, not a chain of `if`s -- it
 * tests the MIDDLE case (0) first and `bgt` splits the remaining two, exactly
 * as the matched GetObjDimensionFromBits does. That forces the switch value to be SIGNED,
 * which is why the mask is spelled `~0x3FFFFFFF` and the third label
 * `~0x7FFFFFFF`: 0xC0000000 and 0x80000000 are `unsigned int` in C89 and would
 * give an unsigned tree (`bhi`).
 *
 * The `(s16)` casts on unk00/unk02/unk08 are the ROM's `ldrsh` at an `int`
 * parameter -- see the note on struct Unk0200ED20 in include/unknown-globals.h,
 * which argues those members are really s16 and the casts should then go away.
 *
 * The two `PopSpriteRequest` calls are the source's, not loop rotation: the twin
 * FlushSpriteRequests writes the single-call form `while ((e = f()) != 0)` and gets
 * ONE `bl` reached by a `b` to the bottom test. */
void FlushSpriteRequestsByTag(void)
{
    struct Unk0200ED20 *e;
    int v;
    int t;

    e = (struct Unk0200ED20 *)PopSpriteRequest();
    while (e != NULL)
    {
        v = e->unk04;
        t = v & ~0x3FFFFFFF;
        v = v & 0x3FFFFFFF;
        switch (t)
        {
        case 0:
            sub_0801E9B0(gUnknown_03002B54, e->unk00, e->unk02, (void *)v, e->unk0c, e->unk0a);
            break;
        case 0x40000000:
            PutObjectListToOamShadow(gUnknown_03002B54, (s16)e->unk00, (s16)e->unk02, (u16 *)v, (s16)e->unk08);
            break;
        case ~0x7FFFFFFF:
            sub_0801E508(gUnknown_03002B54, (s16)e->unk00, (s16)e->unk02, (u16 *)v, (s16)e->unk08);
            break;
        }
        gUnknown_03002B54 = gUnknown_03002B54 + *(u16 *)v;
        e = (struct Unk0200ED20 *)PopSpriteRequest();
    }
    gUnknown_03002510 = 0;
    ResetSpriteRequestQueue();
}
asm(".global sub_0801DFE8\n.thumb_set sub_0801DFE8, FlushSpriteRequestsByTag\n");
