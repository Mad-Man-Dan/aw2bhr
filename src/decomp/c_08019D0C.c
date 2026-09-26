#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019D0C.
 * sub_08019D0C @ 0x08019D0C, sub_08019D48 @ 0x08019D48, sub_08019D78 @ 0x08019D78, sub_08019DA8 @ 0x08019DA8, sub_08019DCC @ 0x08019DCC, sub_08019DEC @ 0x08019DEC
 */

/* The 0x48-byte object the functions from 0x08019A60 to 0x08019D48 walk. It is
 * not the object sub_08019DCC and sub_08019DEC take: both arrive as a ProcPtr,
 * but this one holds a pointer at +0x20 where that one holds a signed halfword,
 * so they are different objects and keep different tags. */
struct Unk08019B50Cmd /* 0x20 */
{
    /* 0x00 */ u8 filler_00[0x0c];
    /* 0x0c */ void (*unk0c)(u8, u8, u8);
    /* 0x10 */ u8 filler_10[0x10];
};
struct Unk08019B50 /* 0x48 */
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ struct Unk08019B50Cmd *unk20;
    /* 0x24 */ u8 unk24[0x0d];
    /* 0x31 */ u8 unk31[0x10];
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 filler_43[0x01];
    /* 0x44 */ struct Unk03001470 *unk44;
};
/* The coordinate object sub_08019DCC and sub_08019DEC take; see the note above
 * for why it is not struct Unk08019B50. */
struct Unk08019DCC /* 0x28 */
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ u8 filler_22[0x02];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u16 unk26;
};

/*
 * sub_08019D0C -- one frame of an option list: take input, then keep the cursor
 * sprite in step.
 *
 * sub_08019B80 gets the frame first; when it reports nothing done,
 * sub_08019A60 acts on the button press instead. Then, if the
 * gUnknown_0848A42C script is still running in some gUnknown_03001470 slot, the
 * cursor sprite's slot at .unk44 is told the current row.
 *
 * Why the C looks odd: the parameter is typed rather than arriving as a ProcPtr
 * and being cast into a local. A local here stays in a register of its own
 * alongside the incoming copy, because both are live across the calls, and that
 * costs one more saved register. sub_08019D48 below is the other way round:
 * there the copy folds away and either spelling matches.
 */
void sub_08019D0C(struct Unk08019B50 *p)
{
    if (!sub_08019B80(p))
        sub_08019A60(p);

    if (sub_08015BD0((s32)gUnknown_0848A42C) != -1)
        p->unk44->unk20 = p->unk42;
}

/*
 * sub_08019D48 -- tear an option list down.
 *
 * Runs the two closers sub_08022ADC and sub_08019C24, uploads
 * gBG0TilemapBuffer to BG VRAM at 0x06007000 now that sub_08019C24 has been
 * over it, and ends the cursor sprite's script with sub_080153B8.
 * sub_08019D78 and sub_08019DA8 below both start here and then tidy up their
 * own extras.
 */
void sub_08019D48(ProcPtr proc)
{
    struct Unk08019B50 *p = (struct Unk08019B50 *)proc;

    sub_08022ADC();
    sub_08019C24();
    sub_08011E54(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
    sub_080153B8(p->unk44);
}

/*
 * sub_08019D78 -- tear the list down and blank the layer it sat over.
 *
 * sub_08019D48 above does the teardown. sub_08012BC8 then fills the 32 x 20
 * visible area of gBG2TilemapBuffer with tile 0x360, and sub_08013AD4 flags BG2
 * for copying to VRAM. One of the two teardowns sub_08019F90 picks between when
 * it builds a list.
 *
 * `proc` is only passed on: the original never touches the argument register
 * before the call, and it is sub_08019D48 that dereferences it.
 */
void sub_08019D78(ProcPtr proc)
{
    sub_08019D48(proc);
    sub_08012BC8(gBG2TilemapBuffer, 0, 0, 0x20, 0x14, 0x360);
    sub_08013AD4(2);
}

/*
 * sub_08019DA8 -- tear the list down and hand control of the map back.
 *
 * sub_08019D48 above does the teardown. sub_0801A538 (which ignores all four
 * arguments), sub_08022580 and sub_080227A8 then run, and DecrementMapLock
 * releases one level of the map's input lock. The other of the two teardowns
 * sub_08019F90 picks between.
 */
void sub_08019DA8(ProcPtr proc)
{
    sub_08019D48(proc);
    sub_0801A538(0, 1, 6, 0xc);
    sub_08022580();
    sub_080227A8();
    DecrementMapLock();
}

/*
 * sub_08019DCC -- put the cursor sprite on the row the object names.
 *
 * sub_08022AD0 gets the object's .unk24 and a y in pixels, .unk20 * 16 +
 * .unk26.
 *
 * This call site is the only evidence there is for sub_08022AD0's parameter
 * widths, and it is why that definition takes s16: the first argument is a
 * signed halfword handed over with no conversion at all, and the second is
 * narrowed as a signed halfword too. u16 parameters would be zero-extended
 * instead.
 */
void sub_08019DCC(struct Unk08019DCC *p)
{
    sub_08022AD0(p->unk24, p->unk20 * 16 + p->unk26);
}

/*
 * sub_08019DEC -- slide the cursor sprite half way towards its target row.
 *
 * The target is .unk26 + 0x10 + .unk20 * 32; .unk1e is moved to the average of
 * itself and that, so each frame closes half the remaining distance. Then
 * sub_0802323C is called with .unk24, the same y in pixels sub_08019DCC
 * computes, and 3.
 *
 * Why the C looks odd: the quotient goes through `u16 t` rather than straight
 * into the member. Dividing a signed value by 2 costs three instructions for
 * the rounding, and the original's last one is an unsigned shift -- the same
 * value once the halfword store throws the top bit away, but the compiler only
 * rewrites it that way when the quotient lands somewhere narrow. Assigning
 * straight to the member keeps the signed shift, whether the member is spelled
 * s16 or u16. The `(s16)` inside the expression is a real narrowing of the sum,
 * not a parameter conversion.
 */
void sub_08019DEC(struct Unk08019DCC *p)
{
    u16 t;

    t = ((s16)(p->unk26 + 0x10 + p->unk20 * 32) + p->unk1e) / 2;
    p->unk1e = t;
    sub_0802323C(p->unk24, p->unk20 * 16 + p->unk26, 3);
}
