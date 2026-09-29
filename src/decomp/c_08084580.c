#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084580.
 * sub_08084580 @ 0x08084580, sub_080845A8 @ 0x080845A8, sub_080845C4 @ 0x080845C4, sub_080845E8 @ 0x080845E8, sub_08084600 @ 0x08084600
 */

#include "proc.h"
/* Picks one of eight animation frames for the actor and hands it to
 * StartTextBoxViaRecord, bracketed by ClearBg0Tilemap / BG_EnableSyncBG0. The selector starts as
 * the phase DivRem(unk52, 6) and is then overridden to 6 or 7 by two independent
 * state tests, each of which re-reads unk52 -- the third read recomputes the
 * member ADDRESS (`adds r0, r6, #0; adds r0, #0x52`) where the first two share
 * it in r4, because it sits after a control-flow merge, which is where CSE's
 * address equivalence stops. That is automatic; nothing in the source asks for
 * it.
 */

struct Unk8084600
{
    /* 00 */ STRUCT_PAD(0x00, 0x52);
    /* 52 */ u16 unk52;
    /* 54 */ STRUCT_PAD(0x54, 0x66);
    /* 66 */ s16 unk66;
};

/* The first call is genuinely peeled out of the loop: the loop counter starts
 * at 0x10 and the index 0 case has to happen too, so the source runs
 * StartPalFadeToWhite(0, ...) before the `for (i = 0x10; i <= 0x1d; i++)` rather
 * than special-casing inside it. Matched off the first probe. */

void sub_08084580(ProcPtr proc)
{
    int i;

    StartPalFadeToWhite(0, 0x10, proc);

    for (i = 0x10; i <= 0x1d; i++)
        StartPalFadeToWhite(i, 0x10, proc);
}

/* gUnknown_08616AC0 is an array of two-pointer records; see the evidence
 * comment on it in include/unknown-globals.h. This one takes member 0. */

void LoadMainMenuCentreTileGraphic(int i)
{
    Decompress(gUnknown_08616AC0[i][0], (void *)0x06013300);
}
asm(".global sub_080845A8\n.thumb_set sub_080845A8, LoadMainMenuCentreTileGraphic\n");

/* Member 1 of the same two-pointer record LoadMainMenuCentreTileGraphic reads member 0 of, and
 * the destination is per-index: 1 KB of VRAM each, so entry i lands at
 * 0x06013B00 + i * 0x400. The `adds r0, #4` on the pool word rather than a
 * displacement in the `ldr` is what the record type buys -- a flat pointer
 * array indexed `i * 2 + 1` reassociates and is three instructions longer. */

/* WAVE 53 (W53-C): both of these gained a SECOND parameter that neither body
 * reads. sub_08081060 is the only caller of either, and it sets up r1 at every
 * one of its ten call sites (`movs r1, #0x88; lsls r1, r1, #1` and friends) --
 * an argument the callee ignores costs zero instructions in the callee, so the
 * promoted definitions could not see it and the CALLER is the only witness.
 * Both functions still match byte-for-byte with the parameter added. */

void LoadMainMenuSideTileGraphic(int i, int a2)
{
    Decompress(gUnknown_08616AC0[i][1], (void *)(0x06013B00 + (i << 0xa)));
}
asm(".global sub_080845C4\n.thumb_set sub_080845C4, LoadMainMenuSideTileGraphic\n");

void LoadMainMenuLabelPlateGraphic(int a1, int a2)
{
    Decompress(gUnknown_0823D980, (void *)0x06015300);
}
asm(".global sub_080845E8\n.thumb_set sub_080845E8, LoadMainMenuLabelPlateGraphic\n");

void MainMenuCarousel_DrawDescriptionText(struct Unk8084600 *p)
{
    int i;

    ClearBg0Tilemap();

    i = DivRem(p->unk52, 6);

    if (IsCampaignCompletionFlagSet(0x28) != 0 && DivRem(p->unk52 + 2, 6) == 2)
        i = 6;

    if (IsCampaignCompletionFlagSet(0x20) != 0 && DivRem(p->unk52 + 2, 6) == 0)
        i = 7;

    if (gUnknown_0300591C[1] == 0)
        StartTextBoxViaRecord(0, 0x12, gBG0TilemapBuffer, gUnknown_08616FA4[i], 0x8000, 0x40);
    else
        StartTextBoxViaRecord(0, 0x12, gBG0TilemapBuffer, gUnknown_08616FB4[i * 2 + DivRem(p->unk66, 2)], 0x8000, 0x40);

    BG_EnableSyncBG0();
}
asm(".global sub_08084600\n.thumb_set sub_08084600, MainMenuCarousel_DrawDescriptionText\n");
