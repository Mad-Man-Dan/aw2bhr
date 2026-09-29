#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019940.
 * sub_08019940 @ 0x08019940
 */

/* Run RecordArmyDefeat for every army 1..4 whose gPlayers[i].unk2a
 * differs from army `a`'s and which passes IsPlayerAliveAndActive, then close out with
 * FinalizeMatchResult and post mode 0x12.
 *
 * gPlayers is a POINTER global, so the ROM's two `ldr`s are the
 * `-fforce-addr` `.rodata` word (0x0808E5B4, which holds 0x08499598) followed
 * by the pointer load; both stay INSIDE the loop because the calls in the body
 * may repoint it. `a`'s element offset (`a * 0x3c`, as `(a << 4) - a << 2`) is
 * the only thing LICM hoists.
 *
 * `i` is `u8`: the ROM re-truncates it `lsls #0x18; lsrs #0x18` at the bottom
 * and the bound is `cmp #4; bls`, an unsigned test. The `&&` really is one --
 * both halves branch to the same skip label and the second is a call, so
 * there is no arm to invert. */
void DefeatOtherTeamsAndEndMatch(u8 a, u8 b)
{
    u8 i;

    for (i = 1; i <= 4; i++)
    {
        if (gPlayers[i].team != gPlayers[a].team
         && IsPlayerAliveAndActive(i))
            RecordArmyDefeat(i, b);
    }
    FinalizeMatchResult();
    gUnknown_030032D8 = 0x12;
}
asm(".global sub_08019940\n.thumb_set sub_08019940, DefeatOtherTeamsAndEndMatch\n");
