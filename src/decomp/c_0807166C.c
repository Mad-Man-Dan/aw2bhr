#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807166C.
 * sub_0807166C @ 0x0807166C
 */

/* Wave 80 (W80-C): BYTE-EXACT UNDER compiler_profile "old-agbcc" (the m4a
 * block override its three matched neighbours MP_clear_modM / MPlayModDepthSet /
 * MPlayLFOSpeedSet carry in data/compiler-overrides.json: cc1 old_agbcc, remove
 * -fprologue-bugfix). Provisional until the orchestrator records the
 * override entry and re-runs the configured verdict. Under the DEFAULT
 * profile this same source is NOT exact (the case bodies rotate r0/r1 and the
 * offset+base add commutes), so the override is required, not cosmetic.
 *
 * Every number quoted in this header before wave 80 was measured under the
 * default profile and does not describe this function. Two source facts
 * closed it once the profile was right, and NEITHER is an allocation lever:
 *
 * 1. ONE SHARED CALL/SKIP TAIL, not twelve. The twelve comparison arms reach
 *    a single `gUnknown_03005744(a, b)` block by `break` and a single
 *    `b->unk40 += 4` block by `goto skip`; cases 0-5 and default `return`.
 *    The old draft wrote the call and the skip out in all twelve arms and
 *    relied on cross-jumping to fold them, which reproduces the INSTRUCTION
 *    STREAM but not the ALLOCATION: gcc 2.95 cross-jumps only in the
 *    post-reload jump pass, so at global_alloc time the written-out arms give
 *    `b` ~36 extra refs and the top priority (b -> r4, a -> r5, op -> r6).
 *    With one tail `b` has 7 refs against `a`'s 11 and the ROM's order
 *    a -> r4, op -> r5, b -> r6 falls out. This is the wave-79 "no source
 *    construct" residual: the construct was the tail, and the wave-79
 *    direction ("shorten op's range or lengthen b's") was pointing at the
 *    wrong variable -- it is b's REF COUNT.
 *
 * 2. THE THIRD WRITE-BACK IS SPELLED FROM THE SECOND ADVANCED POINTER.
 *    `nxt2 = nxt + 1; b->unk40 = nxt2; v = nxt[1]; b->unk40 = nxt2 + 1;`
 *    gives the ROM's `adds r0, r2, #1; str; ldrb r2, [r2, #1]; adds r0, #1;
 *    str`. Under default this local was 0.3% worse (W77-E) because default
 *    CSE chained the stores by itself; old_agbcc does not, and without the
 *    local it recomputes cur+3 from the original cursor (`adds r0, #3`).
 *    `*cur++` and `*b->unk40++` forms both lose the chain under old-agbcc
 *    too (measured this wave).
 *
 * And one wave-77 lever is REVERSED under old-agbcc: the split statement
 * `p = a->unk18; p = p + cur[1];` loads the base straight into p's register
 * (r3) and adds `r0 + r3`; the ROM loads the base into a scratch (r0) and
 * adds `r1 + r0`. The plain `p = a->unk18 + cur[1];` does exactly that under
 * old-agbcc (`cur[1] + a->unk18` and `&a->unk18[cur[1]]` are byte-identical
 * to it). Under default the plain form put the ldrb ahead of the ldr, which
 * is why wave 77 split it; that finding was about the default allocator.
 *
 * Kept from earlier waves, still true under old-agbcc: `v` is a 32-bit
 * local; gUnknown_03005744 is a two-argument function pointer; the opcode
 * map is 0-2 store/add/subtract an immediate, 3-5 the same through
 * a->unk18[operand], 6-11 compare against the immediate and 12-17 against
 * a->unk18[operand], in the order ==, !=, >, >=, <=, <. Case 17's branch is
 * inverted because its arm falls through into the call block. */

struct Unk0807166C_A
{
    /* 0x00 */ u8 filler_00[0x18];
    /* 0x18 */ u8 *unk18;
};
struct Unk0807166C_B
{
    /* 0x00 */ u8 filler_00[0x40];
    /* 0x40 */ u8 *unk40;
};

void ply_memacc(struct Unk0807166C_A *a, struct Unk0807166C_B *b)
{
    u8 op;
    u8 *p;
    u32 v;
    u8 *cur;
    u8 *nxt;
    u8 *nxt2;

    cur = b->unk40;
    op = cur[0];
    nxt = cur + 1;
    b->unk40 = nxt;
    p = a->unk18 + cur[1];
    nxt2 = nxt + 1;
    b->unk40 = nxt2;
    v = nxt[1];
    b->unk40 = nxt2 + 1;
    switch (op)
    {
    case 0:
        *p = v;
        return;
    case 1:
        *p += v;
        return;
    case 2:
        *p -= v;
        return;
    case 3:
        *p = a->unk18[v];
        return;
    case 4:
        *p += a->unk18[v];
        return;
    case 5:
        *p -= a->unk18[v];
        return;
    case 6:
        if (*p == v) break;
        goto skip;
    case 7:
        if (*p != v) break;
        goto skip;
    case 8:
        if (*p > v) break;
        goto skip;
    case 9:
        if (*p >= v) break;
        goto skip;
    case 10:
        if (*p <= v) break;
        goto skip;
    case 11:
        if (*p < v) break;
        goto skip;
    case 12:
        if (*p == a->unk18[v]) break;
        goto skip;
    case 13:
        if (*p != a->unk18[v]) break;
        goto skip;
    case 14:
        if (*p > a->unk18[v]) break;
        goto skip;
    case 15:
        if (*p >= a->unk18[v]) break;
        goto skip;
    case 16:
        if (*p <= a->unk18[v]) break;
        goto skip;
    case 17:
        if (*p < a->unk18[v]) break;
        goto skip;
    default:
        return;
    }
    gUnknown_03005744(a, b);
    return;
skip:
    b->unk40 += 4;
}
asm(".global sub_0807166C\n.thumb_set sub_0807166C, ply_memacc\n");
