#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A38C.
 * sub_0802A38C @ 0x0802A38C
 */

/* `bl _call_via_r3` is an ordinary indirect THUMB call, not an interworking
 * veneer. The register index does NOT count arguments here: the pointer
 * arrives as parameter 2 in r1 and is moved to r3 because r0 carries the one
 * argument.
 *
 * TWO binding locals, each worth a couple of bytes and neither obvious:
 *
 * (1) `t` -- the shifted half of the packed word must be its own statement.
 *     Written `p->unk02 | p->unk03 << 16` the instructions are identical but
 *     the unk02 load is scheduled ahead of the unk03 load and shift; written
 *     the other way round (`p->unk03 << 16 | p->unk02`) the order is right but
 *     the OR's destination moves to the shift's register. Only splitting the
 *     shift out gives the ROM's `ldrb; lsls; ldrb; orrs r1, r0`.
 *
 * (2) `q` -- binding gUnknown_030033EC's ADDRESS before the indirect call is
 *     what makes it live ACROSS that call, so it takes a callee-saved register
 *     and its pool `ldr` is emitted at the top of the block, as in the ROM.
 *     Dereferenced in place the address is rematerialised into a scratch after
 *     the call instead. The load itself stays after the call either way.
 *
 * `ldrb r1, [r5]` on a u16 global is the u8 conversion of a halfword on a
 * little-endian target -- StartSupplyAnimation's unused second parameter is u8, and
 * this call site is the only thing in the tree that shows its width. */
/* WAVE 35: CANONICAL `struct Map`. Eight drafts across blocks 0x08029-0x0802B
 * each invented their own body for this tag, with 2 to 7 named fields. Every
 * one compiles and byte-matches ALONE, so trymatch cannot see the problem;
 * merged into a promoted unit it is a hard `redefinition of struct Map`.
 * All seven distinct bodies describe the SAME layout at different
 * resolutions -- the offsets reconcile exactly -- so this is their union, the
 * most refined reading of each region. Byte-neutral: only a field's START
 * OFFSET enters the address arithmetic, never its declared length, and no
 * draft referenced any filler. Keep the drafts in sync; sync_work.py
 * reintroduces whatever the drafts say. */
struct Unk2A38C
{
    /* 0x00 */ u8 filler_00[0x02];
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
};

bool8 ResupplyUnitWithAnimation(struct Unk2A38C *p, int (*fn)(struct Unk2A38C *))
{
    const u16 *q;
    int r;
    int v;
    int t;

    if (gMap->unk234A[
            gMap->rowOffset[p->unk03] + p->unk02] == 0)
    {
        fn(p);
        return 0;
    }

    q = &gUnknown_030033EC;
    r = fn(p);

    if (r == 0)
        return 0;

    t = p->unk03 << 16;
    v = p->unk02 | t;
    StartSupplyAnimation((struct Unk802C57C *)&v, *q, r, 0);
    return 1;
}
asm(".global sub_0802A38C\n.thumb_set sub_0802A38C, ResupplyUnitWithAnimation\n");
