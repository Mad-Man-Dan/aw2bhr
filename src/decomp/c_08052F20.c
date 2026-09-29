#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052F20.
 * sub_08052F20 @ 0x08052F20
 */

/* Five statements. None of the callees reads an argument register, so no
 * result is flowing between them. */

void BattleAnimParamMenu_Init(void)
{
    StopAllMusic();
    sub_080546F0();
    ResetBattleAnimParams();
    SetUpBattleAnimDisplay();
    sub_080116E8();
}
asm(".global sub_08052F20\n.thumb_set sub_08052F20, BattleAnimParamMenu_Init\n");
