#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066BF4.
 * MatchSetupMoveRuleCursor @ 0x08066BF4, RuleOption_ChangeValue @ 0x08066C70
 */

#include "hardware.h"
/* The cursor stepper for the 0x0816E160 menu -- the twin of RulesScreenRuleOption_ChangeValue,
 * which is byte-identical apart from which private .rodata pool word carries
 * the gpKeySt address (0x0816E160 vs 0x0816E1B4; both ROM words hold
 * 0x03002EE0).
 *
 * unk48 is the cursor, unk4b the entry count and unk49 the previous value,
 * saved first so the tail can play sound 0x64 on any change. unk47 selects
 * WRAPPING (non-zero) or CLAMPING (zero) behaviour, and the two halves are
 * spelled with the `== 0` case FIRST: the ROM branches away on `bne`, which
 * puts the equality arm inline. */
/* Steps the seven-entry unk54[] selector at gUnknown_08580934->unk33 with the
 * gpKeySt->repeated bitmask, and plays sound 0x64 whenever the index actually
 * moved. Byte-identical twin of RulesScreenHandleMenuInput: the two differ only in which
 * private .rodata address-constant word carries &gUnknown_08580934 and
 * &gpKeySt (0x0816E158/0x0816E15C vs 0x0816E1AC/0x0816E1B0 -- both pairs hold
 * 0x08580934 and 0x03002EE0), so the SOURCE is the same and the honest
 * spelling reproduces both.
 *
 * Each half is `if (i == <edge>) { wrap } else { step }` and NOT the
 * `if (i != <edge>) step; else wrap` that reads more naturally: the ROM
 * branches away with `bne`, which puts the equality arm inline and the step in
 * an out-of-line block after the literal pool.
 *
 * The wrap is gated on `unk02 == held`, i.e. it only fires when the pressed
 * mask is exactly the held mask -- no other key down. */
void MatchSetupMoveRuleCursor(void)
{
    int i;

    i = gUnknown_08580934->unk33;

    if (gpKeySt->repeated & DPAD_LEFT)
    {
        if (i == 0)
        {
            if (gpKeySt->repeated == gpKeySt->pressed)
                i = 6;
        }
        else
        {
            i--;
        }
    }

    if (gpKeySt->repeated & DPAD_RIGHT)
    {
        if (i == 6)
        {
            if (gpKeySt->repeated == gpKeySt->pressed)
                i = 0;
        }
        else
        {
            i++;
        }
    }

    if (i != gUnknown_08580934->unk33)
    {
        gUnknown_08580934->unk2a = 0;
        PlayMusicOrSfx2(0x64);
    }

    gUnknown_08580934->unk33 = i;
}

asm(".global sub_08066BF4\n.thumb_set sub_08066BF4, MatchSetupMoveRuleCursor\n");

void RuleOption_ChangeValue(struct Unk08580934_Obj *p)
{
    p->unk49 = p->unk48;

    if (p->unk47 == 0)
    {
        if (gpKeySt->repeated & DPAD_UP)
        {
            if (p->unk48 != 0)
                p->unk48 = p->unk48 - 1;
        }

        if (gpKeySt->repeated & DPAD_DOWN)
        {
            if (p->unk48 < p->unk4b - 1)
                p->unk48 = p->unk48 + 1;
        }
    }
    else
    {
        if (gpKeySt->repeated & DPAD_UP)
        {
            if (p->unk48 == 0)
                p->unk48 = p->unk4b - 1;
            else
                p->unk48 = p->unk48 - 1;
        }

        if (gpKeySt->repeated & DPAD_DOWN)
        {
            if (p->unk48 == p->unk4b - 1)
                p->unk48 = 0;
            else
                p->unk48 = p->unk48 + 1;
        }
    }

    if (p->unk49 != p->unk48)
        PlayMusicOrSfx2(0x64);
}
asm(".global sub_08066C70\n.thumb_set sub_08066C70, RuleOption_ChangeValue\n");
