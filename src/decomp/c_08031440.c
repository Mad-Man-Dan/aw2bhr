#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031440.
 * sub_08031440 @ 0x08031440
 */

#include "hardware.h"
#include "proc.h"

/* The idle step of the gUnknown_0849B048 script: restart it if its slot is
 * gone, quit on B, and in either case advance gUnknown_0849B060->unk00 through
 * LinkScreenSetMessage. Neither `if` is exclusive -- both fall through to the
 * round-trip, which is why the ROM has no branch over it. */
void LinkLobby_WaitForConnection(ProcPtr proc)
{
    if (FindSlotScript((s32)gUnknown_0849B048) == -1)
        Proc_Goto(proc, 0);

    if (gpKeySt->pressed & 2)
    {
        PlayMusicOrSfx2(0x66);
        sub_0801537C(gUnknown_0849B048);
        Proc_Break(proc);
    }

    gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 0, 2);
}
asm(".global sub_08031440\n.thumb_set sub_08031440, LinkLobby_WaitForConnection\n");
