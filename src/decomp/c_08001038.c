#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08001038.
 * sub_08001038 @ 0x08001038
 */

#include "proc.h"

/* Raise a flag, start a proc, and stash the argument in the entry it hands
 * back. The strh into ->unk1e is Unk03001470's own s16 member, so the int
 * parameter (which the existing declaration fixes) is narrowed there and
 * nowhere else. */
void StartDesignRoom(int a)
{
    gUnknown_030040A0 = 1;
    sub_080152EC(gUnknown_084858DC, 0)->unk1e = a;
}
asm(".global sub_08001038\n.thumb_set sub_08001038, StartDesignRoom\n");
