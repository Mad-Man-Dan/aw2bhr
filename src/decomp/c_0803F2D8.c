#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F2D8.
 * sub_0803F2D8 @ 0x0803F2D8
 */

#include "proc.h"
/* The proc the 0x0849F918 script drives; sub_0803F3E4 fills +0x2c/+0x30 in and
 * sub_0803F38C walks the +0x4c script cursor. */
struct UnkF2D8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4c);
    /* 4c */ u16 *unk4c;
};

/* Both fields are full words (`ldr`); the `lsls #0x10; asrs #0x10` on each is
 * the implicit conversion to ScrollCameraToCenterCell's two `s16` parameters, not a cast
 * in the source. */
void sub_0803F2D8(struct UnkF2D8Proc *proc)
{
    ScrollCameraToCenterCell(proc->unk2c + 1, proc->unk30 + 2);
}
