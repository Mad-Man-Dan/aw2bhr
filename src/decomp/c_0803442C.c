#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803442C.
 * sub_0803442C @ 0x0803442C, sub_0803446C @ 0x0803446C, sub_080344B4 @ 0x080344B4
 */

/* The nibble UNPACKER, inverse of src/decomp/c_08034400.c: each source byte's
 * high nibble goes to dst[i * 2] and its low nibble to dst[i * 2 + 1], with 0xf
 * mapped to 0xff in both.
 *
 * Two binding locals carry this, and both are readable off the register
 * allocation rather than guessed:
 *
 * `n` is ONE local reused for both nibbles, not two. The ROM computes both into
 * r2 -- `lsrs r2, r0, #4` and `movs r2, #0xf; ands r2, r0`. The second is forced
 * there (r0 holds the loaded byte and is the `and`'s other operand), but the
 * shift is not, and with two separate locals agbcc coalesces the shift into r0.
 * One local gives one pseudo whose live range spans both halves, so it takes a
 * single hard register and the shift is dragged to r2. That one instruction's
 * destination register was the last difference against an otherwise
 * byte-identical candidate.
 *
 * `j` sits BETWEEN the two `if`s, which is where the ROM's `lsls r3, r5, #1`
 * lands -- so the first store cannot be the one using it. The first store
 * reaches dst by a strength-reduced biv (r4, stepping 2) while `dst + j` is
 * recomputed in both arms of the second `if` off the preserved copy of dst in
 * r6. Writing both stores as dst[i*2] / dst[i*2+1] merges them into one biv
 * with #0 and #1 displacements, kills the index, and lets check_dbra_loop
 * reverse the loop -- 56 bytes against 64. See the check_dbra_loop section in
 * docs/agbcc-codegen.md.
 *
 * src[i] is deliberately re-read rather than bound: the ROM has two `ldrb`. */
void UnpackPathNibbles(u8 *src, u8 *dst)
{
    int i;
    int j;
    int n;

    for (i = 0; i < 6; i++)
    {
        n = src[i] >> 4;

        if (n == 0xf)
            dst[i * 2] = 0xff;
        else
            dst[i * 2] = n;

        j = i * 2;
        n = src[i] & 0xf;

        if (n == 0xf)
            dst[j + 1] = 0xff;
        else
            dst[j + 1] = n;
    }
}
asm(".global sub_0803442C\n.thumb_set sub_0803442C, UnpackPathNibbles\n");

/* A once-per-entry snapshot into the LinkQueueCommand command block: the two
 * u16 cursor pairs are truncated into bytes 2..5 and the six-byte nibble-packed
 * copy PackPathNibbles does lands at +0x0c. gUnknown_030040DC is the guard and the
 * done-flag both. */
void SnapshotActionCommandContext(void)
{
    if (gUnknown_030040DC != 1)
    {
        gUnknown_030044B0[2] = gUnknown_03003100.pos.unk00;
        gUnknown_030044B0[3] = gUnknown_03003100.pos.unk02;
        gUnknown_030044B0[4] = gUnknown_03003F24.pos.unk00;
        gUnknown_030044B0[5] = gUnknown_03003F24.pos.unk02;
        PackPathNibbles(gUnknown_03003110, gUnknown_030044B0 + 0xc);
        gUnknown_030040DC = 1;
    }
}
asm(".global sub_0803446C\n.thumb_set sub_0803446C, SnapshotActionCommandContext\n");

/* The sibling of SendActionCommand: same command block, command id hard-coded to 8.
 * `unit` is bound before the block is filled -- the ROM computes
 * &gUnits[a] into r5 first and keeps it live across the four stores,
 * which is what a local declared here gives. */
void SendMoveCommand(u8 a, int b, int c)
{
    struct Unit *unit = &gUnits[a];

    gUnknown_030044B0[0] = 8;
    gUnknown_030044B0[1] = a;
    gUnknown_030044B0[6] = b;
    gUnknown_030044B0[7] = c;
    gUnknown_030044B0[0x12] = unit->fuel;
    LinkQueueCommand(gUnknown_030044B0);
}
asm(".global sub_080344B4\n.thumb_set sub_080344B4, SendMoveCommand\n");
