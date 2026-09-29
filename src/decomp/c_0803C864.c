#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C864.
 * sub_0803C864 @ 0x0803C864
 */

/* A two-argument indirect call: `bl _call_via_r2` is ordinary agbcc output on
 * ARMv4T (no `blx rN`), and the trampoline's register index counts the
 * arguments. The ROM names the callee -- row 0 and row 1 of the table both
 * hold 0x0803CBA1, i.e. &SetCampaignCompletionFlag with the THUMB bit set -- which is what
 * fixes unk0c as `void (*)(int, int)`. */
void GrantShopItem(u8 a)
{
    gUnknown_0849EDB0[a].unk0c(gUnknown_0849EDB0[a].unk10, 1);
}
asm(".global sub_0803C864\n.thumb_set sub_0803C864, GrantShopItem\n");
