#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011550.
 * sub_08011550 @ 0x08011550, sub_0801156C @ 0x0801156C
 */

#include "proc.h"

/* Only the halfword at +0x64 is ever touched here; the rest of the proc is
 * whatever struct Proc already describes. */
struct Unk11550Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
#include "proc.h"

struct Unk1156CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

void StartFadeToBlack(int a)
{
    struct Unk11550Proc *proc = Proc_Start(gUnknown_0848923C, PROC_TREE_3);

    proc->unk64 = a;
}
asm(".global sub_08011550\n.thumb_set sub_08011550, StartFadeToBlack\n");

void StartFadeFromBlack(int a)
{
    struct Unk1156CProc *proc = Proc_Start(ProcScr_DesignRoomFadeIn, PROC_TREE_3);

    proc->unk64 = a;
}
asm(".global sub_0801156C\n.thumb_set sub_0801156C, StartFadeFromBlack\n");
