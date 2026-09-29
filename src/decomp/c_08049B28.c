#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049B28.
 * sub_08049B28 @ 0x08049B28
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

void ShopScreen_EndMessageScripts(void)
{
    sub_080733B8();
    EndEventScript(gUnknown_084C30E8[gUnknown_084C30F8->unk83a]);
    sub_0801537C(gUnknown_084C3128);

    if (gUnknown_0200C420.unk0f != 0xFF)
        gUnknown_0200C420.unk0f++;
}

asm(".global sub_08049B28\n.thumb_set sub_08049B28, ShopScreen_EndMessageScripts\n");
