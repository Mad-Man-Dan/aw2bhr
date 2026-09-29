#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080452C0.
 * sub_080452C0 @ 0x080452C0
 */

#include "proc.h"
/* Only +0x2c, +0x3c and +0x40 are touched here; the rest of the proc is
 * whatever struct Proc already describes. The `adds r0, #0x2c` before the
 * `strb` is the THUMB displacement limit (0-31 for strb), not address-taking. */
struct Unk452C0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ u8 unk2c;
    /* 2d */ STRUCT_PAD(0x2d, 0x3c);
    /* 3c */ int unk3c;
    /* 40 */ int unk40;
};

void StartCoPowerUnitSparkle(int a, int b, int c)
{
    struct Unk452C0Proc *proc = Proc_Start(gUnknown_084A096C, PROC_TREE_3);

    proc->unk3c = a;
    proc->unk40 = b;
    proc->unk2c = c;
}
asm(".global sub_080452C0\n.thumb_set sub_080452C0, StartCoPowerUnitSparkle\n");
