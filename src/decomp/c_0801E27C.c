#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E27C.
 * sub_0801E27C @ 0x0801E27C
 */

/* Wave 56, W56-H. The value parameters of these four were retyped `u16` ->
 * `s16`. Their only caller is sub_0801D390, which reads each argument with
 * `ldrsh` off the script stream or narrows a computed one with
 * `lsls #0x10; asrs #0x10`; a `u16` parameter forces `ldrh` / `lsrs` at the
 * caller instead. These bodies are byte-identical either way (the members they
 * store into are u16) and were re-verified by trymatch after the change. */
void StartObjAffineScaleTween(int index, s16 a, s16 b, s16 c)
{
    gUnknown_0200F720[index].unk06 = a;
    gUnknown_0200F720[index].unk08 = b;
    gUnknown_0200F720[index].unk0a = c;
}
asm(".global sub_0801E27C\n.thumb_set sub_0801E27C, StartObjAffineScaleTween\n");
