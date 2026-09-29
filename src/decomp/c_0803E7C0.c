#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E7C0.
 * sub_0803E7C0 @ 0x0803E7C0, sub_0803E7E4 @ 0x0803E7E4
 */

void *GetCannonFireCellOffsetX(int a1, int a2)
{
    const struct Unk0849F688 *p;

    switch (a1)
    {
    case 3:
    default:
        p = gUnknown_0849F688;
        break;
    case 4:
        p = gUnknown_0849F698;
        break;
    }
    return p[a2].unk00;
}
asm(".global sub_0803E7C0\n.thumb_set sub_0803E7C0, GetCannonFireCellOffsetX\n");

void *GetCannonFireCellOffsetY(int a1, int a2)
{
    const struct Unk0849F688 *p;

    switch (a1)
    {
    case 3:
    default:
        p = gUnknown_0849F688;
        break;
    case 4:
        p = gUnknown_0849F698;
        break;
    }
    return p[a2].unk04;
}
asm(".global sub_0803E7E4\n.thumb_set sub_0803E7E4, GetCannonFireCellOffsetY\n");
