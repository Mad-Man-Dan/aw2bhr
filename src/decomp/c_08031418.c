#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031418.
 * sub_08031418 @ 0x08031418
 */

#include "proc.h"
/* r1 is loaded from r0, so the parent is this wrapper's only parameter. */
struct Unk31418Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ u32 unk58;
};

void StartLinkLobbySlots(ProcPtr parent)
{
    ((struct Unk31418Proc *)Proc_Start(gUnknown_0849B284, parent))->unk58 = 0;
}
asm(".global sub_08031418\n.thumb_set sub_08031418, StartLinkLobbySlots\n");
