#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039EC8.
 * sub_08039EC8 @ 0x08039EC8
 */

#include "proc.h"
/* Re-places the proc's sprite at its tile coordinates converted to pixels and
 * offset by the camera origin, and breaks the proc when the placement is
 * refused. The two masks differ (0x1ff against 0xff) because the x wraps on a
 * 512-pixel map row and the y on a 256-pixel column.
 *
 * gUnknown_08499590 is dereferenced ONCE here, unlike src/decomp/c_08039140.c
 * where the two reads sit in different branches: both `ldrsh`es are register
 * offsets off one loaded base because they are in a single expression. */
struct Unk39EC8Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ STRUCT_PAD(0x2b, 0x34);
    /* 0x34 */ struct Unk0801C210 *unk34;
};

void SparkleEffect_UpdateApAnim(struct Unk39EC8Proc *proc)
{
    if (AP_Update(proc->unk34,
                     ((proc->unk29 << 4) - gMap->scrollX + 8) & 0x1ff,
                     ((proc->unk2a << 4) - gMap->scrollY + 8) & 0xff) == 0)
        Proc_Break(proc);
}
asm(".global sub_08039EC8\n.thumb_set sub_08039EC8, SparkleEffect_UpdateApAnim\n");
