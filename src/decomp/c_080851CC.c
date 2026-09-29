#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080851CC.
 * sub_080851CC @ 0x080851CC, sub_08085208 @ 0x08085208
 */

/* Twin of CoInfoScreen_DrawSuperPowerPage; the only difference is DrawOamObject's first argument
 * (0x13 vs 0x14).
 *
 * The parameter is a raw `s16 *`, the same vocabulary the matched neighbour
 * src/decomp/c_080859E0.c uses (`void CoInfoPopup_Init(u16 *p) { p[0x32] = 0; }`).
 * A struct tag was deliberately NOT invented: +0x66 is read twice with two
 * different widths -- `ldrb [r4]` for IsPlayerAliveAndActive's u8 parameter and
 * `movs r0,#0; ldrsh r2,[r4,r0]` for DrawArmyCoPanel's int one -- and both are the
 * one s16 element `p[0x33]`, the ldrb being the prototype's narrowing folded
 * into the load. `adds r4, #0x66` is computed once and shared because THUMB has
 * no ldrsh immediate form and 0x66 is past ldrb's imm5 limit.
 *
 * CoInfoScreen_DrawArmyIcons is nullary (it never reads r0), so the proc pointer still
 * sitting in r0 at that `bl` is not argument setup. */
void CoInfoScreen_DrawPowerPage(s16 *p)
{
    CoInfoScreen_DrawArmyIcons();
    DrawOamObject(0x13, 8, 0x28, 0, 1);

    if (IsPlayerAliveAndActive(p[0x33]) != 0)
        DrawArmyCoPanel(0x98, 0x70, p[0x33]);
}
asm(".global sub_080851CC\n.thumb_set sub_080851CC, CoInfoScreen_DrawPowerPage\n");

/* Twin of CoInfoScreen_DrawPowerPage; see that function for the derivation. The only
 * difference is DrawOamObject's first argument (0x14 here, 0x13 there). */
void CoInfoScreen_DrawSuperPowerPage(s16 *p)
{
    CoInfoScreen_DrawArmyIcons();
    DrawOamObject(0x14, 8, 0x28, 0, 1);

    if (IsPlayerAliveAndActive(p[0x33]) != 0)
        DrawArmyCoPanel(0x98, 0x70, p[0x33]);
}
asm(".global sub_08085208\n.thumb_set sub_08085208, CoInfoScreen_DrawSuperPowerPage\n");
