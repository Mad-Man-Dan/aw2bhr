#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BB44.
 * sub_0803BB44 @ 0x0803BB44, sub_0803BB5C @ 0x0803BB5C, sub_0803BB74 @ 0x0803BB74
 */

/* gUnknown_03003F30[] is s8, so the int-context read is
 * `ldrb; lsl #24; asr #24`. The return is u8: the caller MainMenuCampaign_ContinueUsability narrows
 * the result with `lsls #24; lsrs #24` before comparing it against 2, which an
 * int-returning callee would not produce. The `if`/`return` pair is not
 * interchangeable with the ternary -- `x != 0 ? 2 : 1` compiles to a
 * conditional `mov` into r1 plus `adds r0, r1, #0`, four bytes shorter. */
u8 MainMenuCampaign_Usability(void)
{
    if (gUnknown_03003F30[1] != 0)
        return 2;

    return 1;
}
asm(".global sub_0803BB44\n.thumb_set sub_0803BB44, MainMenuCampaign_Usability\n");

/* See MainMenuCampaign_Usability -- same accessor, element 2. */
u8 MainMenuWarRoom_Usability(void)
{
    if (gUnknown_03003F30[2] != 0)
        return 2;

    return 1;
}
asm(".global sub_0803BB5C\n.thumb_set sub_0803BB5C, MainMenuWarRoom_Usability\n");

/* See MainMenuCampaign_Usability -- same accessor, element 3. */
u8 MainMenuVersus_Usability(void)
{
    if (gUnknown_03003F30[3] != 0)
        return 2;

    return 1;
}
asm(".global sub_0803BB74\n.thumb_set sub_0803BB74, MainMenuVersus_Usability\n");
