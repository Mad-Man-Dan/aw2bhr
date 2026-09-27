#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080283E4.
 * DebugVersusPauseScreen @ 0x080283E4
 *
 * Not a Xenesis-documented name. The old sub_080283E4 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

#include "hardware.h"

/* The debug versus-mode pause screen: a text overlay drawn with sub_08013428
 * that lets the tester flip fog of war ("SAKUTEKI") and choose whether player 1
 * and player 2 are human ("1P"/"2P") or computer ("CP").
 *
 *   A / Start      apply: copy the 1P/CP choices into gPlayers and resume
 *                  (sub_08015C30 with gUnknown_03001FBC)
 *   R              toggle fog of war
 *   Left / Right   move the "--" cursor between the two player columns
 *   Up / Down      toggle the column under the cursor between human (1) and
 *                  computer (2)
 *
 * The cursor column is unk38 of this screen's gUnknown_03001470 entry, read as
 * s16.
 *
 * Measured spelling notes:
 * - The fog strings have TWO spaces before ON/OFF, as in the ROM. With one,
 *   "1P" and everything after it in .rodata lands 4 bytes early and the pool
 *   words cannot be placed. That was the last residual of the wave-36 draft,
 *   which had put it down to a missing padding word.
 * - The Up/Down toggle goes through `play = &gPlaySt;` with the cursor in its
 *   own local. The pointer is load-bearing: it creates the &gPlaySt register
 *   before the slot address, and the toggle reuses it. Written through
 *   gPlaySt directly, the function is 4 bytes long. Byte-offset spellings off
 *   `(u8 *)&gPlaySt` either reverse one add's operands or cost 4 bytes.
 * - Players 1 and 2 are named as `gPlayers[n].aiControlled`. The old
 *   `((u8 *)gPlayers)[0x57]` form loaded gPlaySt before gPlayers (90.7%).
 * - Left/Right writes the subscript inline on both sides. The "--" line
 *   needs `slot` bound first: inline, the +0x38 folds into the relocation
 *   addend (W36-M, W73-F).
 */
void DebugVersusPauseScreen(void)
{
    struct Unk03001470 *slot;
    int cursor;
    struct PlaySt *play;

    if (gpKeySt->pressed & (A_BUTTON | START_BUTTON))
    {
        gPlayers[1].aiControlled = gPlaySt.aiControlled[1];
        gPlayers[2].aiControlled = gPlaySt.aiControlled[2];
        sub_08015C30(gUnknown_03001FBC);
    }

    if (gpKeySt->pressed & R_BUTTON)
        gPlaySt.fog = 1 - gPlaySt.fog;

    if (gpKeySt->pressed & (DPAD_RIGHT | DPAD_LEFT))
        gUnknown_03001470[gUnknown_03001FBC].unk38
            = 1 - gUnknown_03001470[gUnknown_03001FBC].unk38;

    if (gpKeySt->pressed & (DPAD_UP | DPAD_DOWN))
    {
        play = &gPlaySt;
        slot = &gUnknown_03001470[gUnknown_03001FBC];
        cursor = *(s16 *)&slot->unk38;
        if (play->aiControlled[cursor + 1] == 1)
            play->aiControlled[cursor + 1] = 2;
        else
            play->aiControlled[cursor + 1] = 1;
    }

    if (gPlaySt.fog == 1)
        sub_08013428(8, 13, "R: SAKUTEKI  ON");
    else
        sub_08013428(8, 13, "R: SAKUTEKI  OFF");

    if (gPlaySt.aiControlled[1] == 1)
        sub_08013428(11, 10, "1P");
    else
        sub_08013428(11, 10, "CP");

    if (gPlaySt.aiControlled[2] == 1)
        sub_08013428(17, 10, "2P");
    else
        sub_08013428(17, 10, "CP");

    sub_08013428(14, 10, "VS");

    slot = &gUnknown_03001470[gUnknown_03001FBC];
    sub_08013428((s16)(*(s16 *)&slot->unk38 * 6 + 11), 11, "--");
    sub_08013428(12, 6, "PAUSE");
}

asm(".global sub_080283E4\n.thumb_set sub_080283E4, DebugVersusPauseScreen\n");
