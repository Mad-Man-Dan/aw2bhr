#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080184E0.
 * sub_080184E0 @ 0x080184E0
 */

/* sub_080184D8 is the next callback in this sequence and lives in its own file;
 * it is declared here because src/decomp has no shared header for these slot
 * callbacks. */
void sub_080184D8(struct Unk0200C528 *s);

/*
 * sub_080184E0 -- hand a gUnknown_0200C528 slot on to sub_080184D8.
 *
 * Installed as a slot's callback by CoScreenWipeOut_Step (src/decomp/c_080184EC.c).
 * All it does is put the next callback in the sequence in its own place, so the
 * slot runs sub_080184D8 from the next frame on.
 *
 * The callback field .unk08 is declared as a node pointer, because
 * EventOp_InstallCallback stores a node link in it, so the function is cast rather than
 * the member retyped.
 */
void sub_080184E0(struct Unk0200C528 *s)
{
    s->unk08 = (struct Unk0200C528Node *)sub_080184D8;
}
