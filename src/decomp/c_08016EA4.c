#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016EA4.
 * sub_08016EA4 @ 0x08016EA4, sub_08016ED8 @ 0x08016ED8
 */

/* The copy reaches through a POINTER (`p->unk000.unk08`), which is why its
 * address add is base-first while the store into the global is index-first.
 * See the operand-order chapter in docs/agbcc-codegen.md. */
void ReloadCampaignFlagBank2FromProfile(void)
{
    struct Unk08016B2C *p = (struct Unk08016B2C *)gUnknown_02000000;
    int i;

    if (ReadSaveSlot(0, gUnknown_02000000) != 0)
        return;
    for (i = 0; i <= 7; i++)
        gUnknown_02028030.unk08[i] = p->unk000.unk08[i];
}
asm(".global sub_08016EA4\n.thumb_set sub_08016EA4, ReloadCampaignFlagBank2FromProfile\n");

/* The 0xa8-byte reload is a WORD LOOP over an index range (0xe..0x37), not a
 * struct assignment: an aggregate copy that size compiles to `bl memcpy`, and
 * one of a named member would fold the +0x38 into the address constant. The
 * index range is what keeps the pool word at the bare `&gUnknown_0200C420` the
 * unk09 store below reloads, and what puts the 0x38 in the pointer instead of
 * the load displacement. The region is gUnknown_0200C420's tail, which
 * src/decomp/c_0803BA1C.c already types as `u32 unk38` plus filler -- hence the
 * cast here rather than a member array. */
void ReloadProgressFromProfile(void)
{
    struct Unk08016B2C *p = (struct Unk08016B2C *)gUnknown_02000000;
    int i;

    if (ReadSaveSlot(0, gUnknown_02000000) != 0)
        return;
    ReloadCampaignFlagBank2FromProfile();
    for (i = 0xe; i <= 0x37; i++)
        ((u32 *)&gUnknown_0200C420)[i] = ((u32 *)&p->unk3f0)[i];
    RestoreBattleMapPoints();
    gUnknown_0200C420.unk09 = p->unk3f0.unk09;
    sub_0808B6E8(&gUnknown_0202FDFC, p->unk4d0, 0xfc);
}
asm(".global sub_08016ED8\n.thumb_set sub_08016ED8, ReloadProgressFromProfile\n");
