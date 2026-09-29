#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011C58.
 * sub_08011C58 @ 0x08011C58, sub_08011C68 @ 0x08011C68, sub_08011C90 @ 0x08011C90
 */

/* The halfword CpuSet wrapper: a BYTE count in, a halfword count out. The
 * third parameter is u16 and the single `lsls #0x10; lsrs #0x11` pair is
 * PROMOTE_MODE's zero-extension fused with the `/ 2` -- the reading the
 * prototype note in include/unknown-functions.h already carried, now confirmed
 * from the definition side. */
void CpuCopy16(const void *src, void *dst, u16 size)
{
    CpuSet(src, dst, size / 2);
}
asm(".global sub_08011C58\n.thumb_set sub_08011C58, CpuCopy16\n");

/* Picks the wide copy when it can: a byte count that is a multiple of 32 goes
 * to CpuFastSet as words, anything else to CpuSet as halfwords.
 *
 * The third parameter is u16, and this is NOT one of the byte-neutral width
 * choices -- see the retyping note on the prototype in
 * include/unknown-functions.h. With u16 the single `lsls r2,#0x10` from
 * PROMOTE_MODE is shared by all three uses: the `& 0x1f` test runs in that
 * shifted domain against 0x1f0000 (`movs #0xf8; lsls #0xd`) and the two
 * divisions are a bare `lsrs #0x11` / `#0x12`. Spelled `int` with `(u16)`
 * casts, the mask becomes a plain imm8 AND that clobbers r1, the dst pointer
 * spills to r4, and the prologue grows to `push {r4, lr}`. */
void CpuCopyAuto(const void *src, void *dst, u16 size)
{
    if (size & 0x1F)
        CpuSet(src, dst, size / 2);
    else
        CpuFastSet(src, dst, size / 4);
}
asm(".global sub_08011C68\n.thumb_set sub_08011C68, CpuCopyAuto\n");

/* The word-copy member of the CpuCopy16 family: byte count in, word count
 * out. Its own body is byte-identical whether the third parameter is `u16` or
 * `int` with a `(u16)` cast; it is spelled u16 to follow its two siblings,
 * where CpuCopyAuto's body does discriminate. Flagged rather than claimed. */
void CpuFastCopy(const void *src, void *dst, u16 size)
{
    CpuFastSet(src, dst, size / 4);
}
asm(".global sub_08011C90\n.thumb_set sub_08011C90, CpuFastCopy\n");
