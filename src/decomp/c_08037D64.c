#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037D64.
 * sub_08037D64 @ 0x08037D64
 */

/* A byte-identical LOOSE duplicate of the promoted StartResumeScript: same
 * instruction stream, only the pool symbol differs. `u16` and not `int` for
 * the reason recorded there -- the `adds r4,r0,#0; lsls #0x10; lsrs #0x10`
 * prologue is PROMOTE_MODE's copy-then-narrow of a sub-word parameter, and an
 * `int` with a `(u16)` cast at the use folds the pair away because the `strh`
 * into the s16 field truncates for free. */
void sub_08037D64(u16 a)
{
    sub_080152EC(gUnknown_0849D53C, 0)->unk1e = a;
}
