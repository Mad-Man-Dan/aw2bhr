#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E0D0.
 * sub_0803E0D0 @ 0x0803E0D0
 */

/* An 8-byte record; only the halfword at +2 (a flag word, tested against
 * 0x3c0) is named by this function, and the whole element is copied whole.
 */
struct Unk3E0D0
{
    /* 00 */ u16 unk00;
    /* 02 */ u16 unk02;
    /* 04 */ u32 unk04;
};

/* Compacts a list in place: while the cursor's flags have any of 0x3c0 set,
 * shift the next element down over it. `q[0] = q[1]` is a whole-struct
 * assignment -- both words load before either stores, and agbcc folds the
 * `q++` into the `stmia rQ!` writeback.
 *
 * DECLARED non-void WITH NO RETURN STATEMENT, deliberately. Both tells from
 * the missing-return rule in docs/agbcc-codegen.md are present: r0 is never
 * written yet the epilogue pops into r1 (so r0 is live-out), and the cursor is
 * pushed out to r3 by an `adds r3, r0, #0` that a void function would not
 * need. Adding `return p;` costs a trailing `adds r0, r3, #0` the ROM does not
 * have, and keeping a second local for the cursor costs an extra copy at the
 * top. DestroyLaserOrMinicannon discards the result.
 */
struct Unk3E0D0 *RemoveInventionRecord(struct Unk3E0D0 *p)
{
    while (p->unk02 & 0x3c0)
    {
        p[0] = p[1];
        p++;
    }
}
asm(".global sub_0803E0D0\n.thumb_set sub_0803E0D0, RemoveInventionRecord\n");
