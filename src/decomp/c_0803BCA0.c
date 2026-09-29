#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BCA0.
 * sub_0803BCA0 @ 0x0803BCA0, sub_0803BCB8 @ 0x0803BCB8
 */

/* Family F060. NOT a VRAM-copy family: only sub_080059E4 talks to VRAM.
 * data/families.json's `varies` lists both pool words and both callees, and
 * the three members split two ways -- sub_080059E4 queues a ROM blob for VRAM
 * through the deferred-copy push RegisterDataMove, while BackupCampaignFlags and
 * RestoreCampaignFlags are a memcpy save/restore pair over two adjacent 0x48-byte
 * IWRAM structs and touch no hardware at all.
 *
 * `movs r2, #0x40` / `#0x48` needs no shift because both fit an imm8; that is
 * the only thing separating this family from F063 and it is a spelling
 * consequence of the constant, not a type one. */

void BackupCampaignFlags(void)
{
    sub_0808B6E8(&gUnknown_02028078, &gUnknown_02028030, 0x48);
}
asm(".global sub_0803BCA0\n.thumb_set sub_0803BCA0, BackupCampaignFlags\n");

/* Family F060. NOT a VRAM-copy family: only sub_080059E4 talks to VRAM.
 * data/families.json's `varies` lists both pool words and both callees, and
 * the three members split two ways -- sub_080059E4 queues a ROM blob for VRAM
 * through the deferred-copy push RegisterDataMove, while BackupCampaignFlags and
 * RestoreCampaignFlags are a memcpy save/restore pair over two adjacent 0x48-byte
 * IWRAM structs and touch no hardware at all.
 *
 * `movs r2, #0x40` / `#0x48` needs no shift because both fit an imm8; that is
 * the only thing separating this family from F063 and it is a spelling
 * consequence of the constant, not a type one. */

void RestoreCampaignFlags(void)
{
    sub_0808B6E8(&gUnknown_02028030, &gUnknown_02028078, 0x48);
}
asm(".global sub_0803BCB8\n.thumb_set sub_0803BCB8, RestoreCampaignFlags\n");
