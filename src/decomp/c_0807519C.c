#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807519C.
 * sub_0807519C @ 0x0807519C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
/* MATCHED (wave 38, W38-E) via decomp-permuter, 279 iterations from the 97.7%
 * draft wave 35 left. Parked at size-exact with 4 bytes differing.
 *
 * THE RESIDUAL WAS ONE REGISTER NUMBER -- `by` took r3 and the ROM gives it r1,
 * because gcc coalesced `by` with `y` even though their live ranges do not
 * overlap. The lever is the `do { } while (0)` around the x block's ELSE arm
 * below. It is not decoration and it is not removable: with the else arm
 * written plainly the function goes straight back to 97.7% with the identical
 * 4-byte diff, and the declaration order `int y; int x;` that the permuter also
 * found does nothing on its own. Wave 35 had already ruled out moving `by`'s
 * assignment earlier (that also drags the +0x36 `ldrsh` ahead of the x block,
 * which the ROM does not have), so there is no plain-statement spelling left.
 *
 * Read this against docs/agbcc-codegen.md's wave-18 warning that a
 * `do { } while (0)` in a draft is usually COMPENSATING for a wrong aggregate
 * type. That warning does not apply here and the distinction is the useful
 * part: wave 35 had already settled the type question on this function -- the
 * coordinates are `int` locals carrying an explicit (s16) cast, worth 12 bytes
 * over `s16` locals -- and the residual survived at size-exact afterwards. A
 * do/while(0) standing on top of an ALREADY-CORRECT type model is a genuine
 * basic-block lever; one that changes the byte COUNT is the wave-18 case.
 *
 * Everything below this line is wave 35's, unchanged and still load-bearing:
 *
 * THE COORDINATES ARE `int` LOCALS CARRYING AN EXPLICIT (s16) CAST, not `s16`
 * locals. agbcc's PROMOTE_MODE promotes a `short` LOCAL to a SImode pseudo and
 * makes it UNSIGNED -- the rule the brief states for parameters applies to
 * locals too -- so a plain `s16 x` narrows with `lsls #16; lsrs #16` at the
 * assignment and then needs a SECOND `lsls #16; asrs #16` pair at every signed
 * use. That is four instructions per coordinate. Written as an `int` holding
 * `(s16)expr` the narrowing IS the sign extension: it happens once, at the
 * assignment, and lands straight in the argument register. The cast is pinned
 * to the ASSIGNMENT rather than to the call site by StartDifficultyStar's third and
 * fourth parameters being `int` -- nothing re-narrows after the merge.
 *
 * Each coordinate is a TWO-ARMED if, not a compound assignment. The x pair
 * cross-jumps completely: both arms leave the value in the same register, so
 * `lsls` and `asrs` both merge past the join. The y pair merges only the
 * `asrs`, because its arms leave the value in different registers, and THAT is
 * why the ROM carries an unconditional `b` there and a mid-function literal
 * pool after it and does not for x. The pool position is a consequence of the
 * `b`, not an independent fact.
 *
 * proc->unk30 is re-`ldr`ed three times because each read sits in a different
 * basic block and gcc 2.9's CSE table resets at a label. It is NOT a local.
 *
 * The body of ProcScr_WM_MoveScope: it spawns one child per step until
 * unk30 catches up with unk2c, parking each child's proc in the ten-entry table
 * at +0x3c, then breaks. The two `unk2c == unk30` tests are ONE shared break
 * block that agbcc cross-jumps, which is why the ROM has a single _08075230. */
struct Unk807519C
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ u16 unk38;
    /* 0x3a */ u16 unk3a;
    /* 0x3c */ void *unk3c[10];
};

void DifficultyStars_SpawnLoop(struct Unk807519C *proc)
{
    PutSprite(1, proc->unk34, proc->unk36, gUnknown_081CC4F0,
              gUnknown_0861433C[proc->unk38]);

    if (proc->unk2c != proc->unk30)
    {
        if ((proc->unk3a & 3) == 0)
        {
            u16 n;
            int bx;
            int by;
            int y;
            int x;

            n = proc->unk30 + 1;

            bx = proc->unk34 + proc->unk30 * 8;

            if (proc->unk30 > 4)
            {
                x = (s16)(bx - 0x28);
            }
            else
            {
                do { x = (s16)bx; } while (0);
            }

            by = proc->unk36 + 8;

            if (proc->unk30 > 4)
            {
                y = (s16)(by + 8);
            }
            else
            {
                y = (s16)by;
            }

            proc->unk3c[proc->unk30] =
                StartDifficultyStar(proc, n, x, y, proc->unk38);
            proc->unk30++;
        }
    }

    if (proc->unk2c == proc->unk30)
    {
        proc->unk3a = 0;
        Proc_Break(proc);
    }

    proc->unk3a++;
}

asm(".global sub_0807519C\n.thumb_set sub_0807519C, DifficultyStars_SpawnLoop\n");
