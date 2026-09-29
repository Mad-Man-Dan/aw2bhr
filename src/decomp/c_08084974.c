#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084974.
 * sub_08084974 @ 0x08084974
 */

#include "hardware.h"

/* The five `orrs`/`ands` on byte 1 of gDispIo are exactly hardware.h's
 * SetDispEnable macro -- one bitfield store per BG plus OBJ, in that order --
 * and the three that follow clear the two windows and the OBJ window. */
void MainMenuCarousel_ShowOverwriteWarning(void)
{
    SetDispEnable(0, 1, 1, 1, 1);
    gDispIo.disp_ct.win0_enable = 0;
    gDispIo.disp_ct.win1_enable = 0;
    gDispIo.disp_ct.objwin_enable = 0;
    LoadBg1WindowFrame(0);
    StartCoSpeechScript(0xca0, 0, 0);
}
asm(".global sub_08084974\n.thumb_set sub_08084974, MainMenuCarousel_ShowOverwriteWarning\n");
