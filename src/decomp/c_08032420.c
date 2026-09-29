#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032420.
 * sub_08032420 @ 0x08032420
 */

#include "proc.h"

/* `lsls #2; adds; lsls #0x13; asrs #0x10` is `x * 40` narrowed to s16: the x5
 * strength reduction with the x8 folded into the s16 cast's left shift. The
 * cast is LinkPlayerCursor_Draw's declared s16 parameters doing the narrowing, not a
 * cast in the source. */

void LinkPlayerCursor_Loop(ProcPtr proc)
{
    LinkPlayerCursor_HandleInput();

    LinkPlayerCursor_Draw(proc,
        gUnknown_0849B060->unk04 * 40,
        gUnknown_0849B060->unk06 * 40);
}
asm(".global sub_08032420\n.thumb_set sub_08032420, LinkPlayerCursor_Loop\n");
