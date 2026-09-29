#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802723C.
 * sub_0802723C @ 0x0802723C, sub_08027278 @ 0x08027278
 */

#include "proc.h"
/* Not the Proc header's own fields: +0x2c, +0x30 and +0x54 are all past
 * PROC_HEADER's 0x29 bytes, so the payload is this proc's own state.
 * Both arguments are saved across the `bl` with a bare `adds rN, r0, #0` and
 * no narrowing, which is PROMOTE_MODE saying `int` -- a u16 would carry
 * `lsls #16; lsrs #16` whatever the store width.
 */
struct Unk2723CProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ s32 unk2c;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ STRUCT_PAD(0x34, 0x54);
    /* 0x54 */ s32 unk54;
};
/* Same body as StartSupplyIconEffect two functions above; only the script and the
 * constant stashed at +0x54 differ. */
struct Unk27278Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ s32 unk2c;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ STRUCT_PAD(0x34, 0x54);
    /* 0x54 */ s32 unk54;
};

void StartSupplyIconEffect(int x, int y)
{
    struct Unk2723CProc *proc = Proc_Start(gUnknown_08499D2C, PROC_TREE_3);

    proc->unk2c = (x << 4) - gMap->scrollX - 0x18;
    proc->unk30 = (y << 4) - gMap->scrollY;
    proc->unk54 = 2;
}
asm(".global sub_0802723C\n.thumb_set sub_0802723C, StartSupplyIconEffect\n");

void sub_08027278(int x, int y)
{
    struct Unk27278Proc *proc = Proc_Start(gUnknown_08499CFC, PROC_TREE_3);

    proc->unk2c = (x << 4) - gMap->scrollX - 0x18;
    proc->unk30 = (y << 4) - gMap->scrollY;
    proc->unk54 = 0;
}
