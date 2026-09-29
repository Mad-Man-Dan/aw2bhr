#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014EF4.
 * sub_08014EF4 @ 0x08014EF4, sub_08014FB0 @ 0x08014FB0
 */


/* Header the heap keeps in front of every block it hands out. A caller's
 * pointer is the byte just past its own header, so the header is
 * `((struct MemBlock *)ptr)[-1]`. `used` is 0 while the block is free. */
struct MemBlock
{
    /* 0x00 */ struct MemBlock *next;
    /* 0x04 */ u32 size;
    /* 0x08 */ u32 used;
    /* 0x0c */ u32 filler_0c;
};

/* The scratch word at 0x0300004C has no symbol of its own in aw2bhr.lds -- the
 * IWRAM symbol table jumps from 0x48 straight to 0x50 -- so it cannot be
 * declared as an extern and still link in the split build. Reaching it as an
 * offset from the symbol that does cover it resolves to the same address.
 * Leave it spelled this way. */
#define gUnknown_0300004C (*(void **)((u8 *)&gUnknown_03000048 + 4))


/*
 * HeapRealloc -- resize a block in the gUnknown_03000050 heap (realloc).
 *
 * The same heap HeapMalloc (allocate) and HeapFree (free) work in. A
 * caller's pointer is the byte just past its own header.
 *
 *   1. A NULL pointer is a plain allocation; a size of 0 frees and returns
 *      NULL.
 *   2. Round the request up to a multiple of 16, and give up (NULL) if the
 *      block is already free.
 *   3. Work out the space available: the block's own size, plus the next
 *      block's size and one header if that block is free.
 *   4. Not enough? Allocate a fresh block, copy `size` bytes across with
 *      sub_0808B6E8, free the old block and return the new pointer.
 *   5. Enough? Keep the pointer. If at least 0x20 bytes would be left over,
 *      cut the tail off as a new free block; otherwise the block keeps the lot.
 *
 * gUnknown_0300004C is written four times and never read; it looks like a
 * scratch slot recording the block the heap code is working on.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The neighbour's size is added through the temporary `t`, inside a
 *     `do { } while (0)`. Without the temporary the sum is computed in place
 *     instead of into a fresh register; without the loop two locals end up in
 *     each other's registers.
 *   - The new free block is reached as `q[1]` off `q = ptr + size` rather than
 *     through a pointer of its own, which is what folds the member offsets
 *     into the store instructions.
 *
 * The new header lands at ptr + size + 0x10, one header further along than the
 * block sub_08014DCC splits off. That asymmetry is in the original.
 */
void *HeapRealloc(void *ptr, u32 size)
{
    struct MemBlock *blk;
    struct MemBlock *next;
    struct MemBlock *q;
    void *newptr;
    u32 avail;

    if (ptr == NULL)
        return HeapMalloc(size);

    if (size == 0)
    {
        HeapFree(ptr);
        return NULL;
    }

    size = (size + 0xf) & ~0xf;
    blk = &((struct MemBlock *)ptr)[-1];
    gUnknown_0300004C = blk;

    if (((struct MemBlock *)ptr)[-1].used == 0)
        return NULL;

    next = blk->next;
    avail = ((struct MemBlock *)ptr)[-1].size;
    gUnknown_0300004C = next;

    if (next != NULL && next->used == 0)
    {
        u32 t;

        t = avail + 0x10;
        do { avail = t + next->size; } while (0);
        next = next->next;
    }

    gUnknown_0300004C = blk;

    if (size > avail)
    {
        newptr = HeapMalloc(size);
        if (newptr == NULL)
            return NULL;
        sub_0808B6E8(newptr, ptr, size);
        HeapFree(ptr);
        return newptr;
    }

    if (size + 0x20 < avail)
    {
        q = (struct MemBlock *)((u8 *)ptr + size);
        blk->next = q + 1;
        blk->size = size;
        gUnknown_0300004C = q + 1;
        q[1].next = next;
        q[1].size = avail - size - 0x10;
        q[1].used = 0;
    }
    else
    {
        blk->next = next;
        blk->size = avail;
    }

    return ptr;
}
asm(".global sub_08014EF4\n.thumb_set sub_08014EF4, HeapRealloc\n");


/*
 * HeapCalloc -- allocate n * size zeroed bytes from the gUnknown_03000050
 * heap (calloc).
 *
 * Returns NULL if the heap is not set up (gUnknown_03000050 is -1) or if
 * HeapMalloc cannot satisfy the request. The clear is a plain byte loop.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The loop counts a separate `i` down instead of `total` itself. Sharing
 *     the one local makes the byte count and the loop counter the same value,
 *     and the original's copy of the returned pointer then disappears.
 *   - `while (i-- != 0)` and not a `for`: the post-decrement is what makes the
 *     compiler test against -1, and reuse the -1 the heap check already loaded.
 */
void *HeapCalloc(int n, int size)
{
    u8 *q;
    void *p;
    int total;
    int i;

    if (gUnknown_03000050 == -1)
        return NULL;

    total = n * size;
    p = HeapMalloc(total);
    if (p == NULL)
        return NULL;

    q = p;
    i = total;
    while (i-- != 0)
        *q++ = 0;

    return p;
}
asm(".global sub_08014FB0\n.thumb_set sub_08014FB0, HeapCalloc\n");
