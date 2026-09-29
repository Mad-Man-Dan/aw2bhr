#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BAFC.
 * sub_0803BAFC @ 0x0803BAFC, sub_0803BB14 @ 0x0803BB14, sub_0803BB2C @ 0x0803BB2C
 */

/* F081 -- `MainMenuVersus_Usability` returns 1 or 2 and this asks whether it is 2.
 *
 * It is NOT `return MainMenuVersus_Usability() == 2;`. That spelling is a byte shorter: agbcc
 * expands a returned comparison through do_store_flag, which presets the false
 * value in a scratch register and conditionally overwrites it (`mov r1,#0;
 * ... bne; mov r1,#1; add r0,r1,#0`) with no unconditional branch. The
 * original has both arms and the `b`, which is an explicit if/else whose arms
 * each return. The `beq`-to-the-1-arm polarity comes out of the `== 2` form
 * with the 1 in the `then` -- gcc lays the else arm down first.
 *
 * The `lsls #0x18; lsrs #0x18` between the `bl` and the `cmp` is agbcc
 * re-narrowing a callee declared to return u8; it is not a cast in this
 * function. See the note on MainMenuVersus_Usability in include/unknown-functions.h.
 */
int MainMenuVersus_ContinueUsability(void)
{
    if (MainMenuVersus_Usability() == 2)
        return 1;
    else
        return 0;
}
asm(".global sub_0803BAFC\n.thumb_set sub_0803BAFC, MainMenuVersus_ContinueUsability\n");

/* F081 -- `MainMenuCampaign_Usability` returns 1 or 2 and this asks whether it is 2.
 *
 * It is NOT `return MainMenuCampaign_Usability() == 2;`. That spelling is a byte shorter: agbcc
 * expands a returned comparison through do_store_flag, which presets the false
 * value in a scratch register and conditionally overwrites it (`mov r1,#0;
 * ... bne; mov r1,#1; add r0,r1,#0`) with no unconditional branch. The
 * original has both arms and the `b`, which is an explicit if/else whose arms
 * each return. The `beq`-to-the-1-arm polarity comes out of the `== 2` form
 * with the 1 in the `then` -- gcc lays the else arm down first.
 *
 * The `lsls #0x18; lsrs #0x18` between the `bl` and the `cmp` is agbcc
 * re-narrowing a callee declared to return u8; it is not a cast in this
 * function. See the note on MainMenuCampaign_Usability in include/unknown-functions.h.
 */
int MainMenuCampaign_ContinueUsability(void)
{
    if (MainMenuCampaign_Usability() == 2)
        return 1;
    else
        return 0;
}
asm(".global sub_0803BB14\n.thumb_set sub_0803BB14, MainMenuCampaign_ContinueUsability\n");

/* F081 -- `MainMenuWarRoom_Usability` returns 1 or 2 and this asks whether it is 2.
 *
 * It is NOT `return MainMenuWarRoom_Usability() == 2;`. That spelling is a byte shorter: agbcc
 * expands a returned comparison through do_store_flag, which presets the false
 * value in a scratch register and conditionally overwrites it (`mov r1,#0;
 * ... bne; mov r1,#1; add r0,r1,#0`) with no unconditional branch. The
 * original has both arms and the `b`, which is an explicit if/else whose arms
 * each return. The `beq`-to-the-1-arm polarity comes out of the `== 2` form
 * with the 1 in the `then` -- gcc lays the else arm down first.
 *
 * The `lsls #0x18; lsrs #0x18` between the `bl` and the `cmp` is agbcc
 * re-narrowing a callee declared to return u8; it is not a cast in this
 * function. See the note on MainMenuWarRoom_Usability in include/unknown-functions.h.
 */
int MainMenuWarRoom_ContinueUsability(void)
{
    if (MainMenuWarRoom_Usability() == 2)
        return 1;
    else
        return 0;
}
asm(".global sub_0803BB2C\n.thumb_set sub_0803BB2C, MainMenuWarRoom_ContinueUsability\n");
