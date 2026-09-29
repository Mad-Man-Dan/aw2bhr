#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B9B8.
 * sub_0803B9B8 @ 0x0803B9B8
 */

/* A mode entry point, and the fourth of the family the gPlaySt.gameMode
 * comment in include/unknown-globals.h already lists: write the mode selector,
 * then start the mode. StartCampaignAfterMap / StartWarRoom / sub_0803B8C4 write 1 / 2 /
 * 3 and Proc_Start a script directly; this one writes 5 and goes through two
 * forwarders instead -- FadeOutMusicDefault (the `FadeOutMusic(2)` BGM fade) and
 * StartDesignRoom(0).
 * `strb r0, [r1, #1]` on the struct base is the ordinary `.unk01` store, not an
 * array index. `pop {r0}; bx r0` -> void. */

void MainMenu_StartDesignRoom(void)
{
    gPlaySt.gameMode = 5;

    FadeOutMusicDefault();
    StartDesignRoom(0);
}
asm(".global sub_0803B9B8\n.thumb_set sub_0803B9B8, MainMenu_StartDesignRoom\n");
