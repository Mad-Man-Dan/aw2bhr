#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080394B4.
 * sub_080394B4 @ 0x080394B4, sub_08039544 @ 0x08039544
 */

/* The text proc's per-frame script step. The proc is sub_080152EC's slot seen
 * through this subsystem's own view: the halfwords struct Unk03001470 types as
 * unk1e/unk20/unk22/unk24 are the tick counter, the script cursor and the pen,
 * and everything from +0x26 on is the byte script StartCoPowerNameBanner copied in. That
 * is the same reinterpretation of +0x26 c_08039544.c uses and NOT a claim that
 * Unk03001470's `u16 unk26` is wrong. */
struct Unk394B4Proc
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ int unk08;
    /* 0x0c */ u8 filler_0c[0x12];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8 unk26[0x10];
};
/* The slot sub_080152EC hands back, viewed the way the 0x08039xxx text procs
 * use it: everything from +0x26 on is a byte script buffer. struct
 * Unk03001470 types +0x26 as a `u16` for a different subsystem; this is the
 * same bytes under another view, NOT a correction to that member. */
struct Unk39544Slot
{
    /* 0x00 */ u8 filler_00[0x26];
    /* 0x26 */ u8 unk26[0x10];
};

/* Four readouts worth keeping.
 *
 * 1. `again` IS A REAL VARIABLE, not a `do { } while (0)` wrapper. It is set to
 *    0 at the top of the body and tested at the bottom; agbcc has no global
 *    constant propagation, so the `movs r6,#0` / `cmp r6,#0; bne` pair survives
 *    even though nothing that lives ever stores 1 into it. The control-code
 *    arm's `again = 1; continue;` is what the ROM's lone `b <loop top>` is:
 *    `continue` in a `do`/`while` jumps to the TEST, jump.c threads that
 *    through the now-known-true test back to the top, and flow analysis then
 *    deletes the dead `movs r6,#1`.
 *
 * 2. THE TERMINATOR IS THE `else`, not the leading `if`. `proc->unk08 = 0` is
 *    emitted LAST, after the whole glyph path and immediately before the loop
 *    test, with the glyph path ending in `b <test>` -- that is gcc's
 *    then/`b end`/else layout, so the source tests `c != 0` and puts the store
 *    in the else. Written `if (c == 0) { store; } else if (...)` the store goes
 *    inline behind a `bne` and 86 of 144 bytes move, at an exact size.
 *
 * 3. `case 0:` SHARES `default:`, and that is the whole reason the dispatch
 *    carries `cmp #0x69; ble <default>`. A two-case switch never emits the
 *    low-bound test (wave 37, W37-A) -- with only {0x69, 0x6c} the tree is a
 *    two-node chain and agbcc emits just the two equality tests, 4 bytes
 *    short. A third node BELOW 0x69 makes it a two-children root, and its
 *    `cmp #0; beq` folds away only because the label it jumps to IS the
 *    default label; given its own body (probed with `case 0x21: break;`) the
 *    test survives instead. This is wave 28's W28-B rule, and the case VALUE
 *    is unobservable beyond being < 0x69.
 *
 * 4. THE TWO `+ 6` ARMS ARE ORDERED 0x6c THEN 0x69. Written the other way
 *    round agbcc cross-jumps the identical bodies into one and emits
 *    `beq; beq; <default>; <shared +6>`; in source order 0x6c-first the last
 *    dispatch test falls through into the 0x6c body and the 0x69 body is
 *    reached by the `beq`, which is the ROM's layout. */
void CoPowerNameBanner_Loop(struct Unk394B4Proc *proc)
{
    int again;
    u8 c;

    do
    {
        again = 0;

        if (proc->unk1e++ > 1)
        {
            proc->unk1e = 0;

            c = proc->unk26[proc->unk20];

            if (c != 0)
            {
                if ((u8)(c - 0x18) <= 8)
                {
                    proc->unk20++;
                    proc->unk22 += 8;
                    proc->unk1e = 2;
                    again = 1;
                    continue;
                }
                else
                {
                    SpawnCoPowerNameLetter(proc->unk22, proc->unk24);
                    proc->unk24++;

                    switch (proc->unk26[proc->unk20])
                    {
                    case 0x6c:
                        proc->unk22 += 6;
                        break;
                    case 0x69:
                        proc->unk22 += 6;
                        break;
                    case 0:
                    default:
                        proc->unk22 += 0xe;
                        break;
                    }

                    proc->unk20++;
                }
            }
            else
            {
                proc->unk08 = 0;
            }
        }
    } while (again);
}
asm(".global sub_080394B4\n.thumb_set sub_080394B4, CoPowerNameBanner_Loop\n");

/* Start the text proc, then copy a NUL-terminated byte string into its script
 * buffer -- the buffer CoPowerNameBanner_Loop later walks.
 *
 * THE BUFFER IS A MEMBER ARRAY, not `(u8 *)proc + 0x26`. Written with pointer
 * arithmetic, fold's `(A + C) + B -> (A + B) + C` reassociation moves the
 * constant past the index, the loop body computes the whole address itself and
 * nothing is left invariant to hoist: 64 bytes, proc in a scratch register,
 * 33.8%. As a COMPONENT_REF the +0x26 is applied at expand time, fold never
 * sees it, LICM hoists `proc + 0x26` into the preheader and the terminator's
 * store recomputes it -- which is why the ROM spends three instructions there
 * where two would do.
 *
 * The parameter is real. unknown-functions.h declared this `void (void)`
 * because the caller "sets up no argument register at all"; r0 is read before
 * being written here, and the call site is
 * `StartCoPowerNameBanner(GetArmyCoPowerName(proc->unk54))` -- two sequential `bl`s carrying a
 * nested call, which needs no instruction between them. */
void StartCoPowerNameBanner(u8 *str)
{
    struct Unk39544Slot *proc;
    u8 i;

    proc = (struct Unk39544Slot *)sub_080152EC(gUnknown_0849D76C, 0);

    for (i = 0; str[i] != 0; i++)
        proc->unk26[i] = str[i];

    proc->unk26[i] = 0;
}
asm(".global sub_08039544\n.thumb_set sub_08039544, StartCoPowerNameBanner\n");
