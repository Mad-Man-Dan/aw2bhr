#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C2B4.
 * sub_0802C2B4 @ 0x0802C2B4
 */

#include "hardware.h"

/* An install-then-disable pair: start the gUnknown_0849A990 script (the literal
 * 0 second argument every other sub_080152EC wrapper passes) and clear BG0 in
 * the DISPCNT shadow.
 *
 * Byte 1 bit 0 of gDispIo is DISPCNT bit 8, i.e. disp_ct.bg0_enable. The mask
 * arrives as `movs r0,#2; rsbs r0,r0,#0` -- the SImode -2 -- and not as a
 * `movs r0,#0xfe`, because the bitfield container is u16 and the complement is
 * formed before the byte-wide read-modify-write is narrowed. MinimapScreen_FadeIn in
 * this same block is the setting half of the pair and emits the expected
 * `movs r1,#1; orrs`, which is what makes bit 0 of byte 1 the discriminating
 * use rather than a byte-neutral guess.
 *
 * sub_080152EC's returned slot is discarded (`pop {r0}; bx r0`), so this is
 * void and the call is a bare statement.
 */

void OpenMinimap(void)
{
    sub_080152EC(gUnknown_0849A990, 0);
    gDispIo.disp_ct.bg0_enable = 0;
}
asm(".global sub_0802C2B4\n.thumb_set sub_0802C2B4, OpenMinimap\n");
