#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D6B8.
 * sub_0803D6B8 @ 0x0803D6B8, sub_0803D6D0 @ 0x0803D6D0, sub_0803D6FC @ 0x0803D6FC
 */

/* Describes only the pointer parameter -- the caller hands over
 * gUnknown_02000000, which no header declares. Five bytes at +0x4C4 are the
 * per-slot table that also lives in gUnknown_030040F8. */
struct Unk3D6FC
{
    /* 0x000 */ u8 filler_000[0x4c4];
    /* 0x4c4 */ u8 unk4c4[5];
};

void ResetTeamColorSnapshot(void)
{
    u8 *base;
    u8 *p;
    int i;

    base = gUnknown_030040F8;
    i = 4;
    p = base + 4;

    do
    {
        *p = i;
        p--;
        i--;
    } while (i >= 0);
}
asm(".global sub_0803D6B8\n.thumb_set sub_0803D6B8, ResetTeamColorSnapshot\n");

void SnapshotTeamColorsFromPlayers(void)
{
    int i;

    for (i = 0; i < 5; i++)
        gUnknown_030040F8[i] = gPlayers[i].teamColor;
}
asm(".global sub_0803D6D0\n.thumb_set sub_0803D6D0, SnapshotTeamColorsFromPlayers\n");

void SnapshotTeamColorsFromRecord(struct Unk3D6FC *s)
{
    int i;

    for (i = 0; i < 5; i++)
        gUnknown_030040F8[i] = s->unk4c4[i];
}
asm(".global sub_0803D6FC\n.thumb_set sub_0803D6FC, SnapshotTeamColorsFromRecord\n");
