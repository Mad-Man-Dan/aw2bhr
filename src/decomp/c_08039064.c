#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039064.
 * sub_08039064 @ 0x08039064, sub_080390CC @ 0x080390CC
 */

u8 GetMovePathIncomingDirection(u8 n)
{
    struct Unk0849D5F8 *p;

    if (n == 0)
        return 0;

    p = gUnknown_0849D5F8;

    if (p->unk20[n - 1] < p->unk20[n])
        return 3;
    if (p->unk20[n - 1] > p->unk20[n])
        return 1;
    if (p->unk2c[n - 1] < p->unk2c[n])
        return 4;
    if (p->unk2c[n - 1] > p->unk2c[n])
        return 2;
}
asm(".global sub_08039064\n.thumb_set sub_08039064, GetMovePathIncomingDirection\n");

u8 GetMovePathOutgoingDirection(u8 n)
{
    struct Unk0849D5F8 *p;

    p = gUnknown_0849D5F8;

    if (n == p->unk45)
        return 0;

    if (p->unk20[n] < p->unk20[n + 1])
        return 1;
    if (p->unk20[n] > p->unk20[n + 1])
        return 3;
    if (p->unk2c[n] < p->unk2c[n + 1])
        return 2;
    if (p->unk2c[n] > p->unk2c[n + 1])
        return 4;
}
asm(".global sub_080390CC\n.thumb_set sub_080390CC, GetMovePathOutgoingDirection\n");
