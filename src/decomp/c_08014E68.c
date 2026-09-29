#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014E68.
 * sub_08014E68 @ 0x08014E68
 */

struct MemBlock
{
    /* 0x00 */ struct MemBlock *next;
    /* 0x04 */ u32 size;
    /* 0x08 */ u32 used;
    /* 0x0c */ u32 filler_0c;
};

int HeapFreeBlock(int head, void *ptr)
{
    struct MemBlock *blk;
    struct MemBlock *q;
    struct MemBlock *next;
    u32 size;

    if (ptr == NULL)
        return 1;

    q = &((struct MemBlock *)ptr)[-1];
    blk = q;

    if (((struct MemBlock *)ptr)[-1].used == 0)
        return 1;

    next = q->next;
    size = ((struct MemBlock *)ptr)[-1].size;

    q = next;

    if (q != NULL && q->used == 0)
    {
        size += 0x10;
        size += q->size;
        next = q->next;
    }

    q = (struct MemBlock *)head;

    while (1)
    {
        if (q->next == blk)
        {
            if (q->used != 0)
                break;
            blk = q;
            size += 0x10;
            size += blk->size;
            break;
        }
        if (q > blk)
            break;
        if (q->next == NULL)
            break;
        q = q->next;
    }

    q = blk;
    q->next = next;
    q->size = size;
    q->used = 0;

    return 0;
}
asm(".global sub_08014E68\n.thumb_set sub_08014E68, HeapFreeBlock\n");
