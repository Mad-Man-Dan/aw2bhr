#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040150.
 * sub_08040150 @ 0x08040150
 */

#include "proc.h"
/* MATCHED -- wave 35, W35-G. 60/60 bytes, relocs match.
 *
 * THE LEVER, and it is new and general: an ASSIGNMENT INSIDE THE EXPRESSION
 * stops agbcc's fold() reassociating `A - (B - k)` into `(A + k) - B`, WITHOUT
 * costing the statement boundary that a separate statement would.
 *
 * W35-A's park note had the tension exactly right: only a statement boundary
 * survived fold(), but a statement boundary also hoists the whole
 * `ldr =gUnknown_08499590; ldr r3,[r2]` chain and both `ldrsh`es above
 * `ldr r0,[r4,#0x50]`, where the ROM interleaves them inside arguments 2 and 3.
 * Writing the subtraction as `(dx = B - 8)` in the operand position resolves
 * both halves at once: fold()'s split_tree will not peel a literal out of a
 * MODIFY_EXPR, so the `-8` stays on the map field (`subs r2,#8; subs r1,r1,r2`),
 * while the expression stays inside the argument so the map pointer's CSE
 * materialises where the ROM has it.
 *
 * The assignment must sit on the RIGHT of the outer `-`: gcc evaluates
 * `proc->unk2c * 16` first, which is the ROM's `ldr r1,[r4,#0x2c]; lsls r1,#4`
 * ahead of the map load. `dx`/`dy` die at once and cost no register.
 *
 * Everything W35-A ruled out stands: `8u`/`0x10u`, `volatile s16` fields and a
 * file-scope `static const int` all leave the reassociation in place. */
struct Unk40150Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x50 - 0x34];
    /* 50 */ struct Unk0801C210 *unk50;
};

void ExplosionEffect_Loop(struct Unk40150Proc *proc)
{
    int dx;
    int dy;

    if (!AP_Update(proc->unk50,
                      proc->unk2c * 16
                          - (dx = gMap->scrollX - 8),
                      proc->unk30 * 16
                          - (dy = gMap->scrollY - 0x10)))
        Proc_Break(proc);
}
asm(".global sub_08040150\n.thumb_set sub_08040150, ExplosionEffect_Loop\n");
