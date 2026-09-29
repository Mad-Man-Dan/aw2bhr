#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080848B4.
 * sub_080848B4 @ 0x080848B4, sub_080848D8 @ 0x080848D8, sub_080848FC @ 0x080848FC
 */

/* One of the six predicates in gUnknown_08616B00, and AreAllWarRoomMapsRecorded's twin:
 * "scan a range, return FALSE the moment one record's 12-bit field at bit 8 is
 * zero". The counter runs over a GLOBAL id range (0x8a..0xab) while the record
 * pointer starts at gUnknown_0200C2D0[0], so the subscript carries the bias --
 * `i - 0x8a` -- and strength reduction turns it into the `adds r1, #8` giv with
 * the pool word at addend 0. The loop is NOT reversed by check_dbra_loop
 * despite `i` surviving only in the exit test, because the early `return FALSE`
 * gives it two exits; MainMenuCarousel_UpdateCompletionFlags in the same block has one exit and IS
 * reversed. */

bool8 AreAllCampaignMissionsRecorded(void)
{
    int i;

    for (i = 0x8a; i <= 0xab; i++)
        if (gUnknown_0200C2D0[i - 0x8a].unk00[0].unk00_08 == 0)
            return FALSE;
    return TRUE;
}
asm(".global sub_080848B4\n.thumb_set sub_080848B4, AreAllCampaignMissionsRecorded\n");

/* AreAllCampaignMissionsRecorded's twin over the other unit table: id range 0x6c..0x89 (the same
 * 30 ids HaveAllMapsBeenPlayed's second loop walks) against gUnknown_0200C078's 0x14
 * stride. Same bias-in-the-subscript reading -- see the comment on
 * AreAllCampaignMissionsRecorded. */

bool8 AreAllWarRoomMapsRecorded(void)
{
    int i;

    for (i = 0x6c; i <= 0x89; i++)
        if (gUnknown_0200C078[i - 0x6c].unk00[0].unk00_08 == 0)
            return FALSE;
    return TRUE;
}
asm(".global sub_080848D8\n.thumb_set sub_080848D8, AreAllWarRoomMapsRecorded\n");

/* The call variant of AreAllCampaignMissionsRecorded's shape: every id 0..0x12 must pass
 * IsCoSelectable's bitset test. The bare `lsls r0, r0, #0x18` truth test is
 * IsCoSelectable's declared `u8` return, not a cast. */

bool8 AreAllCosSelectable(void)
{
    int i;

    for (i = 0; i <= 0x12; i++)
        if (!IsCoSelectable(i))
            return FALSE;
    return TRUE;
}
asm(".global sub_080848FC\n.thumb_set sub_080848FC, AreAllCosSelectable\n");
