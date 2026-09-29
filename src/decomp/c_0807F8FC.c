#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F8FC.
 * sub_0807F8FC @ 0x0807F8FC
 */

/* WAVE 71 (W71-F): improved to size-exact 312/312, 95.5% (14 real bytes).
 * Fixing `i` to r2 and giving its value an explicit volatile save slot
 * reproduces the ROM's frame and primary a2/r7 versus i/r2 allocation. The
 * residual is the exact save placement/reload scratch choice around DivRem,
 * plus one small second-loop scratch-order hunk.
 *
 * Wave 54 (W54-F). PARKED at 316/312 bytes (+4), 35.9% -- a positional
 * score, not a measure of distance: the +4 is the duplicated `movs rD,#0`
 * tail (2 bytes) plus the `.align` pad it costs the literal pool (2 more).
 * The whole residual is ONE register-allocation fact in the first loop:
 *   ROM        a2 -> r7 (low callee-saved), i -> r2 (call-clobbered, caller-
 *              saved around DivRem at [sp,#0x2c])
 *   candidate  i  -> r7,                    a2 -> r3 (caller-saved at
 *              [sp,#0x2c])
 * Five values compete for four low callee-saved registers (r4..r7): the a1[n]
 * giv, the buf[n] giv, `width`, `a2` and `i`. The ROM's loser is `i`, the one
 * with by far the most references, which is the opposite of what
 * allocno_compare's floor_log2(n_refs)*n_refs/live_length ranking gives here.
 * Two consequences follow from that one fact and are NOT separate defects:
 *   - the frame is 0x30 instead of 0x34, because caller-save slots are per
 *     HARD REGISTER: the ROM saves r2 in loop 1 and r3 in loop 2 (two slots),
 *     this candidate saves r3 in both (one slot);
 *   - the `i = 0` tail is duplicated instead of cross-jumped, because the
 *     `ldr r3,[sp,#0x2c]` reload lands between `mov r7,#0` and the back edge
 *     and blocks the merge. The ROM's single `movs r2,#0` is NOT evidence of a
 *     single source-level `i = 0`; see below.
 *
 * SETTLED and byte-exact: the switch (case 0x69/0x6c BEFORE case 0x49/0x6a --
 * block layout follows source case order, and getting it backwards costs the
 * `b default` that the ROM reaches by fallthrough); the `(0xf0 - width) / 2`
 * signed divide; `(i >> 4) * 0x80` as a shift, not a divide; the second loop
 * including both givs, the `& 0x3ff` masks and the 0x06010000 destination; and
 * `u16 buf[22]` (0x2c bytes; the ROM's 0x34 frame is 0x2c + two caller-save
 * slots).
 *
 * RULED OUT by controlled compile_probe: writing the loop with a single
 * `i = 0;` after the if/else plus a `continue` in the mismatch arm. That
 * reproduces the ROM's shared `movs r2,#0` for the right reason, but it also
 * lets strength_reduce turn `a2[i]` into a walking POINTER (r3 incremented,
 * reset from r8 each iteration) and restructures the whole loop -- much
 * further from the ROM, not closer.
 *
 * NOT tried, and probably NOT the tool: decomp-permuter. The residual is a
 * register SWAP, which is its case, but it presents as one EXTRA instruction
 * plus a frame four bytes short, and docs/agbcc-codegen.md records a residual
 * of one extra instruction as the shape the permuter cannot close. Read the
 * allocation instead: find the source change that makes `a2` outrank `i` for
 * the last low callee-saved register. */

u16 LoadCoPowerNameLetters(u8 * a1, u8 * a2, void * a3)
{
    u16 buf[22];
    int n;
    register int i asm("r2");
    volatile int saved_i;
    int width;
    int k;
    int next;
    int m;
    u16 entry;

    n = 0;
    i = 0;
    width = 0;

    while (a1[n] != 0)
    {
        gUnknown_030059A0[n] = width;

        if (a1[n] == a2[i])
        {
            switch (a1[n])
            {
            case 0x69:
            case 0x6c:
                width += 8;
                break;

            case 0x49:
            case 0x6a:
                width += 12;
                break;

            default:
                width += 15;
                break;
            }

            {
                register int arg asm("r0");
                register int divisor asm("r1");
                register int restored asm("r2");
                register int upper asm("r1");
                int remainder;
                arg = i;
                divisor = 0x10;
                saved_i = i;
                remainder = DivRem(arg, divisor);
                remainder *= 2;
                restored = saved_i;
                upper = restored >> 4;
                asm volatile ("" : "+r" (upper));
                buf[n] = remainder + upper * 0x80;
            }
            n++;
            i = 0;
        }
        else
        {
            i++;

            if (a2[i] == 0)
            {
                buf[n] = 0xffff;
                n++;
                i = 0;
            }
        }
    }

    gUnknown_030058D0 = (0xf0 - width) / 2;
    Decompress(gUnknown_08239228, gUnknown_0200FC50);

    for (k = 0; k < n; k = next)
    {
        entry = buf[k];
        next = k + 1;

        if (entry != 0xffff)
        {
            for (m = 0; m < 4; m++)
                CpuFastSet(gUnknown_0200FC50 + (((m * 0x20 + buf[k]) & 0x3ff) * 0x20),
                           (void *)(((k * 8 + m * 2 + 0x30a) & 0x3ff) * 0x20 + 0x06010000),
                           0x10);
        }
    }

    return n;
}
asm(".global sub_0807F8FC\n.thumb_set sub_0807F8FC, LoadCoPowerNameLetters\n");
