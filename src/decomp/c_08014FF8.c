#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014FF8.
 * HeapAllocAligned @ 0x08014FF8
 *
 * Not a Xenesis-documented name. The old sub_08014FF8 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

/* Aligned allocate from the gUnknown_03000050 heap, the sibling of HeapAlloc
 * (sub_08014DCC). Alignments of 16 or less defer to HeapMalloc. Otherwise
 * it best-fits the smallest free block that can hold `size` (rounded up to 16)
 * at an `align`-aligned address past its header. The gap in front of the
 * aligned address goes back to the heap, as a free block when it is large
 * enough and otherwise to the previous block. The tail is split off as a new
 * free block when at least 32 bytes remain.
 *
 * Two quirks are in the ROM, not in this transcription. The small-gap arm
 * points the previous block 64 bytes before the new header
 * (`(struct MemBlock *)data - 4`), and the no-split arm stores the distance to
 * the next block in words, not bytes.
 *
 * Measured spelling notes:
 * - `next = cur; best = next;` in the taken branch clobbers the register CSE
 *   would reuse for `cur->next`, which forces the ROM's reload of it.
 * - One cursor `p` walks the list for the previous block, writes the gap's
 *   header and finally holds the split-off tail, read back through
 *   `hdr->next` as in HeapAlloc. The best == head test compares against a
 *   fresh load of gUnknown_03000050 rather than `p`. If `p` is the value
 *   compared, cse knows p == best in that arm and deletes the ROM's
 *   `mov r2, r8` (-4 bytes with padding).
 * - The gap arms reach the block through `p = best` and the tail through
 *   `best`. The different pseudos keep GCSE from sharing `data - best`
 *   between them.
 * - The large-gap arm's size and the `<= 32` test use `* 4`, which is signed
 *   int (`bgt`) and CSEs with the gap already computed in bytes. With
 *   `* sizeof(u32)` it is unsigned, and the test changes and the arms
 *   cross-jump one instruction earlier.
 */
struct MemBlock
{
    /* 0x00 */ struct MemBlock *next;
    /* 0x04 */ u32 size;
    /* 0x08 */ u32 used;
    /* 0x0c */ u32 filler_0c;
};

void *HeapAllocAligned(int align, u32 size)
{
    struct MemBlock *cur;
    struct MemBlock *next;
    struct MemBlock *best;
    struct MemBlock *bestNext;
    struct MemBlock *p;
    u32 *data;
    u32 *aligned;
    u32 bestSize;
    u32 used;

    best = NULL;
    data = NULL;
    if (gUnknown_03000050 != -1 && size != 0)
    {
        if (align <= 16)
            return HeapMalloc(size);

        size = (size + 15) & ~15;
        bestSize = 0;
        bestNext = NULL;
        cur = (struct MemBlock *)gUnknown_03000050;
        while (1)
        {
            used = cur->used;
            next = cur->next;
            if (used == 0)
            {
                aligned = (u32 *)(((u32)(cur + 1) + (align - 1)) & ~(align - 1));
                if ((u32)aligned >= (u32)(cur + 1)
                 && (u32)aligned + size < (u32)cur + cur->size
                 && (bestSize > cur->size || bestSize == 0))
                {
                    data = aligned;
                    next = cur;
                    best = next;
                    bestSize = cur->size;
                    bestNext = cur->next;
                    next = bestNext;
                }
            }
            if (next == NULL)
                break;
            cur = next;
        }

        if (bestSize != 0)
        {
            if (best != (struct MemBlock *)gUnknown_03000050)
            {
                if ((data - (u32 *)best) * 4 <= 32)
                {
                    if (data - (u32 *)best > 15)
                    {
                        for (p = (struct MemBlock *)gUnknown_03000050; p->next != NULL; p = p->next)
                        {
                            if (p->next == best)
                            {
                                p->next = (struct MemBlock *)data - 4;
                                p->size += 16;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    p = best;
                    p->next = (struct MemBlock *)data - 1;
                    p->size = (data - (u32 *)best) * 4 - 32;
                    p->used = 0;
                }
            }
            else
            {
                p = best;
                p->next = (struct MemBlock *)data - 1;
                p->size = (data - (u32 *)p) * sizeof(u32) - 32;
                p->used = 0;
            }

            if (bestSize >= size + 32)
            {
                ((struct MemBlock *)data)[-1].next = (struct MemBlock *)((u8 *)data + size);
                ((struct MemBlock *)data)[-1].size = size;
                ((struct MemBlock *)data)[-1].used = 1;
                p = ((struct MemBlock *)data)[-1].next;
                p->next = bestNext;
                p->size = bestSize - size - (data - (u32 *)best) * sizeof(u32);
                p->used = 0;
            }
            else
            {
                ((struct MemBlock *)data)[-1].next = bestNext;
                ((struct MemBlock *)data)[-1].size = (u32 *)bestNext - data;
                ((struct MemBlock *)data)[-1].used = 1;
            }
            return data;
        }
    }
    return NULL;
}

asm(".global sub_08014FF8\n.thumb_set sub_08014FF8, HeapAllocAligned\n");
