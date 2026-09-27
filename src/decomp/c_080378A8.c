#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080378A8.
 * sub_080378A8 @ 0x080378A8
 */

/* MATCHED, wave 50 (W50-L), second try_match attempt.
 *
 * Redraws the whole minimap: for every visible cell copy a four-halfword tile
 * quad into the caller's buffer, and tally the terrain types seen. Cells
 * outside the map get a blank quad. Sibling of sub_08037A78, which builds its
 * destination the same way (`a1 + xtable[x] + ytable[y]`, both scaled by 2)
 * and reads the same gUnknown_080A0F38 tile pool.
 *
 * `gUnknown_0849D474` appears TWICE with unrelated meanings -- once to pick a
 * tile id, once as the x-offset table -- because the tables are contiguous in
 * one ROM blob and agbcc CSEs the single base register across both. That is
 * what the ROM does; it is not a transcription slip.
 *
 * THREE READOUTS, EACH WORTH AN ATTEMPT.
 *
 * 1. The BLANK-TILE arm is emitted FIRST, so the source tests the negated
 *    condition: `if (x >= buf[0] || y >= buf[1]) src = blank; else {...}`.
 *    Written the positive way round agbcc lays the big arm inline and the
 *    blank arm after it, which moves the gUnknown_0849D434 pool word out of
 *    the first literal pool. Read the pool order backwards to spot this.
 * 2. `hi = b >> 5` is its own statement BEFORE the switch -- the ROM computes
 *    it into r2 between the `ldrb` and the first `cmp`. Folded into the
 *    expression after the switch it lands after the `ldrh`, one slot late.
 *    The switch value must be UNSIGNED (`bhi`, not `bgt`), and the case bodies
 *    are laid out 0xe, 6, 0xa, 0xb, which is their source order: the last test
 *    of agbcc's decision tree falls straight into the FIRST case body, so 0xe
 *    is inline with a `bne` to the default while the other three are jumped to.
 * 3. The header pointer needs the exemplar's two-statement form
 *    (`p = (u16 *)buf; p += off + x;`). Written as one expression the operands
 *    of the final add come out reversed -- `adds r0, r0, r5` where the ROM has
 *    `adds r0, r5, r0` -- which is a size-exact 99.5% and was the last byte to
 *    fall. Same rule as readout 3 of src/decomp/c_080377C4.c.
 *
 * `w` and `h` are UNSIGNED, not u8: `(buf[0] - w) >> 1` is a `lsrs`, and both
 * `w++` and `h++` are bare `adds` with no re-truncation. The `(u8)` cast on the
 * `h` bound is real and asymmetric with `w` on purpose -- `w` lives in r6 so
 * combine folds its cast away against the `ldrb`, while `h` is spilled to
 * [sp,#4] in between, which breaks the chain and leaves the `lsls #24; lsrs
 * #24` pair standing. `x` and `y` are u16, truncated at each increment.
 *
 * The four counters are zeroed through `off` rather than a literal: agbcc parks
 * the constant in r8, which is where `off` then lives, and the ROM's
 * `movs r0,#0; mov r8,r0; mov r1,r8` round trip is exactly that. */

void sub_080378A8(void *a1)
{
    const u16 *src;
    u16 *dst;
    u16 *p;
    u8 *buf;
    unsigned w;
    unsigned h;
    unsigned off;
    unsigned b;
    unsigned hi;
    u16 x;
    u16 y;

    off = 0;
    gUnknown_02027F74.unk03 = off;
    gUnknown_02027F74.unk02 = off;
    gUnknown_02027F74.unk00 = off;
    gUnknown_02027F74.unk01 = off;

    w = ((u8 *)gUnknown_03003F68)[0];
    h = ((u8 *)gUnknown_03003F68)[1];
    if (w > 0x20)
        w = 0x20;
    if ((u8)h > 0x20)
        h = 0x20;
    off = (((u8 *)gUnknown_03003F68)[0] - w) >> 1;
    if (w & 1)
        w++;
    if (h & 1)
        h++;

    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            buf = (u8 *)gUnknown_03003F68;
            if (x >= buf[0] || y >= buf[1])
            {
                src = gUnknown_0849D434;
            }
            else
            {
                p = (u16 *)buf;
                p += off + x;
                b = gUnknown_0849959C[p[1]];
                hi = b >> 5;
                switch (b & 0x1f)
                {
                case 0xe:
                    gUnknown_02027F74.unk01++;
                    break;
                case 6:
                    gUnknown_02027F74.unk00++;
                    break;
                case 0xa:
                    gUnknown_02027F74.unk02++;
                    break;
                case 0xb:
                    gUnknown_02027F74.unk03++;
                    break;
                }
                src = (const u16 *)gUnknown_080A0F38
                    + gUnknown_0849D474[gUnknown_0849D3DC[b & 0x1f] + hi];
            }
            dst = (u16 *)a1 + gUnknown_0849D474[x] + gUnknown_0849D4F4[y];
            dst[0] = src[0];
            dst[2] = src[2];
            dst[4] = src[4];
            dst[6] = src[6];
        }
        off += ((u8 *)gUnknown_03003F68)[0];
    }
}
