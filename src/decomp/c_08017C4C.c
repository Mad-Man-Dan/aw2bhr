#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017C4C.
 * sub_08017C4C @ 0x08017C4C, sub_08017C70 @ 0x08017C70, sub_08017CB0 @ 0x08017CB0, sub_08017CF0 @ 0x08017CF0, sub_08017D30 @ 0x08017D30
 */

/*
 * sub_08017C4C -- clear a slot's callback once its script has finished.
 *
 * Installed as the callback by sub_08017C70 and sub_08017CB0 below.
 * sub_08015BD0 searches the gUnknown_03001470 slots for the one running the
 * gUnknown_0849A00C script; when there is none (-1) this slot's .unk08 is
 * cleared, which stops the callback being called again. sub_08017ABC is the
 * same function for another script.
 *
 * The `(s32)` cast on the script pointer is what every sub_08015BD0 caller in
 * src/decomp does, because its prototype takes an s32.
 */
void sub_08017C4C(struct Unk0200C528 *slot)
{
    if (sub_08015BD0((s32)gUnknown_0849A00C) == -1)
        slot->unk08 = NULL;
}

/*
 * sub_08017C70 -- script command: pass the current node's x/y to sub_08029088.
 *
 * gUnknown_0200C528[a].unk04 is the slot's cursor into its list of script
 * nodes. The node's .unk08 and .unk0a go to sub_08029088 as a signed pair,
 * sub_08017C4C above is installed as the slot's callback, and the cursor steps
 * on one node. Returns FALSE, which ends the slot's turn for this frame (see
 * the dispatcher in src/decomp/c_08019404.c).
 *
 * .unk08 and .unk0a are declared u16 but read here as signed halfwords, so the
 * casts are written out; retyping the members would emit the same bytes, so the
 * header is left alone. The slot's callback field is itself declared as a node
 * pointer, hence the cast on sub_08017C4C.
 */
bool8 sub_08017C70(s16 a)
{
    struct Unk0200C528Node *p = gUnknown_0200C528[a].unk04;

    sub_08029088((s16)p->unk08, (s16)p->unk0a);
    gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)sub_08017C4C;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}

/* sub_08017CB0 -- sub_08017C70 above with sub_0802909C in place of
 * sub_08029088. */
bool8 sub_08017CB0(s16 a)
{
    struct Unk0200C528Node *p = gUnknown_0200C528[a].unk04;

    sub_0802909C((s16)p->unk08, (s16)p->unk0a);
    gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)sub_08017C4C;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}

/*
 * sub_08017CF0 -- script command: run sub_08017A80 unless gUnknown_03002514 is 1.
 *
 * When gUnknown_03002514 is 1 the command is skipped: the slot's cursor steps
 * on one node and TRUE comes back, which makes the dispatcher in
 * src/decomp/c_08019404.c run the next command in the same frame. Otherwise
 * sub_08017A80 handles the node and its result is passed straight on.
 *
 * Why the C looks odd: the arms are written the opposite way round from the
 * order the original's code is in. When both arms of an if/else end in
 * `return`, the compiler puts the else arm inline and branches to the then arm,
 * so testing `!= 1` is what leaves the cursor step inline as the original has
 * it. Writing the test the natural way round, with or without an explicit
 * `else`, swaps the two blocks over.
 */
s16 sub_08017CF0(s16 a)
{
    if (gUnknown_03002514 != 1)
        return sub_08017A80(a);
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}

/*
 * sub_08017D30 -- sub_08017CF0 above, over sub_08017A58 instead of
 * sub_08017A80.
 *
 * It is also what settles this family's return type: it passes sub_08017A58's
 * result straight on and sign-extends it as a halfword, which a bool8 callee
 * could not produce. See the note in include/unknown-functions.h. Same inverted
 * arms as sub_08017CF0.
 */
s16 sub_08017D30(s16 a)
{
    if (gUnknown_03002514 != 1)
        return sub_08017A58(a);
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}
