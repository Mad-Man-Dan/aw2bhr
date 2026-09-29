#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803388C.
 * sub_0803388C @ 0x0803388C
 */

#include "proc.h"
/* Starts the gUnknown_0849BB50 proc unless one is already running, and seeds it
 * with the same `a * 32 + 0x2e` that the promoted LinkC4_SetCursorRow writes to +0x38
 * of the very same proc -- so the two agree on both the expression and the
 * field.
 *
 * The Proc_Find result is STORED at +0x58, not merely tested: the ROM's
 * `str r5, [r0, #0x58]` reuses the register holding it rather than
 * materialising a zero, and a literal `= 0` compiles to `movs r0,#0; str` and
 * one fewer pushed register. It is null on this path by construction.
 *
 * The script is bound to a NAMED local, and that is what fixes the register
 * allocation: declared this way the address takes r4 and the Proc_Find result
 * r5, which is the ROM's assignment. With the address left as an -fforce-addr
 * temporary the two swap and every instruction that names them differs. */
struct Unk3388CProc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x04];
    /* 38 */ int unk38;
    /* 3c */ u8 filler_3c[0x1c];
    /* 58 */ ProcPtr unk58;
};

void LinkC4_StartCursorProc(int a, ProcPtr parent)
{
    const struct ProcCmd *script = gUnknown_0849BB50;
    ProcPtr found = Proc_Find(script);
    struct Unk3388CProc *proc;

    if (found == NULL)
    {
        proc = Proc_Start(script, parent);
        proc->unk2c = 0x58;
        proc->unk30 = proc->unk38 = a * 32 + 0x2e;
        proc->unk58 = found;
    }
}
asm(".global sub_0803388C\n.thumb_set sub_0803388C, LinkC4_StartCursorProc\n");
