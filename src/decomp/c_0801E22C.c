#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E22C.
 * sub_0801E22C @ 0x0801E22C, sub_0801E248 @ 0x0801E248, sub_0801E264 @ 0x0801E264
 */

/* Writes the first three halfwords of a 0x10-byte record and notifies. The
 * three members were filler until this wave; SetObjAffineRecordScale and SetObjAffineRecordAngle
 * write prefixes of the same run, which is what separates them as members
 * rather than one wider field.
 *
 * UpdateObjAffineRecord TAKES THE INDEX, and this function is one of the three that
 * measure it: r0 is never rewritten, and the record address goes to r4 with
 * the pool word in r5 -- two callee-saved registers pushed by a function that
 * has nothing to keep across the call. Declared `UpdateObjAffineRecord(void)`, agbcc
 * reuses the dead r0 for the address, drops to `push {r4, lr}`, and misses. */
void SetObjAffineRecord(int index, u16 a, u16 b, u16 c)
{
    gUnknown_0200F720[index].unk00 = a;
    gUnknown_0200F720[index].unk02 = b;
    gUnknown_0200F720[index].unk04 = c;
    UpdateObjAffineRecord(index);
}
asm(".global sub_0801E22C\n.thumb_set sub_0801E22C, SetObjAffineRecord\n");

/* SetObjAffineRecord's two-member form. One fewer parameter frees r3, so the record
 * address lands there and only the pool word needs r4 -- `push {r4, lr}` where
 * its three-member sibling pushes r4 and r5. That descending register pattern
 * across the three writers is the readout that r0 stays live for
 * UpdateObjAffineRecord's argument. */
/* Wave 56, W56-H. The value parameters of these four were retyped `u16` ->
 * `s16`. Their only caller is sub_0801D390, which reads each argument with
 * `ldrsh` off the script stream or narrows a computed one with
 * `lsls #0x10; asrs #0x10`; a `u16` parameter forces `ldrh` / `lsrs` at the
 * caller instead. These bodies are byte-identical either way (the members they
 * store into are u16) and were re-verified by trymatch after the change. */
void SetObjAffineRecordScale(int index, s16 a, s16 b)
{
    gUnknown_0200F720[index].unk00 = a;
    gUnknown_0200F720[index].unk02 = b;
    UpdateObjAffineRecord(index);
}
asm(".global sub_0801E248\n.thumb_set sub_0801E248, SetObjAffineRecordScale\n");

/* The third member alone. Two parameters leave r2 and r3 free, so nothing is
 * saved at all and the prologue is a bare `push {lr}` -- the bottom of the
 * descending pattern its two siblings sit above. */
void SetObjAffineRecordAngle(int index, s16 a)
{
    gUnknown_0200F720[index].unk04 = a;
    UpdateObjAffineRecord(index);
}
asm(".global sub_0801E264\n.thumb_set sub_0801E264, SetObjAffineRecordAngle\n");
