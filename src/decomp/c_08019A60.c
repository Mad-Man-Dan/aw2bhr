#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019A60.
 * sub_08019A60 @ 0x08019A60
 */

#include "hardware.h"
/* The list object is reached through this cast and never copied into a local
 * pointer -- see the note on the function below. */

#define P ((struct Unk8019A60 *)arg)

/*
 * Menu_HandleButtons -- act on a button press in an option list.
 *
 * `arg` is the list object (struct Unk8019A60). .unk42 is the row the cursor is
 * on, .unk31 maps a row to an item index, .unk20 is the array of item records
 * and .unk24 holds one flags byte per item. Each item record carries three
 * callbacks, and each is called with (item index, row, that item's flags). The
 * bits of gpKeySt->pressed are the GBA key order, so bit 0 is A, bit 1 is B and
 * bit 2 is Select:
 *
 *   - Select runs the item's .unk10 callback.
 *   - A runs .unk14, but only if RunMapEventsForAction lets the item through. A sound
 *     plays first: 0x68 when bit 1 of the item's flags is set, 0x65 otherwise.
 *   - B stops the gUnknown_08489568 script, runs .unk18, then plays sound 0x66.
 *
 * A callback of NULL means that button does nothing for that item. Only the
 * first of the three buttons found pressed is acted on.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `arg` goes through the `#define P` cast and is never copied into a local
 *     pointer. A local costs a second register holding the same pointer, and
 *     that pushes a further value into a high register.
 *   - In the A branch `idx` and `tbl` hold the cursor's address and the row
 *     table, and `new_var` holds the cursor value read before the RunMapEventsForAction
 *     test. Without them that branch recomputes both addresses, which the
 *     original does not.
 *   - The `do { } while (0)` around the last call in that branch is
 *     byte-neutral as far as was tested, and is kept because nothing has
 *     re-derived it.
 */
void Menu_HandleButtons(void *arg)
{
    long new_var;
    void (*fn)(u8, u8, u8);

    if (gpKeySt->pressed & 4)
    {
        fn = P->unk20[P->unk31[P->unk42]].unk10;

        if (fn != 0)
            fn(P->unk31[P->unk42], P->unk42,
               P->unk24[P->unk31[P->unk42]]);
    }
    else if (gpKeySt->pressed & 1)
    {
        u8 *idx;
        u8 *tbl;

        idx = &P->unk42;
        new_var = *idx;
        tbl = P->unk31;

        if (RunMapEventsForAction(P->unk20[tbl[new_var]].unk00, 0) == 0)
        {
            if (P->unk24[tbl[*idx]] & 2)
                PlayMusicOrSfx2(0x68);
            else
                PlayMusicOrSfx2(0x65);

            fn = P->unk20[tbl[*idx]].unk14;

            if (fn != 0)
            {
                do { fn(tbl[*idx], *idx, P->unk24[tbl[*idx]]); } while (0);
            }
        }
    }
    else if (gpKeySt->pressed & 2)
    {
        sub_0801537C(gUnknown_08489568);

        fn = P->unk20[P->unk31[P->unk42]].unk18;

        if (fn != 0)
            fn(P->unk31[P->unk42], P->unk42,
               P->unk24[P->unk31[P->unk42]]);

        PlayMusicOrSfx2(0x66);
    }
}
asm(".global sub_08019A60\n.thumb_set sub_08019A60, Menu_HandleButtons\n");
