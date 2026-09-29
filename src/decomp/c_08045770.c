#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045770.
 * sub_08045770 @ 0x08045770
 */

/* One of three identical wrappers -- sub_08038548 and EndOfGame_FinishVersusMap are the
 * others, and the only thing that changes between them is the callback.
 * The `lsls #24; lsrs #24` between the two calls is the s8 -> u8 conversion
 * of GetSuspendIdForGameMode's result for StartSaveScreen's `u8` first parameter; see the
 * prototypes in include/unknown-functions.h for how both widths were settled.
 * `pop {r0}; bx r0` -- void.
 */
void StartSaveScreenForGameMode(void)
{
    StartSaveScreen(GetSuspendIdForGameMode(gPlaySt.gameMode), StartMainMenuAfterProgressLoad);
}
asm(".global sub_08045770\n.thumb_set sub_08045770, StartSaveScreenForGameMode\n");
