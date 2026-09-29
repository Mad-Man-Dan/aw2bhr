#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065238.
 * sub_08065238 @ 0x08065238, sub_0806530C @ 0x0806530C, sub_0806540C @ 0x0806540C, sub_080654E8 @ 0x080654E8
 */

/* Builds the gUnknown_08580934->unk34[] object row: one sub_080152EC slot per
 * live entry (->unk08 of them), each seeded with its index and handed to
 * LoadCoFace with its own 0x24-tile VRAM window.
 *
 * The two 0x24-stepped registers are ONE source expression each, not a shared
 * one. `i * 0x24 + 0x190` appears twice -- once inside the `& 0x3ff` for the
 * tile address and once as the +0x44 field -- and strength_reduce makes them
 * two givs of i rather than one CSE: the tile use gets `i * 0x24 + 0x190` in a
 * callee-saved register while the +0x44 use gets the bare `i * 0x24` and
 * rematerialises `0xc8 << 1` inside the loop body. Binding the value to a
 * single local instead (`x = 0x190; ... x += 0x24;`) collapses both to one
 * register, hoists the constant into the preheader and loses one high
 * register from the prologue -- probed, and it is the wrong shape. */
void sub_08065238(void)
{
    struct Unk08580934_Obj *obj;
    int i;

    sub_08073304(gUnknown_085802A4, gUnknown_0200FC50, 0x230, 0xf, 1, 1, 3);

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        obj = (struct Unk08580934_Obj *)sub_080152EC(gUnknown_08580AF0, 3);
        obj->unk1c = i;
        gUnknown_08580934->unk34[i] = obj;
        obj->unk46 = 0;
        obj->unk28 = sub_08065200(i);
        obj->unk38 = 0x34;
        obj->unk24 = i * 2;
        obj->unk44 = i * 0x24 + 0x190 + (i << 12);
        LoadCoFace(gUnknown_08580934->unk18[gUnknown_08580934->unk1c[i]],
                     (void *)(0x06010000 + (((i * 0x24 + 0x190) & 0x3ff) << 5)),
                     i + 0x10);
    }
}

/* The third member of the sub_08065238 / sub_0806540C set in this block: same
 * loop, same two 0x24-stepped givs, same (0x230, 0xf, 1, 1, 3) sub_08073304
 * call. It differs by allocating a SECOND Proc per slot (gUnknown_08580A08)
 * and handing it to sub_08064D44 -- the same shape sub_0806502C uses on
 * sub_08064BC8. `obj` is reused for both allocations, which is what puts both
 * in r5, and `i * 2` is written twice (the +0x24 field and sub_08064D44's
 * fourth argument) and CSEd into r4 across the LoadCoFace call. */
void sub_0806530C(void)
{
    struct Unk08580934_Obj *obj;
    int i;

    sub_08073304(gUnknown_085802A4, gUnknown_0200FC50, 0x230, 0xf, 1, 1, 3);

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        obj = (struct Unk08580934_Obj *)sub_080152EC(gUnknown_08580B90, 3);
        obj->unk1c = i;
        gUnknown_08580934->unk34[i] = obj;
        obj->unk46 = 0;
        obj->unk28 = sub_08065200(i);
        obj->unk38 = 0;
        obj->unk24 = i * 2;
        obj->unk44 = i * 0x24 + 0x190 + (i << 12);
        LoadCoFace(gUnknown_08580934->unk18[gUnknown_08580934->unk1c[i]],
                     (void *)(0x06010000 + (((i * 0x24 + 0x190) & 0x3ff) << 5)),
                     i + 0x10);
        obj = (struct Unk08580934_Obj *)sub_080152EC(gUnknown_08580A08, 3);
        sub_08064D44(obj, sub_08065200(i) + 8, i, i * 2);
    }
}

/* The sibling of sub_08065238 (same block): identical statement for statement
 * apart from the Proc script (gUnknown_08580BC8 vs gUnknown_08580AF0) and the
 * +0x38 seed, which is 0 here and 0x34 there. That one constant is the whole
 * reason this function saves a third high register -- 0x38's zero cannot share
 * the pseudo holding 0x46's zero, so it gets its own callee-saved slot. */
void sub_0806540C(void)
{
    struct Unk08580934_Obj *obj;
    int i;

    sub_08073304(gUnknown_085802A4, gUnknown_0200FC50, 0x230, 0xf, 1, 1, 3);

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        obj = (struct Unk08580934_Obj *)sub_080152EC(gUnknown_08580BC8, 3);
        obj->unk1c = i;
        gUnknown_08580934->unk34[i] = obj;
        obj->unk46 = 0;
        obj->unk28 = sub_08065200(i);
        obj->unk38 = 0;
        obj->unk24 = i * 2;
        obj->unk44 = i * 0x24 + 0x190 + (i << 12);
        LoadCoFace(gUnknown_08580934->unk18[gUnknown_08580934->unk1c[i]],
                     (void *)(0x06010000 + (((i * 0x24 + 0x190) & 0x3ff) << 5)),
                     i + 0x10);
    }
}

/* Teardown for the three Proc scripts sub_08065238 / sub_0806530C /
 * sub_0806540C start: if none of them is running it returns at once, otherwise
 * it snaps every unk34[] object to its final position and stops all five.
 *
 * gUnknown_0816E0F8 / _0816E0FC / _0816E100 are agbcc's own -fforce-addr words
 * holding &gUnknown_08580AF0, &gUnknown_08580B90 and &gUnknown_08580BC8 (read
 * straight out of baserom.gba at those addresses). Each of the three is
 * referenced TWICE across the control-flow merge -- once by sub_08015BD0 and
 * once by sub_0806377C -- which is exactly the documented trigger; the two
 * scripts named only once (gUnknown_08580A38, gUnknown_08580A08) get plain
 * pool words instead. Naming all five honestly reproduces the split.
 *
 * The two later `== -1` tests compile to `cmp r4, r5` against registers rather
 * than a fresh -1: inside the `&&` chain the earlier results are already known
 * to be -1 and CSE substitutes them. That is not a comparison between the
 * three results in the source. */
void sub_080654E8(void)
{
    struct Unk08580934_Obj *obj;
    int i;

    sub_080733B8();

    if (sub_08015BD0((s32)gUnknown_08580AF0) == -1
        && sub_08015BD0((s32)gUnknown_08580B90) == -1
        && sub_08015BD0((s32)gUnknown_08580BC8) == -1)
        return;

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        obj = gUnknown_08580934->unk34[i];
        obj->unk24 = obj->unk28 / 32;
        obj->unk34 = -5;
        obj->unk30 = 0;
    }

    sub_0806377C(gUnknown_08580AF0);
    sub_0806377C(gUnknown_08580B90);
    sub_0806377C(gUnknown_08580BC8);
    sub_0806377C(gUnknown_08580A38);
    sub_0806377C(gUnknown_08580A08);
}
