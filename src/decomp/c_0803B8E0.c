#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B8E0.
 * sub_0803B8E0 @ 0x0803B8E0, sub_0803B904 @ 0x0803B904
 */

/* gUnknown_03003F30 is already typed `s8 []`, and this is the tell: the byte is
 * read `ldrb` then re-signed with `lsls #24; asrs #24` before the zero test,
 * which is what a signed char element costs and what a u8 would not emit. */
void MainMenuVersus_Continue(void)
{
    if (gUnknown_03003F30[3] != 0)
        StartResumeScript(4);
    else
        sub_0803B8C4();
}
asm(".global sub_0803B8E0\n.thumb_set sub_0803B8E0, MainMenuVersus_Continue\n");

/* Same guard as MainMenuVersus_Continue one element down, but the argument is computed
 * rather than literal, and the three-shift run `lsls #0x18; asrs #8; lsrs #0x10`
 * is exactly the two conversions folded together: GetSuspendIdForGameMode returns s8, so
 * its result is re-narrowed at the call site (`lsls #24; asrs #24`), and
 * StartResumeScript's parameter is u16, so it is then zero-extended (`lsls #16;
 * lsrs #16`). agbcc collapses the adjacent `asrs #24; lsls #16` into `asrs #8`.
 * Nothing here is a mask plus a shift. */
void MainMenuWarRoom_Continue(void)
{
    if (gUnknown_03003F30[2] != 0)
        StartResumeScript(GetSuspendIdForGameMode(2));
    else
        StartWarRoom();
}
asm(".global sub_0803B904\n.thumb_set sub_0803B904, MainMenuWarRoom_Continue\n");
