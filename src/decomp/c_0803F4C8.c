#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F4C8.
 * sub_0803F4C8 @ 0x0803F4C8, sub_0803F510 @ 0x0803F510
 */

#include "proc.h"
struct UnkF4C8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x50);
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
    /* 58 */ int unk58;
};
struct UnkF510Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
};

/* gUnknown_08499590 is named twice and CSE keeps the single deref in r3; the
 * two `movs #K; ldrsh rD, [r3, rK]` pairs are signed halfwords at +4 and +6 of
 * the map descriptor, i.e. the camera origin this subtracts to get
 * camera-relative coordinates.
 *
 * The `lsls r0, r0, #0x18; cmp r0, #0` after the call is agbcc re-narrowing a
 * BYTE-returning callee, which is what retyped AP_Update from `int` to `u8`
 * in unknown-functions.h -- with `int` the test is a bare `cmp r0, #0`.
 */
void VolcanoRock_WaitImpact(struct UnkF4C8Proc *proc)
{
    if (AP_Update(proc->unk50, (proc->unk54 - gMap->scrollX) & 0x1FF, (proc->unk58 - gMap->scrollY) & 0xFF) == 0)
    {
        AP_Delete(proc->unk50);
        Proc_Break(proc);
    }
}
asm(".global sub_0803F4C8\n.thumb_set sub_0803F4C8, VolcanoRock_WaitImpact\n");

/* `adds r1, r2, #0` before the bl -- the parent is the third parameter, the
 * same shape src/decomp/c_0803F3E4.c has. Both stored fields are words; the
 * `lsls #0x10; asrs #0x10` pairs afterwards are the conversions to
 * ScrollCameraToCenterCell's `s16` parameters and apply to the arguments, not the fields. */
void StartDeathRayFire(int a, int b, ProcPtr parent)
{
    struct UnkF510Proc *proc = Proc_StartBlocking(gUnknown_0849F940, parent);

    proc->unk2c = a;
    proc->unk30 = b;

    ScrollCameraToCenterCell(a + 1, b + 1);
}
asm(".global sub_0803F510\n.thumb_set sub_0803F510, StartDeathRayFire\n");
