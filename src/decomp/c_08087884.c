#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087884.
 * sub_08087884 @ 0x08087884
 */

#include "proc.h"
struct Unk8087884Proc
{
    u8 filler_00[0x54];
    int unk54;
};

void StartEnemyCoMinimugs(int a, ProcPtr parent)
{
    ((struct Unk8087884Proc *)Proc_Start(ProcScr_PutEnemyCoMinimug, parent))->unk54 = a;
}
asm(".global sub_08087884\n.thumb_set sub_08087884, StartEnemyCoMinimugs\n");
