#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071C84.
 * sub_08071C84 @ 0x08071C84, sub_08071CA4 @ 0x08071CA4
 */

#include "hardware.h"

/* Restores one 16-colour bank: `lsls #5` is the byte stride of a palette bank
 * and gPal is `u16 []`, so the destination is &gPal[a * 16] and the count 0x10
 * is in halfwords.
 *
 * This is the reader that settles what gUnknown_08613F54 IS. StartPalFade
 * passes the same symbol as its first argument and stashes it in a proc field,
 * which had been read as a proc script; a proc script would not survive being
 * CpuSet into palette RAM. See include/unknown-functions.h.
 *
 * SetWhitePal is the same function over gUnknown_08613F74. */
void SetBlackPal(int a)
{
    CpuSet(gUnknown_08613F54, &gPal[a * 16], 0x10);
}
asm(".global sub_08071C84\n.thumb_set sub_08071C84, SetBlackPal\n");

/* SetBlackPal's twin over the other palette -- byte-identical apart from the
 * pool symbol, and each member's own data_refs were re-read before
 * transcribing. See SetBlackPal. */
void SetWhitePal(int a)
{
    CpuSet(gUnknown_08613F74, &gPal[a * 16], 0x10);
}
asm(".global sub_08071CA4\n.thumb_set sub_08071CA4, SetWhitePal\n");
