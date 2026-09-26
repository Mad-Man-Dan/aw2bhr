#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080184E0.
 * sub_080184E0 @ 0x080184E0
 */

/* Wave 32, W32-A: retyped from a file-local duplicate of the slot struct
 * (`struct Unk80184D8`, filler[8] + a callback word) to the shared
 * `struct Unk0200C528`. sub_080184EC installs THIS function into a slot's +8
 * and steps that slot's `unk0e`, which is what identifies the object; the two
 * files can now name one type. Byte-identical -- the +8 slot is declared
 * `struct Unk0200C528Node *` because sub_08018B40 stores a node link there, so
 * the function is cast rather than the member retyped (the c_080180A8.c
 * precedent). */
void sub_080184D8(struct Unk0200C528 *s);

void sub_080184E0(struct Unk0200C528 *s)
{
    s->unk08 = (struct Unk0200C528Node *)sub_080184D8;
}
