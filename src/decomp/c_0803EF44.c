#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803EF44.
 * sub_0803EF44 @ 0x0803EF44
 */

#include "proc.h"
struct Proc3EF44
{
    u8 filler_00[0x2c];
    int unk_2c;
    int unk_30;
};

void StartLaserFire(int a1, int a2, ProcPtr parent)
{
    struct Proc3EF44 *proc = Proc_StartBlocking(gUnknown_0849F7F0, parent);

    proc->unk_2c = a1;
    proc->unk_30 = a2;
    ScrollCameraToCenterCell(a1, a2);
}
asm(".global sub_0803EF44\n.thumb_set sub_0803EF44, StartLaserFire\n");
