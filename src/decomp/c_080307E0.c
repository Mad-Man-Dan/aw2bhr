#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080307E0.
 * sub_080307E0 @ 0x080307E0
 */

struct Unk08090CD8Entry *SioPeekPendingSend(int *a1)
{
    struct Unk0849B018 *ptr = gUnknown_0849B018;
    struct Unk08090CD8Entry *result;

    if (ptr->unk12c[ptr->unk1aac].unk00 == 0xAF)
    {
        *a1 = ptr->unk12c[ptr->unk1aac].unk04;
        result = &ptr->unk12c[ptr->unk1aac];
    }
    else
    {
        result = NULL;
    }
    return result;
}
asm(".global sub_080307E0\n.thumb_set sub_080307E0, SioPeekPendingSend\n");
