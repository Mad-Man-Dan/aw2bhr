#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804BB28.
 * sub_0804BB28 @ 0x0804BB28, sub_0804BB44 @ 0x0804BB44
 */

/* `lsls r2, r0, #1; adds r2, r2, r0; lsls r2, r2, #3` is a multiply by
 * 3 << 3 = 0x18, i.e. an index on the 0x18-byte gUnknown_08555850 record.
 * The `adds r3, #4` on the BASE rather than an `ldr r0, [r2, #4]` is
 * -fforce-addr putting the bare symbol in the pool and adding the member
 * offset to it; the honest `gUnknown_08555850[a].unk04` produces it. */
/* Wave 43, W43-K: the dead third parameter was added when LoadBattleBackdropArt was
 * matched. It is invisible here -- r2 is written by the index multiply before
 * any read -- but the only caller emits `movs r2,#0x80; lsls r2,#7` in front of
 * the `bl`, which nothing else consumes, so the arity is 3. Same reading as the
 * GetBattleBackdropId note in unknown-functions.h. Byte-neutral: re-verified. */
void LoadBackdropTiles(int a, void *dst, int c)
{
    LZ77UnCompVram(gUnknown_08555850[a].unk04, dst);
}
asm(".global sub_0804BB28\n.thumb_set sub_0804BB28, LoadBackdropTiles\n");

/* Two indices on one record: `a` picks the 0x18-byte entry and the u16
 * gUnknown_03004520 picks a word inside its +0x0c array.
 *
 * `lsls r2, r2, #9; lsrs r2, r2, #0xb` is a NET RIGHT SHIFT OF TWO that also
 * drops the top nine bits -- read the pair as `(u32)x << 9 >> 11`, not as a
 * mask plus a shift. Written that way agbcc emits the two shifts; an explicit
 * `(x & 0x7fffff) >> 2` costs a pool word for the mask. */
void LoadBackdropPalette(int a, void *dst, int c)
{
    CpuFastSet(gUnknown_08555850[a].unk0c[gUnknown_03004520], dst, ((u32)c << 9) >> 11);
}
asm(".global sub_0804BB44\n.thumb_set sub_0804BB44, LoadBackdropPalette\n");
