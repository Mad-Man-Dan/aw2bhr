#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D724.
 * sub_0803D724 @ 0x0803D724, sub_0803D73C @ 0x0803D73C
 */

#include "proc.h"
/* The proc's own two fields; the rest is struct Proc.
 * NOTE for whoever maintains include/unknown-globals.h: the comment on
 * gUnknown_0849F330 credits this Proc_Start to "sub_0803D75C". There is no such
 * function -- 0x0803D75C is a LABEL inside StartSaveScreen, whose extent is
 * 0x0803D73C..0x0803D770. */
struct UnkD73CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ void (*unk4c)(void);
    /* 50 */ STRUCT_PAD(0x50, 0x64);
    /* 64 */ u16 unk64;
};

/* The entry `lsls #0x18; lsrs #0x18` is a declared `u8` parameter; the second
 * pair `lsls #0x18; asrs #0x18` is the conversion to SetSuspendFlag's `s8`
 * first parameter, not a second narrowing of this one. */
void ClearSuspendFlag(u8 a)
{
    if (a != 0)
        SetSuspendFlag(a, 0);
}
asm(".global sub_0803D724\n.thumb_set sub_0803D724, ClearSuspendFlag\n");

void StartSaveScreen(u8 a, void (*f)(void))
{
    struct UnkD73CProc *proc;

    if (a != 0)
        SetSuspendFlag(a, 0);

    proc = Proc_Start(gUnknown_0849F330, PROC_TREE_3);
    proc->unk64 = a;
    proc->unk4c = f;
}
asm(".global sub_0803D73C\n.thumb_set sub_0803D73C, StartSaveScreen\n");
