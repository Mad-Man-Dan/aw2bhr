#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017C4C.
 * sub_08017C4C @ 0x08017C4C, sub_08017C70 @ 0x08017C70, sub_08017CB0 @ 0x08017CB0, sub_08017CF0 @ 0x08017CF0, sub_08017D30 @ 0x08017D30
 */

/*
 * EventCb_ClearWhenCameraScrollEnds -- clear a slot's callback once its script has finished.
 *
 * Installed as the callback by EventOp_ScrollCameraKeepInView and EventOp_ScrollCameraToCenter below.
 * FindSlotScript searches the gUnknown_03001470 slots for the one running the
 * gUnknown_0849A00C script; when there is none (-1) this slot's .unk08 is
 * cleared, which stops the callback being called again. sub_08017ABC is the
 * same function for another script.
 *
 * The `(s32)` cast on the script pointer is what every FindSlotScript caller in
 * src/decomp does, because its prototype takes an s32.
 */
void EventCb_ClearWhenCameraScrollEnds(struct Unk0200C528 *slot)
{
    if (FindSlotScript((s32)gUnknown_0849A00C) == -1)
        slot->unk08 = NULL;
}
asm(".global sub_08017C4C\n.thumb_set sub_08017C4C, EventCb_ClearWhenCameraScrollEnds\n");

/*
 * EventOp_ScrollCameraKeepInView -- script command: pass the current node's x/y to ScrollCameraToKeepCellInView.
 *
 * gUnknown_0200C528[a].unk04 is the slot's cursor into its list of script
 * nodes. The node's .unk08 and .unk0a go to ScrollCameraToKeepCellInView as a signed pair,
 * EventCb_ClearWhenCameraScrollEnds above is installed as the slot's callback, and the cursor steps
 * on one node. Returns FALSE, which ends the slot's turn for this frame (see
 * the dispatcher in src/decomp/c_08019404.c).
 *
 * .unk08 and .unk0a are declared u16 but read here as signed halfwords, so the
 * casts are written out; retyping the members would emit the same bytes, so the
 * header is left alone. The slot's callback field is itself declared as a node
 * pointer, hence the cast on EventCb_ClearWhenCameraScrollEnds.
 */
bool8 EventOp_ScrollCameraKeepInView(s16 a)
{
    struct Unk0200C528Node *p = gUnknown_0200C528[a].unk04;

    ScrollCameraToKeepCellInView((s16)p->unk08, (s16)p->unk0a);
    gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)EventCb_ClearWhenCameraScrollEnds;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08017C70\n.thumb_set sub_08017C70, EventOp_ScrollCameraKeepInView\n");

/* EventOp_ScrollCameraToCenter -- EventOp_ScrollCameraKeepInView above with ScrollCameraToCenterCell in place of
 * ScrollCameraToKeepCellInView. */
bool8 EventOp_ScrollCameraToCenter(s16 a)
{
    struct Unk0200C528Node *p = gUnknown_0200C528[a].unk04;

    ScrollCameraToCenterCell((s16)p->unk08, (s16)p->unk0a);
    gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)EventCb_ClearWhenCameraScrollEnds;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08017CB0\n.thumb_set sub_08017CB0, EventOp_ScrollCameraToCenter\n");

/*
 * EventOp_CallFunctionSkippable -- script command: run EventOp_CallFunction unless gUnknown_03002514 is 1.
 *
 * When gUnknown_03002514 is 1 the command is skipped: the slot's cursor steps
 * on one node and TRUE comes back, which makes the dispatcher in
 * src/decomp/c_08019404.c run the next command in the same frame. Otherwise
 * EventOp_CallFunction handles the node and its result is passed straight on.
 *
 * Why the C looks odd: the arms are written the opposite way round from the
 * order the original's code is in. When both arms of an if/else end in
 * `return`, the compiler puts the else arm inline and branches to the then arm,
 * so testing `!= 1` is what leaves the cursor step inline as the original has
 * it. Writing the test the natural way round, with or without an explicit
 * `else`, swaps the two blocks over.
 */
s16 EventOp_CallFunctionSkippable(s16 a)
{
    if (gUnknown_03002514 != 1)
        return EventOp_CallFunction(a);
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}
asm(".global sub_08017CF0\n.thumb_set sub_08017CF0, EventOp_CallFunctionSkippable\n");

/*
 * EventOp_WaitSkippable -- EventOp_CallFunctionSkippable above, over EventOp_Wait instead of
 * EventOp_CallFunction.
 *
 * It is also what settles this family's return type: it passes EventOp_Wait's
 * result straight on and sign-extends it as a halfword, which a bool8 callee
 * could not produce. See the note in include/unknown-functions.h. Same inverted
 * arms as EventOp_CallFunctionSkippable.
 */
s16 EventOp_WaitSkippable(s16 a)
{
    if (gUnknown_03002514 != 1)
        return EventOp_Wait(a);
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}
asm(".global sub_08017D30\n.thumb_set sub_08017D30, EventOp_WaitSkippable\n");
