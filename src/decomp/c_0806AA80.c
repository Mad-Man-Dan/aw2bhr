#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AA80.
 * sub_0806AA80 @ 0x0806AA80
 */

#include "proc.h"

/* Both parameters are `int`, NOT the (s16, s16) include/unknown-functions.h
 * used to declare: the body opens `adds r4, r0, #0` / `adds r5, r1, #0` and
 * goes straight to `lsls r4, r4, #4`, with no `lsls #0x10; lsrs #0x10`
 * prologue pair -- and agbcc's PROMOTE_MODE emits that pair for EVERY sub-word
 * parameter regardless of signedness. sub_08044968, the only promoted caller,
 * was re-verified byte-exact against the wider declaration.
 *
 * The subtraction cannot be written inline. `a1 * 16 - (v - 8)` is folded by
 * `associate_trees` into `(a1 * 16 + 8) - v`, which is an `adds #8` before the
 * load; the ROM computes `v - 8` in its own register first, and only a
 * SEPARATE STATEMENT keeps it there. The `x -= d` shape is also what puts the
 * results in r4/r5 with `adds r1, r4, #0` copies at the call -- computing them
 * straight into the argument registers, as a single `x = a1 * 16 - d` does,
 * saves those two moves and is two instructions short. */
void StartMeteorImpact(int a1, int a2)
{
    ProcPtr proc;
    int x;
    int y;
    int dx;
    int dy;

    PlayMusicOrSfx2(0xC5);
    proc = Proc_Start(gUnknown_0858168C, PROC_TREE_3);

    x = a1 * 16;
    dx = gMap->scrollX - 8;
    x -= dx;

    y = a2 * 16;
    dy = gMap->scrollY - 8;
    y -= dy;

    SetupMeteorImpact(proc, x, y);
}
asm(".global sub_0806AA80\n.thumb_set sub_0806AA80, StartMeteorImpact\n");
