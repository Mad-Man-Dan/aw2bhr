#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C890.
 * sub_0803C890 @ 0x0803C890
 */

/* Fire every gUnknown_0849EDB0 row's unk0c callback with a 1, skipping the
 * rows whose unk04 is 0xffff or above. Two different sentinels on the same
 * field: -1 ends the list (signed `cmp` against a materialised -1) and 0xffff
 * is a "no entry" marker inside it, tested UNSIGNED (`ldr r0,=0xfffe;
 * cmp r1,r0; bhi`), which is why the cast sits on the compare and not on the
 * field -- unk04 is `int` at every other reader.
 *
 * `i` is a u8 -- the `lsl #0x18; lsr #0x18` on the increment -- so the row
 * offset `i * 0x18` is recomputed from scratch each iteration instead of
 * becoming a giv. Same loop as ResetCampaignUnlocks's last one, which has no gate and
 * therefore one spare register, and so hoists `base + 0x10` where this one
 * does not. */

void GrantAllShopItems(void)
{
    u8 i;

    for (i = 0; gUnknown_0849EDB0[i].unk04 != -1; i++)
    {
        if ((u32)gUnknown_0849EDB0[i].unk04 < 0xffff)
            gUnknown_0849EDB0[i].unk0c(gUnknown_0849EDB0[i].unk10, 1);
    }
}
asm(".global sub_0803C890\n.thumb_set sub_0803C890, GrantAllShopItems\n");
