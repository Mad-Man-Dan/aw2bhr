#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080184EC.
 * sub_080184EC @ 0x080184EC, sub_0801853C @ 0x0801853C
 */

/*
 * sub_080184EC -- one step of a screen wipe: advance the counter by 4 and move
 * the raster effect with it.
 *
 * Installed as a gUnknown_0200C528 slot's callback, and one of the family with
 * sub_080180A8, sub_080180CC and sub_0801820C, which step the same counter and
 * the same three display shadows. Each frame the slot's .unk0e goes up by 4.
 * While it is 0x2e or less, the two scroll shadows take -0x70 minus the
 * counter, gUnknown_03001420 takes the counter itself, and sub_08011AAC re-arms
 * sub_08017EEC as the HBlank handler. Past 0x2e the wipe is over: sub_08012A74
 * tidies up and the slot's callback becomes sub_080184E0.
 *
 * .unk0e is an s16, which is why the compare sign-extends the value just
 * stored instead of re-reading it. Storing sub_080184E0 in the callback field
 * is a state change and not a call; that field is declared as a node pointer
 * (sub_08018B40 stores a node link in it), hence the cast.
 */
void sub_080184EC(struct Unk0200C528 *slot)
{
    slot->unk0e += 4;
    if (slot->unk0e > 0x2e)
    {
        slot->unk08 = (struct Unk0200C528Node *)sub_080184E0;
        sub_08012A74();
    }
    else
    {
        gUnknown_0300309C = -0x70 - slot->unk0e;
        gUnknown_03002028 = -0x70 - slot->unk0e;
        gUnknown_03001420 = slot->unk0e;
        sub_08011AAC((void *)sub_08017EEC);
    }
}

/*
 * sub_0801853C -- script command: start the sub_080184EC wipe on this slot.
 *
 * Unless a fade is already running, the slot's counter is zeroed and
 * sub_080184EC above becomes its callback. Either way the cursor then steps one
 * 0x10-byte node on and FALSE comes back, which ends the slot's turn for this
 * frame (see the dispatcher in src/decomp/c_08019404.c).
 *
 * Why the C looks odd: the two tests are one `||`, so the body runs when
 * gUnknown_03002514 is not 1 or gUnknown_03001420 is 0. The slot's address is
 * computed on both sides of the branch in the original; the compiler puts it
 * there by itself.
 */
bool8 sub_0801853C(s16 a)
{
    if (gUnknown_03002514 != 1 || gUnknown_03001420 == 0)
    {
        gUnknown_0200C528[a].unk0e = 0;
        gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)sub_080184EC;
    }
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
