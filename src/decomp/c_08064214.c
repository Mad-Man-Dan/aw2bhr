#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064214.
 * sub_08064214 @ 0x08064214
 */

/* Composes three axis rotations into one matrix and transforms the eight
 * gUnknown_0202F140 vectors by it.
 *
 * The three 0x30-byte matrices are plain `int[12]` frame slots, which is what
 * `sub sp, #0x90` and the `mov r0, sp` / `add rN, sp, #0x30` / `#0x60`
 * argument setup say. They are CAST at each call rather than declared with the
 * callees' struct tags: struct Mtx43, Unk64034Mtx, Unk6407CMtx and Vec3 are all
 * defined file-locally in src/decomp/ (c_08063DDC.c, c_08064034.c) and are only
 * forward-declared in include/unknown-functions.h, so this unit cannot name a
 * complete one -- and defining its own copy is exactly what c_08064034.c's
 * header comment warns breaks a promote.py merge. The casts are byte-neutral.
 *
 * The parameters are `int`, not `s16`: each is copied whole in the prologue
 * (`adds rN, r0, #0`) and only then narrowed with `lsls #0x10; asrs #0x10` at
 * its own call, which is copy-then-narrow -- an `s16` parameter would have been
 * narrowed once at entry.
 *
 * MultiplyMatrix43 is called with its second and third arguments the same pointer,
 * so it composes in place; the two calls chain m0*m1 into m1 and m1*m2 into m2,
 * leaving m2 as the one the loop uses. */
void UnusedRotateCubeVertices(int a1, int a2, int a3)
{
    int m0[12];
    int m1[12];
    int m2[12];
    int i;

    BuildRotationMatrixX((struct Mtx43 *)m0, (s16)a1);
    BuildRotationMatrixY((struct Unk64034Mtx *)m1, (s16)a2);
    BuildRotationMatrixZ((struct Unk6407CMtx *)m2, (s16)a3);
    MultiplyMatrix43((struct Mtx43 *)m0, (struct Mtx43 *)m1, (struct Mtx43 *)m1);
    MultiplyMatrix43((struct Mtx43 *)m1, (struct Mtx43 *)m2, (struct Mtx43 *)m2);

    for (i = 0; i < 8; i++)
        TransformVec3ByRotation((struct Vec3 *)gUnknown_0202F140[i].unk00,
                     (struct Mtx43 *)m2,
                     (struct Vec3 *)gUnknown_0202F140[i].unk0c);
}
asm(".global sub_08064214\n.thumb_set sub_08064214, UnusedRotateCubeVertices\n");
