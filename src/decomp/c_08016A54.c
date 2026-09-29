#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016A54.
 * sub_08016A54 @ 0x08016A54, sub_08016B2C @ 0x08016B2C, sub_08016BC0 @ 0x08016BC0
 */

/* The two table loops clear one BITFIELD word per record -- three assignments,
 * `strb` / `and 0xFFF000FF` / `and 0xF` on the upper halfword -- see
 * struct Unk0200C078Rec in include/unknown-globals.h. */
void ResetProfileToDefaults(void)
{
    int i, j;

    gUnknown_0200C420.unk00 = 0;
    gUnknown_0200C420.unk04 = 0;
    gUnknown_0200C420.unk0c = 0;
    gUnknown_0200C420.unk0d = 0;
    gUnknown_0200C420.unk0e = 1;
    gUnknown_0200C420.unk14 = 0;
    gUnknown_0200C420.unk09 = 0;
    gUnknown_0200C420.unk0a = 0;
    gUnknown_0200C420.unk0b = 0;
    gUnknown_0200C420.unk0f = 0;
    for (i = 0; i <= 0x12; i++)
        SetLoadedCoPalette(i, 0);
    ClearCampaignFlags60To9F();
    gUnknown_0200C420.unk10 = 0;
    gUnknown_0200C420.unk12 = 0;
    for (i = 0; i <= 0x1d; i++) {
        for (j = 0; j <= 4; j++) {
            gUnknown_0200C078[i].unk00[j].unk00_00 = 0;
            gUnknown_0200C078[i].unk00[j].unk00_08 = 0;
            gUnknown_0200C078[i].unk00[j].unk00_14 = 0;
        }
    }
    for (i = 0; i <= 0x29; i++) {
        for (j = 0; j <= 1; j++) {
            gUnknown_0200C2D0[i].unk00[j].unk00_00 = 0;
            gUnknown_0200C2D0[i].unk00[j].unk00_08 = 0;
            gUnknown_0200C2D0[i].unk00[j].unk00_14 = 0;
        }
    }
    ResetCampaignUnlocks();
    gUnknown_0200C420.unk08 = 0;
}
asm(".global sub_08016A54\n.thumb_set sub_08016A54, ResetProfileToDefaults\n");

/* The inner bound is 4 in BOTH loops -- five words -- while the second table's
 * stride is only 8 bytes. That overrun is the original's bug; see
 * struct Unk08016B2C in include/unknown-globals.h. */
int PackProfileRecord(void *arg)
{
    struct Unk08016B2C *p = arg;
    int i, j;

    for (i = 0; i <= 0x1d; i++)
        for (j = 0; j <= 4; j++)
            p->unk048[i].unk00[j] = gUnknown_0200C078[i].unk00[j];
    for (i = 0; i <= 0x29; i++)
        for (j = 0; j <= 4; j++)
            p->unk2a0[i].unk00[j] = gUnknown_0200C2D0[i].unk00[j];
    sub_0808B6E8(&p->unk3f0, &gUnknown_0200C420, 0xe0);
    sub_0808B6E8(p, &gUnknown_02028030, 0x48);
    sub_0808B6E8(p->unk4d0, &gUnknown_0202FDFC, 0xfc);
    return 0x5CC;
}
asm(".global sub_08016B2C\n.thumb_set sub_08016B2C, PackProfileRecord\n");

/* PackProfileRecord's mirror, including its five-words-per-8-byte-row overrun. */
int UnpackProfileRecord(void *arg)
{
    struct Unk08016B2C *p = arg;
    int i, j;

    for (i = 0; i <= 0x1d; i++)
        for (j = 0; j <= 4; j++)
            gUnknown_0200C078[i].unk00[j] = p->unk048[i].unk00[j];
    for (i = 0; i <= 0x29; i++)
        for (j = 0; j <= 4; j++)
            gUnknown_0200C2D0[i].unk00[j] = p->unk2a0[i].unk00[j];
    sub_0808B6E8(&gUnknown_0200C420, &p->unk3f0, 0xe0);
    if (gUnknown_0200C420.unk0e > 3)
        gUnknown_0200C420.unk0e = 1;
    if (gUnknown_0200C420.unk14 > 1)
        gUnknown_0200C420.unk14 = 0;
    sub_0808B6E8(&gUnknown_02028030, p, 0x48);
    sub_0808B6E8(&gUnknown_0202FDFC, p->unk4d0, 0xfc);
    return 0x5CC;
}
asm(".global sub_08016BC0\n.thumb_set sub_08016BC0, UnpackProfileRecord\n");
