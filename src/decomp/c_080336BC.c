#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080336BC.
 * sub_080336BC @ 0x080336BC, sub_0803376C @ 0x0803376C
 */

#include "proc.h"
/* MATCHED byte-for-byte, wave 43 (W43-C), first attempt.
 *
 * The block-sender's per-frame tick, and the consumer half of the proc
 * StartSioBigSend builds: unk2a is the block count, unk2c the cursor, unk24 the
 * write pointer, unk2e the tail byte count and unk2f the 0..100 percentage.
 * Every field lines up with StartSioBigSend's initialisation, which is what
 * settles this struct -- a producer and a consumer agreeing independently,
 * rather than any single access here.
 *
 * `LinkReceiveBlock(...) != -1` re-narrows the callee's `s16` return with
 * `lsls #0x10; asrs #0x10`; that is the declared width, not a cast at this
 * use. The -1 arrives as `movs r1,#1; rsbs r1,r1,#0` because `cmp` takes only
 * an 8-bit immediate -- it is the constant, not arithmetic.
 *
 * The two comparisons over the SAME pair of u16 fields come out with
 * different signedness and both are correct as written. The guard is `bge`
 * (signed) because `proc->unk2a - 1` is int arithmetic that can reach -1; the
 * tail is `blo` (unsigned) because both operands are bare zero-extended
 * `ldrh` loads and agbcc knows they are non-negative. Nothing in the source
 * distinguishes them, so this is not a lever -- it is worth recording only
 * because a signed/unsigned mismatch usually IS a type signal and here it is
 * not.
 *
 * `100 * proc->unk2c / proc->unk2a` is signed division, so agbcc emits a call
 * to __divsi3. That is a libgcc helper, NOT a subsystem function -- never
 * declare it.
 *
 * The copy loop reloads proc->unk24 after the byte store because the store
 * goes through a pointer that could alias the field. Two statements are what
 * the ROM has; `*p++ = v` keeps the value in a register and does not match. */

struct Unk336BCProc
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ void (*unk20)(struct Unk336BCProc *);
    /* 0x24 */ u8 *unk24;
    /* 0x28 */ u8 filler_28[0x02];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u8 unk2e;
    /* 0x2f */ u8 unk2f;
};
/* MATCHED byte-for-byte, wave 43 (W43-C), by decomp-permuter. It had been
 * parked at 92.6% since wave 33, with waves 33 and 34 both concluding there
 * was "no untested source-level lever". There was one.
 *
 * THE ANSWER TO THE PARK'S OPEN QUESTION. The residual was r4/r5 swapped and
 * nothing else: the ROM keeps `n` (the rounded block count) in r5 and `m`
 * (the `b & 0x7f` remainder) in r4, and every hand draft put them the other
 * way round. The earlier waves correctly ruled out declaration order, `u8 m`,
 * and swapping the two computation statements. The lever is none of those --
 * it is BINDING THE MASK CONSTANT 0x7f TO ITS OWN LOCAL, ASSIGNED BEFORE THE
 * EARLY-RETURN GUARD.
 *
 * Why it works, and it is the allocno-priority story the park was reaching
 * for. agbcc's global.c ranks allocnos by roughly
 * `floor_log2(n_refs) * freq / live_length` and hands out the low registers
 * first. `n` has four references (two defs, two uses) against `m`'s three, so
 * `n` outranks `m` and takes r4 -- which is exactly what every hand draft
 * produced. Hoisting 0x7f into `mask` adds a THIRD allocno whose live range
 * starts at the top of the function and reaches past the guard to `b & mask`,
 * which shifts the live-length denominators enough to invert the n/m
 * ordering. The constant still folds into `movs r4,#0x7f`, so the extra local
 * costs zero instructions.
 *
 * Two things generalise:
 *   - Wave 34's conclusion that "statement order controls INSTRUCTION order
 *     and does not touch the ALLOCATION at all" is RIGHT, and it is precisely
 *     why the lever had to be a new VALUE rather than a reordering. When a
 *     pure slot-wrong residual survives every reordering, add a live range;
 *     do not keep shuffling the ones already there.
 *   - The permuter earned its keep on exactly the case the brief predicts for
 *     it (order-wrong and slot-wrong), at 108 bytes -- well under the "above
 *     256 bytes" guidance, which is about when to REACH for it, not a floor
 *     below which it stops working. Iteration 402, ~5 minutes, 4 threads.
 *
 * Prototype, settled in wave 33 and unchanged: the `int` return comes from
 * both exits setting r0 (`movs r0,#1; rsbs r0,r0,#0` and `movs r0,#0`), which
 * a void function does not do; the `u8` fourth parameter from the
 * `lsls r3,#24; lsrs r6,r3,#24` at entry. */

struct Unk3376CProc
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ int unk20;
    /* 0x24 */ u32 unk24;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 filler_29[0x01];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u8 unk2e;
    /* 0x2f */ u8 unk2f;
};

void SioBigReceive_Loop(struct Unk336BCProc *proc)
{
    int i;

    if (proc->unk2c < proc->unk2a - 1)
    {
        if (LinkReceiveBlock(proc->unk24, 0) != -1)
        {
            proc->unk24 += 0x80;
            proc->unk2f = 100 * proc->unk2c / proc->unk2a;
            proc->unk2c++;
        }
    }
    else if (LinkReceiveBlock((void *)gUnknown_03004400, 0) != -1)
    {
        for (i = 0; i < proc->unk2e; i++)
        {
            *proc->unk24 = gUnknown_03004400[i];
            proc->unk24++;
        }

        proc->unk2f = 100 * proc->unk2c / proc->unk2a;
        proc->unk2c++;
    }

    if (proc->unk20 != 0)
        proc->unk20(proc);

    if (proc->unk2c >= proc->unk2a)
        Proc_Break(proc);
}
asm(".global sub_080336BC\n.thumb_set sub_080336BC, SioBigReceive_Loop\n");

int StartSioBigSend(u32 a, u32 b, int c, u8 d, ProcPtr parent)
{
    int mask;
    struct Unk3376CProc *proc;
    u16 n;
    int m;

    mask = 0x7f;

    if (b > 0x007FFF80)
        return -1;

    n = (b >> 7) + 1;
    m = b & mask;

    if (m != 0)
        n = n + 1;

    proc = Proc_StartBlocking(gUnknown_0849BB08, parent);

    proc->unk24 = a;
    proc->unk28 = d;
    proc->unk20 = c;
    proc->unk2a = n;
    proc->unk2e = m;
    proc->unk2f = 0;
    proc->unk2c = 0;

    return 0;
}
asm(".global sub_0803376C\n.thumb_set sub_0803376C, StartSioBigSend\n");
