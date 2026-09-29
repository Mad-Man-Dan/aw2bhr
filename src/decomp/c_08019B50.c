#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019B50.
 * sub_08019B50 @ 0x08019B50
 */

/* The 0x48-byte object the 0x08019A60-0x08019D48 group walks. It is NOT the
 * object Menu_PlaceCursorSprite/Menu_SlideCursorSprite take: both arrive as a ProcPtr, but this one
 * holds a word pointer at +0x20 where that one holds an `ldrsh` halfword, so
 * they are different objects and keep different tags. */
struct Unk08019B50Cmd /* 0x20 */
{
    /* 0x00 */ u8 filler_00[0x0c];
    /* 0x0c */ void (*unk0c)(u8, u8, u8);
    /* 0x10 */ u8 filler_10[0x10];
};
struct Unk08019B50 /* 0x48 */
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ struct Unk08019B50Cmd *unk20;
    /* 0x24 */ u8 unk24[0x0d];
    /* 0x31 */ u8 unk31[0x10];
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 filler_43[0x01];
    /* 0x44 */ struct Unk03001470 *unk44;
};

/* `void *` because src/decomp/c_08019D00.c already forwards one; the local
 * struct is the model, not the signature.
 *
 * `bl _call_via_r4` does NOT mean four arguments here, and this is the
 * counter-example to reading the register index as the arity. The callback is
 * loaded into r4 while r3 still holds the live index, long before r3 dies, so
 * r4 is a register-allocation outcome; the arguments are the three registers
 * the call site actually writes, r0, r1 and r2. */
void Menu_CallCursorHook(void *arg)
{
    struct Unk08019B50 *p = (struct Unk08019B50 *)arg;
    u8 i = p->unk42;
    u8 j = p->unk31[i];
    void (*fn)(u8, u8, u8);

    fn = p->unk20[j].unk0c;
    if (fn != NULL)
        fn(j, i, p->unk24[j]);
}
asm(".global sub_08019B50\n.thumb_set sub_08019B50, Menu_CallCursorHook\n");
