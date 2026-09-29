#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806DCB8.
 * sub_0806DCB8 @ 0x0806DCB8, sub_0806DD34 @ 0x0806DD34
 */

#include "hardware.h"
/* Byte-identical twin of RuleOption_ChangeValue -- SAME SOURCE. The only difference in
 * asm/ is the private -fforce-addr .rodata word carrying &gpKeySt
 * (0x0816E1B4 here, 0x0816E160 there); both ROM words hold 0x03002EE0. */
struct Unk0806DD34
{
    /* 0x00 */ u8 filler_00[0x47];
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4a */ u8 filler_4a[0x01];
    /* 0x4b */ u8 unk4b;
};

/* Byte-identical twin of MatchSetupMoveRuleCursor -- SAME SOURCE, not a variant. The two
 * differ in asm/ only because each owns a private -fforce-addr .rodata word
 * for &gUnknown_08580934 and &gpKeySt (0x0816E1AC/0x0816E1B0 here,
 * 0x0816E158/0x0816E15C there); the ROM images of all four words are
 * 0x08580934 and 0x03002EE0. See MatchSetupMoveRuleCursor for the shape notes. */
void RulesScreenHandleMenuInput(void)
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
asm(".global sub_0806DCB8\n.thumb_set sub_0806DCB8, RulesScreenHandleMenuInput\n");

void RulesScreenRuleOption_ChangeValue(struct Unk0806DD34 *p)
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
asm(".global sub_0806DD34\n.thumb_set sub_0806DD34, RulesScreenRuleOption_ChangeValue\n");
