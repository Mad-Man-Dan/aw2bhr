#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A664.
 * sub_0808A664 @ 0x0808A664, sub_0808A6A0 @ 0x0808A6A0
 */

/* The `ldrh` after the `strh` is a genuine re-read, not a missed CSE: the
 * incremented counter is stored and then loaded again to be handed to DivRem.
 * That is what `g++; DivRem(g, 2)` produces, where `DivRem(++g, 2)` would keep
 * the value in the register.
 *
 * The gate is a bare `& 1` on the `s32` gGameClock, so this runs on
 * alternate frames of an alternate condition -- the half-rate idiom the
 * gUnknown_081A47E4 readers use with `/2` instead. */
void CampaignIntroBg_Loop(void)
{
    if (gGameClock & 1)
    {
        gUnknown_0300200C++;

        if (DivRem(gUnknown_0300200C, 2) != 0)
            gUnknown_03002000--;
    }
}
asm(".global sub_0808A664\n.thumb_set sub_0808A664, CampaignIntroBg_Loop\n");

/* One statement: StartTextBox's result is used directly, with no local. Six
 * arguments -- `movs r0,#0x80; lsls r0,r0,#8` is 0x8000 and 0x41 follows it in
 * the second stack slot -- and the returned gUnknown_03001470 slot takes a 6 in
 * its +0x3a byte. `adds r0, #0x3a` is strb's 31-byte displacement limit. */
void CampaignIntro_ShowPrologueText(void)
{
    StartTextBox(6, 5, gBG0TilemapBuffer, 0, 0x8000, 0x41)->unk3a = 6;
}
asm(".global sub_0808A6A0\n.thumb_set sub_0808A6A0, CampaignIntro_ShowPrologueText\n");
