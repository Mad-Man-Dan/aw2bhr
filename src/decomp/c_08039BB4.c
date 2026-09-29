#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039BB4.
 * sub_08039BB4 @ 0x08039BB4
 */

#include "proc.h"
/* +0x29 is the first byte past PROC_HEADER. Both byte members need a runtime
 * `adds` because strb's displacement stops at 31; agbcc reuses the first
 * address with `adds r1, #1` rather than recomputing it. */
struct Unk39BB4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ u8 unk29;
    /* 2a */ u8 unk2a;
    /* 2b */ STRUCT_PAD(0x2b, 0x2c);
    /* 2c */ u16 unk2c;
};

/* All three parameters are `int`: each is saved across the `bl` with a bare
 * `adds rN, rM, #0` and no PROMOTE_MODE narrowing, which a u8 or u16 parameter
 * would have carried. */
void StartSparkleZoom(int a, int b, int c)
{
    struct Unk39BB4Proc *proc = Proc_Start(gUnknown_0849D82C, PROC_TREE_3);

    proc->unk29 = a;
    proc->unk2a = b;
    proc->unk2c = c;
}
asm(".global sub_08039BB4\n.thumb_set sub_08039BB4, StartSparkleZoom\n");
