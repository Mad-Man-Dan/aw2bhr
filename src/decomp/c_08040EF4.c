#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040EF4.
 * sub_08040EF4 @ 0x08040EF4
 */

#include "hardware.h"
/* Wave 80 (W80-C). Two of the three residual facts were SOURCE CONSTRUCTS,
 * and both are fixed below; see the tail comment for what remains.
 *
 * 1. The shared base is `proc->unk42 + (proc->unk3e - 0x10)`, NOT
 *    `proc->unk42 - 0x10 + proc->unk3e`. gcc 2.95 fold's `associate:` rule
 *    rewrites `(VAR - CON) + ARG1` into `VAR + (ARG1 - CON)`, so the old
 *    spelling was evaluated as unk42 + (unk3e - 16): address of unk42
 *    computed, then `ldrh unk3e; subs #16`, then the deferred `ldrh unk42`
 *    and the add. The ROM evaluates unk42, subtracts 16, THEN loads unk3e
 *    (`ldrh; subs #16; ldrh; adds`). The mirror rule `ARG0 + (VAR - CON)`
 *    -> `(ARG0 - CON) + VAR` produces exactly that from the spelling used
 *    here. A `(s16)` cast on `unk42 - 0x10` also blocks the reassociation
 *    (measured) but its narrowing survives, +4 bytes.
 *
 * 2. `u`'s add is `adds r5, r5, r0` -- the Div term is the FIRST operand,
 *    yet its sign extension is emitted AFTER the base. Writing `(s16)Div(..)
 *    + base` in one expression puts the sext before the base (W78's 90.9%
 *    regression) because the cast is expanded with its operand. Binding the
 *    Div result to an int `d` and the base to a local in the statement
 *    AFTER the second Div (wave 66 bound it BEFORE, which is what crossed
 *    the Div and spilled y) gives: call, base, sext(d), add(d, base). Frame
 *    stays 12 bytes.
 *
 * x is s16 and y is not: the ROM keeps `x << 16` on the stack and derives
 * both an `asrs` (the OR into the attribute word) and an `lsrs` (the u16
 * argument of DrawSpriteNumberFont2) from it, which only a signed halfword object
 * does. `s != 0 ? s : 2` is written out twice (two identical
 * `movs r1,#2; cmp; beq; mov r1,rS` blocks). */
struct Unk40EF4Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ struct Unk0801C210 *unk2c;
    /* 30 */ u8 filler_30[0x04];
    /* 34 */ struct Unk0801C210 *unk34;
    /* 38 */ struct Unk0801C210 *unk38;
    /* 3c */ u16 unk3c;
    /* 3e */ u16 unk3e;
    /* 40 */ u8 filler_40[0x02];
    /* 42 */ u16 unk42;
    /* 44 */ u8 filler_44[0x02];
    /* 46 */ u16 unk46;
};

void CaptureAnimSprites_Loop(struct Unk40EF4Proc *proc)
{
  s16 x;
  short new_var;
  s16 s;
  short y;
  int u;
  int v;
  int d;
  int base;
  do
  {
    x = proc->unk3c & 0x1ff;
  }
  while (0);
  y = proc->unk3e & 0xff;
  s = Div((0x14 - proc->unk46) * 0x100, 0x14);
  new_var = y;
  d = Div((0x100 - s) * 16, 0x100);
  base = proc->unk42 + (proc->unk3e - 0x10);
  u = (((s16) d) + base) & 0xff;
  v = (((s16) ((proc->unk46 - 0x14) * 2)) + base) & 0xff;
  proc += 0;
  AP_Update(proc->unk2c, x | 0x200, new_var | 0x100);
  AP_Update(proc->unk38, x | 0x600, u | 0x100);
  AP_Update(proc->unk34, x | 0x400, v | 0x100);
  SetObjAffine(3, Div(gSinLut[0x40] * 16, 0x100), Div((-gSinLut[0]) * 16, (s != 0) ? (s) : (2)), Div(gSinLut[0] * 16, 0x100), Div(gSinLut[0x40] * 16, (s != 0) ? (s) : (2)));
  DrawSpriteNumberFont2(x, new_var - 0xc, 0x14 - proc->unk46);
}
asm(".global sub_08040EF4\n.thumb_set sub_08040EF4, CaptureAnimSprites_Loop\n");
