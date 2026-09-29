#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014DCC.
 * HeapAlloc @ 0x08014DCC
 *
 * Not a Xenesis-documented name. The old sub_08014DCC symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

/* Best-fit allocator for a heap laid out by HeapInitBuffer. HeapMalloc is the
 * wrapper that passes gUnknown_03000050; HeapFreeBlock frees and HeapRealloc
 * reallocates. The header layout is the one c_08014E68.c established.
 *
 * Rounds the request up to 16 bytes, walks the block list for the smallest
 * free block that fits, and splits off the remainder as a new free block when
 * it has room for a header plus 16 bytes. Returns the data after the header,
 * or NULL.
 *
 * Measured spelling notes:
 * - The split-off block reuses the scan cursor `p`, read back through
 *   `best->next`. That makes the ROM's `adds r2, r0, #0` copy and also keeps
 *   `p` and `next` in separate registers in the loop. With a separate `rest`
 *   local, `p = next` is coalesced away and the function is 8 bytes short.
 * - The first NULL return is early and the second falls through to the end.
 *   jump cross-jumps the two `movs r0, #0` exits into the trailing block,
 *   which leaves the guard as `bne <loop>; b <ret0>`. A goto to a shared
 *   `fail:` label for both exits gives `beq <ret0>; b <loop>` instead.
 */
struct MemBlock
{
    /* 0x00 */ struct MemBlock *next;
    /* 0x04 */ u32 size;
    /* 0x08 */ u32 used;
    /* 0x0c */ u32 filler_0c;
};

void *HeapAlloc(int heap, u32 size)
{
    struct MemBlock *best = NULL;
    struct MemBlock *p = (struct MemBlock *)heap;
    struct MemBlock *next;
    struct MemBlock *bestNext;
    u32 bestSize;
    u32 total;

    size = (size + 0xf) & ~0xf;
    total = size + sizeof(struct MemBlock);
    bestSize = 0;
    bestNext = NULL;

    if (size == 0)
        return NULL;

    for (;; p = next)
    {
        if (p->used == 0 && p->size >= size && (bestSize > p->size || bestSize == 0))
        {
            best = p;
            bestSize = p->size;
            bestNext = best->next;
        }
        next = p->next;
        if (next == NULL)
            break;
    }

    if (bestSize != 0)
    {
        if (bestSize >= size + 2 * sizeof(struct MemBlock))
        {
            best->next = (struct MemBlock *)((u8 *)best + total);
            best->size = size;
            best->used = 1;
            p = best->next;
            p->next = bestNext;
            p->size = bestSize - total;
            p->used = 0;
        }
        else
        {
            best->next = bestNext;
            best->size = bestSize;
            best->used = 1;
        }
        return best + 1;
    }
    return NULL;
}

asm(".global sub_08014DCC\n.thumb_set sub_08014DCC, HeapAlloc\n");
