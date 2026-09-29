#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080290B0.
 * StartCameraScroll @ 0x080290B0
 */

/* MATCHED -- wave 60 (W60-D).  384/384 bytes.
 *
 * PROMOTION REQUIREMENT: needs its .rodata pool word PLACED.  The
 * data/promoted.json entry must carry  "rodata": ["0x08090B5C"]  and
 * tools/split_rodata.py + tools/gen_lds.py must be re-run before building.
 * That word is the -fforce-addr address constant for gUnknown_08499590;
 * trymatch reports it as "different symbols that resolve to the same address"
 * and calls it a match.
 *
 * THE WHOLE RESIDUAL WAS TWO DECLARATION INITIALISERS.  The wave-36 park
 * classified this as a pure three-way permutation of the high registers --
 *      ROM   dx -> ip,  dy -> r8,  force-addr word -> sb
 *      draft dx -> r8,  dy -> sb,  force-addr word -> ip
 * -- and called the draft "INSTRUCTION-EXACT".  IT WAS NOT.  `s16 dx = 0;` and
 * `s16 dy = 0;` as declaration initialisers make agbcc emit
 *      movs r0,#0 ; mov r8,r0 ; mov sb,r0
 * BEFORE the `bl SetMapCursorPosition`, where the ROM emits it AFTER.  Writing them as
 * plain statements after the call:
 *      SetMapCursorPosition(a1, a2);
 *      dx = 0;
 *      dy = 0;
 * moves the initialisation past the call and the entire three-cycle unwinds by
 * itself -- dx takes ip, dy takes r8, the pool word takes sb, and nothing else
 * in the function had to change.  Confirmed by compile_probe on both spellings
 * side by side before any try_match was spent.
 *
 * WHY, and this is the general rule (now in docs/agbcc-codegen.md): all three
 * pseudos tie on reference count, so global_alloc's allocno_compare decides
 * them on priority = log2(refs)*refs/LIVE_LENGTH.  A local initialised in its
 * DECLARATION has its live range start at block entry -- ahead of any call
 * that precedes its first real use -- which inflates allocno_live_length and
 * DROPS its priority.  Only one call-clobbered high register exists (ip), so
 * the highest-priority allocno takes it and the other two are pushed onto the
 * callee-saved pair.  Lengthening dx's range by a handful of insns is enough
 * to hand ip to the pool word instead.
 *
 * READ BACKWARDS THIS IS A CHEAP DIAGNOSTIC: if a candidate is size-exact and
 * differs only by which high register holds what, check whether any competing
 * local is initialised in its declaration while the ROM initialises it after a
 * preceding call.  Do not reach for the permuter first -- decomp-permuter ran
 * 4 x 300 s here (chained, each round from the previous best.c) and moved
 * nothing, because it never rewrites a declaration initialiser into a
 * statement.
 *
 * SETTLED, do not re-derive: the `*(u16 *)(gUnknown_08499590 + K)` spellings
 * (which avoid struct Map entirely and reproduce the ROM's three-level
 * force-addr chain), `s16` for dx/dy/cx/cy (the `lsls #16; asrs #16` at every
 * signed use is PROMOTE_MODE on an s16 LOCAL), and `/ 16` rather than `>> 4`.
 */

void StartCameraScroll(int a1, int a2, u8 a3)
{
    struct Unk03001470 *proc;
    s16 dx;
    s16 dy;
    s16 cx;
    s16 cy;

    SetMapCursorPosition(a1, a2);

    dx = 0;
    dy = 0;

    cx = gMap->scrollX / 16;
    cy = gMap->scrollY / 16;

    if (a3 != 0)
    {
        a1 -= 7;
        a2 -= 4;

        if (a1 < 0)
            a1 = 0;

        if (a2 < 0)
            a2 = 0;

        if (a1 + 15 > gMap->width)
            a1 = gMap->width - 15;

        if (a2 + 10 > gMap->height)
            a2 = gMap->height - 10;

        if (cx == a1 && cy == a2)
            return;
    }
    else
    {
        if (a1 < cx + 2 && cx != 0)
            dx = -2;

        if (a1 > cx + 12 && cx + 15 < gMap->width)
            dx = -12;

        if (a2 < cy + 2 && cy != 0)
            dy = -2;

        if (a2 > cy + 7 && cy + 10 < gMap->height)
            dy = -7;

        if (dx != 0)
            a1 = a1 + dx;
        else
            a1 = cx;

        if (dy != 0)
            a2 = a2 + dy;
        else
            a2 = cy;

        if (a1 < 0)
            a1 = 0;

        if (a2 < 0)
            a2 = 0;

        if (a1 + 15 > gMap->width)
            a1 = gMap->width - 15;

        if (a2 + 10 > gMap->height)
            a2 = gMap->height - 10;

        if (dx == 0 && dy == 0)
            return;
    }

    gMap->unk10 = 0;

    proc = sub_080152EC(gUnknown_0849A00C, 0);
    proc->unk22 = a1 << 4;
    proc->unk24 = a2 << 4;
    IncrementMapLock();
}

asm(".global sub_080290B0\n.thumb_set sub_080290B0, StartCameraScroll\n");
