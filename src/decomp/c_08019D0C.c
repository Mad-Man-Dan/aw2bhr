#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019D0C.
 * sub_08019D0C @ 0x08019D0C, sub_08019D48 @ 0x08019D48, sub_08019D78 @ 0x08019D78, sub_08019DA8 @ 0x08019DA8, sub_08019DCC @ 0x08019DCC, sub_08019DEC @ 0x08019DEC
 */

/* The 0x48-byte object the functions from 0x08019A60 to 0x08019D48 walk. It is
 * not the object Menu_PlaceCursorSprite and Menu_SlideCursorSprite take: both arrive as a ProcPtr,
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
/* The coordinate object Menu_PlaceCursorSprite and Menu_SlideCursorSprite take; see the note above
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
 * Menu_Loop -- one frame of an option list: take input, then keep the cursor
 * sprite in step.
 *
 * Menu_MoveCursorFromDpad gets the frame first; when it reports nothing done,
 * Menu_HandleButtons acts on the button press instead. Then, if the
 * gUnknown_0848A42C script is still running in some gUnknown_03001470 slot, the
 * cursor sprite's slot at .unk44 is told the current row.
 *
 * Why the C looks odd: the parameter is typed rather than arriving as a ProcPtr
 * and being cast into a local. A local here stays in a register of its own
 * alongside the incoming copy, because both are live across the calls, and that
 * costs one more saved register. Menu_Teardown below is the other way round:
 * there the copy folds away and either spelling matches.
 */
void Menu_Loop(struct Unk08019B50 *p)
{
    if (!Menu_MoveCursorFromDpad(p))
        Menu_HandleButtons(p);

    if (FindSlotScript((s32)gUnknown_0848A42C) != -1)
        p->unk44->unk20 = p->unk42;
}
asm(".global sub_08019D0C\n.thumb_set sub_08019D0C, Menu_Loop\n");

/*
 * Menu_Teardown -- tear an option list down.
 *
 * Runs the two closers SnapMapCursorDisplayToCell and ClearBg0TilemapBuffer, uploads
 * gBG0TilemapBuffer to BG VRAM at 0x06007000 now that ClearBg0TilemapBuffer has been
 * over it, and ends the cursor sprite's script with sub_080153B8.
 * Menu_OnEndBlankBg2 and Menu_OnEndReleaseMapLock below both start here and then tidy up their
 * own extras.
 */
void Menu_Teardown(ProcPtr proc)
{
    struct Unk08019B50 *p = (struct Unk08019B50 *)proc;

    SnapMapCursorDisplayToCell();
    ClearBg0TilemapBuffer();
    RegisterDataMove(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
    sub_080153B8(p->unk44);
}
asm(".global sub_08019D48\n.thumb_set sub_08019D48, Menu_Teardown\n");

/*
 * Menu_OnEndBlankBg2 -- tear the list down and blank the layer it sat over.
 *
 * Menu_Teardown above does the teardown. FillTilemapRect then fills the 32 x 20
 * visible area of gBG2TilemapBuffer with tile 0x360, and sub_08013AD4 flags BG2
 * for copying to VRAM. One of the two teardowns CreateMenu picks between when
 * it builds a list.
 *
 * `proc` is only passed on: the original never touches the argument register
 * before the call, and it is Menu_Teardown that dereferences it.
 */
void Menu_OnEndBlankBg2(ProcPtr proc)
{
    Menu_Teardown(proc);
    FillTilemapRect(gBG2TilemapBuffer, 0, 0, 0x20, 0x14, 0x360);
    sub_08013AD4(2);
}
asm(".global sub_08019D78\n.thumb_set sub_08019D78, Menu_OnEndBlankBg2\n");

/*
 * Menu_OnEndReleaseMapLock -- tear the list down and hand control of the map back.
 *
 * Menu_Teardown above does the teardown. DisableWindow0AndResetMapLayers (which ignores all four
 * arguments), RedrawUnitLayer and RedrawUnitIconLayer then run, and DecrementMapLock
 * releases one level of the map's input lock. The other of the two teardowns
 * CreateMenu picks between.
 */
void Menu_OnEndReleaseMapLock(ProcPtr proc)
{
    Menu_Teardown(proc);
    DisableWindow0AndResetMapLayers(0, 1, 6, 0xc);
    RedrawUnitLayer();
    RedrawUnitIconLayer();
    DecrementMapLock();
}
asm(".global sub_08019DA8\n.thumb_set sub_08019DA8, Menu_OnEndReleaseMapLock\n");

/*
 * Menu_PlaceCursorSprite -- put the cursor sprite on the row the object names.
 *
 * SetMapCursorDisplayPosition gets the object's .unk24 and a y in pixels, .unk20 * 16 +
 * .unk26.
 *
 * This call site is the only evidence there is for SetMapCursorDisplayPosition's parameter
 * widths, and it is why that definition takes s16: the first argument is a
 * signed halfword handed over with no conversion at all, and the second is
 * narrowed as a signed halfword too. u16 parameters would be zero-extended
 * instead.
 */
void Menu_PlaceCursorSprite(struct Unk08019DCC *p)
{
    SetMapCursorDisplayPosition(p->unk24, p->unk20 * 16 + p->unk26);
}
asm(".global sub_08019DCC\n.thumb_set sub_08019DCC, Menu_PlaceCursorSprite\n");

/*
 * Menu_SlideCursorSprite -- slide the cursor sprite half way towards its target row.
 *
 * The target is .unk26 + 0x10 + .unk20 * 32; .unk1e is moved to the average of
 * itself and that, so each frame closes half the remaining distance. Then
 * EaseMapCursorAndDraw is called with .unk24, the same y in pixels Menu_PlaceCursorSprite
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
void Menu_SlideCursorSprite(struct Unk08019DCC *p)
{
    u16 t;

    t = ((s16)(p->unk26 + 0x10 + p->unk20 * 32) + p->unk1e) / 2;
    p->unk1e = t;
    EaseMapCursorAndDraw(p->unk24, p->unk20 * 16 + p->unk26, 3);
}
asm(".global sub_08019DEC\n.thumb_set sub_08019DEC, Menu_SlideCursorSprite\n");
