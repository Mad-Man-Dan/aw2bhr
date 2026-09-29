#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080482D8.
 * sub_080482D8 @ 0x080482D8
 */

/* MATCHED, wave 71, 500/500 configured.  The wave-55 park below correctly
 * identified the affine fold, but its claim that the if/else allocation shape
 * was settled was false.  A `switch` with default first gives the ROM's
 * dispatch and keeps `p` in sl.  Splitting `x` as `x = i; x <<= 4; x += 0x38`
 * blocks the live-through fold.  A post-check `id = v` creates the target r7
 * copy, and `y = t + K; y *= 8` forces the target in-place add/shift.
 *
 * Historical wave-55 evidence follows.
 *
 * SETTLED, do not re-litigate:
 *  - Parameter is `struct Unk0804769C *` (retyped in unknown-functions.h this
 *    wave; c_080484CC.c already passed one). UnitList_DrawSprites sees the SAME object
 *    through c_08047B98.c's file-local `struct Unk08047B98`, which is now
 *    forward-declared in the header so the cast at the call site is all that is
 *    needed and neither promoted file had to be edited.
 *  - `p->unk20` needs its own `adds rN, #0x20` because 0x20 is outside the
 *    `ldrb rd,[rn,#imm5]` range. That is NOT an address bind and there is
 *    nothing to spell for it.
 *  - `EaseMapCursorAndDraw(0x1e, (p->unk1f - p->unk20) * 16 + 0x36, 3)`. The
 *    `lsls #0x14 / +0xd8<<0xe / asrs #0x10` triple is the s16 conversion of
 *    `x * 16 + 0x36` done in the high half -- read it as one (s16) cast, not as
 *    a shift and a mask.
 *  - The loop is `for (i = 0; i <= 5 && gUnknown_02028DD8[p->unk20 + i] != 0xff;
 *    i++)` with the array read done AGAIN at the top of the body into `v`; the
 *    ROM reads it twice and there is no CSE across the back edge.
 *  - RETRACTED in wave 71: the arms are a switch; this is what puts the default
 *    body inline after both compares and produces the matching allocation.
 *  - Inside each arm, `y` is assigned BEFORE `x`. Assigning x first swaps
 *    `lsls r5,#3` and `lsls r4,#4` in every arm.
 *  - The last call in each arm takes `x += 4` (destructive `adds rN, #4`), not
 *    `x + 4`; `x + 4` folds to `i * 16 + 60` and loses the chain.
 *
 * HISTORICAL REMAINING DIFF. In the two arms that emit three or four sprites,
 * the second sprite's x is `x + 8`. agbcc folds that back through x's own
 * definition to `i * 16 + 64`, which keeps `i * 16` live in its own register,
 * so this body pays TWO instructions the ROM does not:
 *
 *     ROM        lsls r4, r0, #4 ; adds r4, #56          <- i*16 dies into x
 *                adds r2, r4, #0 ; adds r2, #8           <- +8 off x
 *     candidate  lsls r4, r1, #4 ; adds r5, r4, #0 ; adds r5, #56
 *                adds r2, r4, #0 ; adds r2, #64          <- +64 off i*16
 *
 * i.e. the ROM's `i * 16` has ONE use and this candidate's has two. The cost is
 * one `adds rN, rM, #0` per arm plus the register cascade it causes: `t` is
 * pushed into r8 and one extra value spills, so the frame is `sub sp, #12`
 * where the ROM has `#8`, and the trailing `.short 0` alignment disappears.
 *
 * Ruled out, with the measurement:
 *  - `x + 4` -> `x += 4`: FIXES the third offset (`adds r5, #4`), does not
 *    touch the +8. Kept in this body.
 *  - dropping the `y` local: loses the y-before-x ordering in every arm and
 *    changes nothing about the fold. Reverted.
 *  - `int` vs `u32` for x: byte-identical.
 * At that point untried was a spelling in which the
 * second sprite's x is not an affine function of `i` that cse.c can rebuild --
 * every form of `x + 8`, `x += 8` and `i * 16 + 0x40` reduces to the same rtx.
 * Wave 71 showed that blocking that fold and correcting the dispatch allocation
 * were both required.
 */

void UnitList_DrawFrame(struct Unk0804769C *p)
{
    u16 i;
    u16 v;
    u8 id;
    u16 t;
    u16 r;
    int x;
    int y;
    struct Unit *e;

    if (p->unk21 != 0)
    {
        UnitList_HandleInput((struct Unk08047B98 *)p);
        EaseMapCursorAndDraw(0x1e, (p->unk1f - p->unk20) * 16 + 0x36, 3);
    }

    UnitList_DrawSprites((struct Unk08047B98 *)p);
    DrawOamObject(gPlayers[gUnknown_030033EC].teamColor + 0x3d, 0xf, 8, 0, 0);
    DrawOamObject(1, 0xaa, 8, 0, 0);

    for (i = 0; i <= 5 && gUnknown_02028DD8[p->unk20 + i] != 0xff; i++)
    {
        v = gUnknown_02028DD8[p->unk20 + i];
        e = &gUnits[gUnknown_03003F2C + v];
        if (e->type == 0)
            break;

        id = v;
        t = sub_0804769C(p, id) + 1;
        if (e->flags & 8)
        {
            r = sub_08047740(p, id, t - 1, p->unk20 + i);
            switch (r)
            {
            default:
                y = t + 2;
                y *= 8;
                x = i;
                x <<= 4;
                x += 0x38;
                PutSprite(0, y, x, (u16 *)gUnknown_0848B688, 0x8208);
                x += 4;
                PutSprite(0, y, x, (u16 *)gUnknown_0848B688, 0x8209);
                break;
            case 1:
                y = t + 2;
                y *= 8;
                x = i;
                x <<= 4;
                x += 0x38;
                PutSprite(0, y, x, (u16 *)gUnknown_0848B688, 0x8208);
                PutSprite(0, y, x + 8, (u16 *)gUnknown_0848B688, 0x8208);
                x += 4;
                PutSprite(0, y, x, (u16 *)gUnknown_0848B688, 0x8209);
                break;
            case 2:
                y = t + 1;
                y *= 8;
                x = i;
                x <<= 4;
                x += 0x38;
                PutSprite(0, y, x, (u16 *)gUnknown_0848B688, 0x8208);
                PutSprite(0, y, x + 8, (u16 *)gUnknown_0848B688, 0x8208);
                y = t + 2;
                y *= 8;
                PutSprite(0, y, x, (u16 *)gUnknown_0848B688, 0x8208);
                x += 4;
                PutSprite(0, y, x, (u16 *)gUnknown_0848B688, 0x8209);
                break;
            }
        }
    }
}
asm(".global sub_080482D8\n.thumb_set sub_080482D8, UnitList_DrawFrame\n");
