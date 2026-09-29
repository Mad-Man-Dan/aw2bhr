#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030038.
 * sub_08030038 @ 0x08030038
 */

/* MATCHED (wave 49, W49-B). First attempt, no probe rounds.
 *
 * The five names in this function's data_refs are ONE object, not five
 * globals: 0x02025564 + 0x00 / +0x02 / +0x05 / +0x22 / +0x34 are
 * gUnknown_02025564 itself, .unk02[0], .unk05[0], .unk20[0].unk02[0] and
 * .unk20[0].unk14[0].  gen_lds.py invents a gUnknown_<addr> for each because
 * agbcc materialises a member's address constant as a whole pool word, and for
 * a RAM address there is no relocation that could tell the two spellings apart.
 * Declaring the other four would have created four more names for storage that
 * already has one, and the error would have been byte-neutral -- no build, no
 * try_match and no ROM hash could catch it.  try_match confirms the reading:
 * it reports all four as "different symbols that resolve to the same address".
 *
 * Corroboration that needed no assembly: the three copy loops' trip counts are
 * 2, 3 and 24, which are exactly the declared extents of unk00, unk02 and
 * unk05.
 *
 * The second parameter is indexed with the same +2 / +5 / +0x22 / +0x34 and
 * *28 offsets as the global, so it is a second struct Unk02025564.  The
 * repeated `lsls #3; subs; lsls #2` is a multiply by 28, i.e. an index into
 * unk20[], and offsets like +0x33 are unk20[i] MEMBER offsets after the base
 * add (0x20 + 0x13 = unk13), not stray globals.
 *
 * unk00/unk02/unk05 are volatile, which is why each `|=` reads the destination
 * a second time between the `orrs` and the `strb`.  That dead-looking `ldrb` is
 * the volatile re-read, not an artefact to spell away.
 *
 * s16 first parameter: PROMOTE_MODE zero-extends it at entry (`lsls #0x10;
 * lsrs #0x10`) and the signed use in `a1 * 3` re-extends it (`lsls #0x10;
 * asrs #0x10`) -- the brief's rule that the sign lives in the second shift
 * pair, not the prologue.
 *
 * Nothing in struct Unk02025564 or struct Unk02025584 needed changing. */

void LinkMergeMapListBlock(s16 a1, struct Unk02025564 *p)
{
    int i;
    int j;

    for (i = 0; i < 2; i++)
        gUnknown_02025564.unk00[i] |= p->unk00[i];
    for (i = 0; i < 3; i++)
        gUnknown_02025564.unk02[i] |= p->unk02[i];
    for (i = 0; i < 24; i++)
        gUnknown_02025564.unk05[i] |= p->unk05[i];

    for (j = 0; j < 3; j++)
    {
        gUnknown_02025564.unk20[a1 * 3 + j].unk00 = p->unk20[j].unk00;
        gUnknown_02025564.unk20[a1 * 3 + j].unk01 = p->unk20[j].unk01;
        gUnknown_02025564.unk20[a1 * 3 + j].unk13 = p->unk20[j].unk13;
        gUnknown_02025564.unk20[a1 * 3 + j].unk19 = p->unk20[j].unk19;
        gUnknown_02025564.unk20[a1 * 3 + j].unk1a = p->unk20[j].unk1a;
        for (i = 0; i < 17; i++)
            gUnknown_02025564.unk20[a1 * 3 + j].unk02[i] = p->unk20[j].unk02[i];
        for (i = 0; i < 5; i++)
            gUnknown_02025564.unk20[a1 * 3 + j].unk14[i] = p->unk20[j].unk14[i];
    }
}
asm(".global sub_08030038\n.thumb_set sub_08030038, LinkMergeMapListBlock\n");
