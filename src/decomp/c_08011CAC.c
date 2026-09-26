#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011CAC.
 * Decompress @ 0x08011CAC
 */

/*
 * Decompress -- unpack src into dst, choosing the unpacker from src's header.
 *
 * The top nibble of the first byte is the format. gUnknown_08489314 holds two
 * entries per format, one for a VRAM destination and one for anywhere else, so
 * the index is (src[0] & 0xF0) >> 3 plus one when dst is outside
 * 0x06000000..0x06017FFF. A NULL slot means the data is not packed at all and
 * is copied straight through with CpuFastSet.
 *
 * The count for that copy is the header word with the format nibble taken out
 * and the two remaining pieces closed up -- the low nibble of byte 0 beside
 * bits 8 and above, 28 bits of byte count -- then divided by 4 for CpuFastSet's
 * word count and masked down to its 21-bit count field.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The VRAM test must be an if/else with 1 in the else arm, not
 *     `notVram = <comparison>;` and not a `?:`. The compiler presets the else
 *     arm's value and branches around the then arm's store, so this is the only
 *     spelling that sets 1 first and stores 0 afterwards, in the original's
 *     order.
 *   - The table index is a statement of its own. Written inside the subscript,
 *     the load of the table's address is lifted to the top of the block, ahead
 *     of the byte load that starts the index.
 *   - The high part of the count is `(header & 0xFFFFFF00) >> 4`, not
 *     `(header >> 8) << 4`. The two are equal, but this compiler does not turn
 *     the second into the first and leaves two shifts where the original has a
 *     mask and one shift.
 */

void Decompress(u8 * src, void * dst)
{
    void (*func)(const void *, void *);
    u32 notVram;
    u32 idx;

    if (((u32)dst - 0x06000000) <= 0x17FFF)
        notVram = 0;
    else
        notVram = 1;

    idx = notVram + ((src[0] & 0xF0) >> 3);
    func = gUnknown_08489314[idx];

    if (func == NULL)
        CpuFastSet(src, dst, (((src[0] & 0xF) | ((*(u32 *)src & 0xFFFFFF00) >> 4)) >> 2) & 0x1FFFFF);
    else
        func(src, dst);
}
