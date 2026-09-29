#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038548.
 * sub_08038548 @ 0x08038548
 */

/* MATCHED. A LOOSE duplicate of the promoted StartSaveScreenForGameMode -- same instruction
 * stream, one pool word different (`StartWarRoom` for `StartMainMenuAfterProgressLoad`), so the
 * body is that function's with one symbol swapped and it cost one line.
 *
 * It is also the first free match the wave-20 screen produced that did not
 * exist when the screen was first run: StartSaveScreenForGameMode was matched during the
 * wave. See the wave-21 list. */
void sub_08038548(void)
{
    StartSaveScreen(GetSuspendIdForGameMode(gPlaySt.gameMode), StartWarRoom);
}
