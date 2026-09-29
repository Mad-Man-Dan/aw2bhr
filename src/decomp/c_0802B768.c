#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802B768.
 * sub_0802B768 @ 0x0802B768, sub_0802B7E8 @ 0x0802B7E8, sub_0802B868 @ 0x0802B868, sub_0802B8C4 @ 0x0802B8C4
 */

/* gUnknown_03003130 is referenced ONCE here and gets an ordinary inline pool
 * word; SlideCursorInfoPanelToSide1 and SlideCursorInfoPanelToSide0 reference it repeatedly and get the
 * -fforce-addr .rodata indirection instead. Same function shape, same type,
 * same global -- reference count is the whole difference (W35-D's trigger,
 * replicated). */
/* WAVE 35: CANONICAL `struct Map`. Eight drafts across blocks 0x08029-0x0802B
 * each invented their own body for this tag, with 2 to 7 named fields. Every
 * one compiles and byte-matches ALONE, so trymatch cannot see the problem;
 * merged into a promoted unit it is a hard `redefinition of struct Map`.
 * All seven distinct bodies describe the SAME layout at different
 * resolutions -- the offsets reconcile exactly -- so this is their union, the
 * most refined reading of each region. Byte-neutral: only a field's START
 * OFFSET enters the address arithmetic, never its declared length, and no
 * draft referenced any filler. Keep the drafts in sync; sync_work.py
 * reintroduces whatever the drafts say. */

/* The mirror of SlideCursorInfoPanelToSide0: same two `x - (x - K) / 2` easings, opposite
 * bounds. gUnknown_08090BB8 is an agbcc -fforce-addr address word, not a
 * global -- the ROM word at 0x08090BB8 holds 0x03003130 -- so naming
 * gUnknown_03003130 directly reproduces the read and its reload after every
 * call.
 *
 * `/ 2` is a SIGNED divide, not `>> 1`: the `lsrs #0x1f; adds` pair before the
 * `asrs` is gcc's sign bias, and it appears twice here.
 *
 * Both inner tests put the CONSTANT arm first (`<=` then the divide in the
 * else). Written the other way round the instructions are identical but the
 * two arms swap around the branch. */
void SlideCursorInfoPanelToSide1(void)
{
    u16 v;

    v = -gUnknown_0849A2A0[GetCursorInfoPanelSizeClass(gUnknown_03003130.unk10,
                                        gUnknown_03003130.unk11)];

    if (gUnknown_03003130.unk0c <= 3)
    {
        gUnknown_03003130.unk08 = 1;

        if (gUnknown_03003130.unk0c <= (s16)v + 1)
            gUnknown_03003130.unk0c = 0xf2;
        else
            gUnknown_03003130.unk0c =
                gUnknown_03003130.unk0c - (gUnknown_03003130.unk0c - (s16)v) / 2;
    }

    if (gUnknown_03003130.unk0c > 0x94)
    {
        if (gUnknown_03003130.unk0c <= 0x95)
            gUnknown_03003130.unk0c = 0x94;
        else
            gUnknown_03003130.unk0c =
                gUnknown_03003130.unk0c - (gUnknown_03003130.unk0c - 0x94) / 2;

        LatchCursorInfoPanelCell();
    }

    if (gUnknown_03003130.unk0c == 0x94)
        LatchCursorInfoPanelCell();
}
asm(".global sub_0802B768\n.thumb_set sub_0802B768, SlideCursorInfoPanelToSide1\n");

/* gUnknown_08090BBC is an agbcc -fforce-addr address word, not a global: the
 * ROM word at 0x08090BBC holds 0x03003130, so naming gUnknown_03003130
 * directly reproduces the three-level read exactly, including the reload of
 * the address after every call. Same case as gUnknown_08090BC0 /
 * gUnknown_08090BC4 in c_0802BB74.c.
 *
 * `x - (x - K) / 2` appears three times here and is a SIGNED divide by two,
 * not `>> 1` -- the `lsrs #0x1f; adds` pair before the `asrs` is gcc's sign
 * bias. Writing the shift emits different code. */
void SlideCursorInfoPanelToSide0(void)
{
    u16 v;

    v = gUnknown_0849A2A0[GetCursorInfoPanelSizeClass(gUnknown_03003130.unk10,
                                       gUnknown_03003130.unk11)] + 0x98;

    if (gUnknown_03003130.unk0c > 0x93)
    {
        gUnknown_03003130.unk08 = 0;

        if (gUnknown_03003130.unk0c >= (s16)v - 1)
            gUnknown_03003130.unk0c = -0x5a;
        else
            gUnknown_03003130.unk0c =
                gUnknown_03003130.unk0c - (gUnknown_03003130.unk0c - (s16)v) / 2;
    }

    if (gUnknown_03003130.unk0c <= 2)
    {
        if (gUnknown_03003130.unk0c > 1)
            gUnknown_03003130.unk0c = 3;
        else
            gUnknown_03003130.unk0c =
                gUnknown_03003130.unk0c - (gUnknown_03003130.unk0c - 3) / 2;

        LatchCursorInfoPanelCell();
    }

    if (gUnknown_03003130.unk0c == 3)
        LatchCursorInfoPanelCell();
}
asm(".global sub_0802B7E8\n.thumb_set sub_0802B7E8, SlideCursorInfoPanelToSide0\n");

void UpdateCursorInfoPanelSlide(void)
{
    u16 x;

    x = gUnknown_030033E4.unk00 * 16 - gMap->scrollX;

    if ((s16)(gUnknown_030033E4.unk02 * 16
              - gMap->scrollY) > 0x20)
    {
        if ((s16)x > 0x7f)
            SlideCursorInfoPanelToSide0();
        else
            SlideCursorInfoPanelToSide1();
    }
    else if (gUnknown_03003130.unk08 == 1)
        SlideCursorInfoPanelToSide1();
    else
        SlideCursorInfoPanelToSide0();
}
asm(".global sub_0802B868\n.thumb_set sub_0802B868, UpdateCursorInfoPanelSlide\n");

/* a3 is u16 and `a3 * 4` is written INLINE at both uses. The ROM's split
 * `lsls r4, #0x10` in the prologue and `lsrs r4, #0xe` twelve instructions
 * later is PROMOTE_MODE's entry zero-extension with its second half sunk to
 * the use and fused with the `<< 2`. Binding `t = (u16)a3 * 4;` to a local
 * keeps the pair adjacent and moves the shift out of the argument setup. */
void DrawCursorInfoCargoSlotIcon(s16 a1, s16 a2, u16 a3)
{
    PutOamHi(a1, a2 | 0x400, gUnknown_0849A22C[1], (a3 * 4) | 0xf000);
    RegisterDataMove((void *)gUnknown_0810E9E0,
                 (void *)(0x06010000 + ((a3 * 4 + 0x1DA) & 0x3ff) * 0x20), 0x80);
}
asm(".global sub_0802B8C4\n.thumb_set sub_0802B8C4, DrawCursorInfoCargoSlotIcon\n");
