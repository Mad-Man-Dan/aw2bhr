#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080680E4.
 * sub_080680E4 @ 0x080680E4
 */

/* Lays out the 0x08581210 menu proc: how many rows it has (+0x2c), how tall
 * that makes it (+0x2e = rows * 36), where it slides in from (+0x34/+0x36), the
 * per-row enable byte table at +0x38, and the initially-selected row (+0x4c).
 * +0x4d picks the side: 0 slides in from x = 0xf0 (offscreen right), non-zero
 * from x = -height. No calls and no globals -- this is pure struct arithmetic.
 *
 * Two spellings are load-bearing and both were probed:
 *
 * `n` is a local, not a re-read of proc->unk2a. The ROM assigns it in BOTH arms
 * of the row-count if/else (`adds r2, r0, #0` / `ldrb r2, [r2]`) and then uses
 * it only in the far arm's bound, `n + 1`. Written as `proc->unk2a + 1` there
 * instead, agbcc keeps the ADDRESS proc+0x2a in a callee-saved register and
 * re-loads through it inside the loop, where the ROM carries the VALUE and
 * hoists `n + 1` into the preheader. `lo` is the same story one register over.
 *
 * `proc->unk36 = proc->unk34 - proc->unk2e + 0x1e` reads back the field this
 * function just stored, and that is deliberate: CSE supplies the 0xf0 still
 * live in r0 from the `strh`, so no reload is emitted. Spelled with the
 * constant instead -- `0xf0 - proc->unk2e + 0x1e` -- `fold` reassociates
 * `(C1 - x) + C2` into `(C1 + C2) - x` and materialises 0x10e as
 * `movs #0x87; lsls #1`, which is 4 bytes the ROM does not have. A memory read
 * is the one operand fold cannot reassociate through. */

struct Unk680E4Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 filler_2b[0x01];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u8 filler_30[0x04];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ u8 unk38[0x14];
    /* 0x4c */ u8 unk4c;
    /* 0x4d */ u8 unk4d;
    /* 0x4e */ u8 unk4e;
};

void IntroCoNameBanner_Init(struct Unk680E4Proc *proc)
{
    int i;
    int lo;
    int n;

    if (proc->unk2a <= 3)
    {
        proc->unk2c = proc->unk2a * 3 + 1;
        lo = proc->unk2a * 2;
        n = proc->unk2a;
    }
    else
    {
        proc->unk2c = proc->unk2a * 2 + 1;
        n = proc->unk2a;
        lo = n;
    }

    proc->unk2e = proc->unk2c * 36;
    proc->unk4e = 0x2e - (proc->unk2a - 3) * 5;

    if (proc->unk4d == 0)
    {
        proc->unk34 = 0xf0;
        proc->unk36 = proc->unk34 - proc->unk2e + 0x1e;

        for (i = proc->unk2c - 1; i >= 0; i--)
        {
            if (i >= lo && i < proc->unk2c - 1)
            {
                proc->unk4c = i;
                proc->unk38[i] = 1;
            }
            else
            {
                proc->unk38[i] = 0;
            }
        }
    }
    else
    {
        proc->unk34 = -proc->unk2e;
        proc->unk36 = -0x1e;

        for (i = proc->unk2c - 1; i >= 0; i--)
        {
            if (i > 0 && i < n + 1)
            {
                proc->unk4c = i;
                proc->unk38[i] = 1;
            }
            else
            {
                proc->unk38[i] = 0;
            }
        }
    }
}
asm(".global sub_080680E4\n.thumb_set sub_080680E4, IntroCoNameBanner_Init\n");
