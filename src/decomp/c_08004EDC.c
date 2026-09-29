#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08004EDC.
 * sub_08004EDC @ 0x08004EDC, sub_08004F1C @ 0x08004F1C, sub_08004F5C @ 0x08004F5C
 */

/* Family F057: one body three times, differing only in which bit of
 * gActiveMap->flags it tests (0x200 / 0x400 / 0x800) and which value it
 * stores into ->unk10 (0 / 1 / 2). data/families.json's `varies` has exactly
 * those two entries; everything else -- both callees, the script global and
 * the trailing `unk02 = 7` -- is shared by all three members.
 *
 * The bit test is a plain mask on the halfword and NOT a bitfield: a 1-bit
 * field at bit 9 of a u16 container would be read `ldrb [p,#1]` plus a double
 * shift, because get_best_mode picks the narrowest mode containing the field.
 * The ROM does `ldrh [p]` and ANDs a computed constant, so the two are
 * distinguishable here and the mask is the answer.
 *
 * `u8` third parameter: the entry `lsls #0x18; lsrs #0x18` is PROMOTE_MODE,
 * and there is no second sign-extending pair at the `cmp r2, #2`, which rules
 * out s8. The first two parameters are never read -- nothing in the ROM calls
 * any of the three, so the arity is a floor of three and `int` for the two
 * dead ones is the byte-neutral choice.
 *
 * The `if (A && B) { ... }` and the two-early-returns spellings are
 * byte-identical here (probed): there is only one exit block, so the
 * discriminator in docs/agbcc-codegen.md's two-guards rule does not apply. */

void DesignRoomMenu_PickSlot0(int a, int b, u8 c)
{
    if ((gActiveMap->flags & 0x200) && c != 2)
    {
        CloseTopMenu();
        gActiveMap->designSlot = 0;
        sub_080152EC(gUnknown_08487E14, 0);
        gActiveMap->state = 7;
    }
}
asm(".global sub_08004EDC\n.thumb_set sub_08004EDC, DesignRoomMenu_PickSlot0\n");

/* Family F057: one body three times, differing only in which bit of
 * gActiveMap->flags it tests (0x200 / 0x400 / 0x800) and which value it
 * stores into ->unk10 (0 / 1 / 2). data/families.json's `varies` has exactly
 * those two entries; everything else -- both callees, the script global and
 * the trailing `unk02 = 7` -- is shared by all three members.
 *
 * The bit test is a plain mask on the halfword and NOT a bitfield: a 1-bit
 * field at bit 9 of a u16 container would be read `ldrb [p,#1]` plus a double
 * shift, because get_best_mode picks the narrowest mode containing the field.
 * The ROM does `ldrh [p]` and ANDs a computed constant, so the two are
 * distinguishable here and the mask is the answer.
 *
 * `u8` third parameter: the entry `lsls #0x18; lsrs #0x18` is PROMOTE_MODE,
 * and there is no second sign-extending pair at the `cmp r2, #2`, which rules
 * out s8. The first two parameters are never read -- nothing in the ROM calls
 * any of the three, so the arity is a floor of three and `int` for the two
 * dead ones is the byte-neutral choice.
 *
 * The `if (A && B) { ... }` and the two-early-returns spellings are
 * byte-identical here (probed): there is only one exit block, so the
 * discriminator in docs/agbcc-codegen.md's two-guards rule does not apply. */

void DesignRoomMenu_PickSlot1(int a, int b, u8 c)
{
    if ((gActiveMap->flags & 0x400) && c != 2)
    {
        CloseTopMenu();
        gActiveMap->designSlot = 1;
        sub_080152EC(gUnknown_08487E14, 0);
        gActiveMap->state = 7;
    }
}
asm(".global sub_08004F1C\n.thumb_set sub_08004F1C, DesignRoomMenu_PickSlot1\n");

/* Family F057: one body three times, differing only in which bit of
 * gActiveMap->flags it tests (0x200 / 0x400 / 0x800) and which value it
 * stores into ->unk10 (0 / 1 / 2). data/families.json's `varies` has exactly
 * those two entries; everything else -- both callees, the script global and
 * the trailing `unk02 = 7` -- is shared by all three members.
 *
 * The bit test is a plain mask on the halfword and NOT a bitfield: a 1-bit
 * field at bit 9 of a u16 container would be read `ldrb [p,#1]` plus a double
 * shift, because get_best_mode picks the narrowest mode containing the field.
 * The ROM does `ldrh [p]` and ANDs a computed constant, so the two are
 * distinguishable here and the mask is the answer.
 *
 * `u8` third parameter: the entry `lsls #0x18; lsrs #0x18` is PROMOTE_MODE,
 * and there is no second sign-extending pair at the `cmp r2, #2`, which rules
 * out s8. The first two parameters are never read -- nothing in the ROM calls
 * any of the three, so the arity is a floor of three and `int` for the two
 * dead ones is the byte-neutral choice.
 *
 * The `if (A && B) { ... }` and the two-early-returns spellings are
 * byte-identical here (probed): there is only one exit block, so the
 * discriminator in docs/agbcc-codegen.md's two-guards rule does not apply. */

void DesignRoomMenu_PickSlot2(int a, int b, u8 c)
{
    if ((gActiveMap->flags & 0x800) && c != 2)
    {
        CloseTopMenu();
        gActiveMap->designSlot = 2;
        sub_080152EC(gUnknown_08487E14, 0);
        gActiveMap->state = 7;
    }
}
asm(".global sub_08004F5C\n.thumb_set sub_08004F5C, DesignRoomMenu_PickSlot2\n");
