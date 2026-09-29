#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BFFC.
 * sub_0801BFFC @ 0x0801BFFC
 */

#include "proc.h"
/* Stores two words into a proc, looking it up when the caller passes NULL.
 * Both offsets are past PROC_HEADER's 0x29 bytes, so the object really is a
 * Proc and the header spelling compiles. */
struct Unk1BFFCProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ u32 unk2c;
    /* 30 */ u32 unk30;
};

void MoveSpriteRefresher(struct Unk1BFFCProc *proc, u32 a, u32 b)
{
    if (proc == NULL)
        proc = Proc_Find(gUnknown_0848B418);

    proc->unk2c = a;
    proc->unk30 = b;
}
asm(".global sub_0801BFFC\n.thumb_set sub_0801BFFC, MoveSpriteRefresher\n");
