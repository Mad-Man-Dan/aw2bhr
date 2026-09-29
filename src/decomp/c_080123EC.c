#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080123EC.
 * sub_080123EC @ 0x080123EC
 */

#include "hardware.h"

/* Forced blank, then the three display-shadow clears. The DISPCNT shadow is
 * written whole as 0x80 (forced_blank alone), which no single `.bits`
 * assignment produces -- hence the `*(u16 *)&` cast, the same spelling
 * src/decomp/c_08012420.c uses on the same member. */
void ResetDisplayState(void)
{
    *(u16 *)&gDispIo.disp_ct = 0x80;
    gUnknown_030020B4.raw = 0;
    gUnknown_03001FD0 = 0;

    ResetBgShadows();
    SetDefaultColorEffects();
    ResetWindowShadows();
    ResetScreenShakeOffset();
}
asm(".global sub_080123EC\n.thumb_set sub_080123EC, ResetDisplayState\n");
