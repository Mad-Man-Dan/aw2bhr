#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804BD20.
 * sub_0804BD20 @ 0x0804BD20
 */

/* Both indices are `u16` parameters. The second shows it plainly --
 * `lsls #0x10; lsrs #0x10` at entry is PROMOTE_MODE -- and the first shows it
 * folded: `lsls r4, r4, #0x10` at entry with the matching `lsrs r4, r4, #0xe`
 * later is that same zero-extension combined with the * 4 element scaling, i.e.
 * a net left shift of two on sixteen bits. Reading it as `a << 2` alone loses
 * the truncation. */
void LoadFigurePalettes(u16 a, u16 b, void *dst, void *d)
{
    CpuFastSet(gUnknown_08555D30[b][a], dst, 0x10);
    BuildHitFlashPalette(gUnknown_08555D30[b][a], d);
}
asm(".global sub_0804BD20\n.thumb_set sub_0804BD20, LoadFigurePalettes\n");
