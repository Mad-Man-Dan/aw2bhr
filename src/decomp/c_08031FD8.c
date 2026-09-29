#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031FD8.
 * sub_08031FD8 @ 0x08031FD8
 */

#include "proc.h"

/* Counts the non-empty slots belonging to each army still in play and jumps the
 * proc to label 0 if it found any. gUnknown_02025564.unk20[] is read as four
 * groups of three (`i * 3 + j`), which is where the `* 84` giv comes from; the
 * 0x33 is unk20's 0x20 plus unk13's 0x13, folded. gUnknown_0300449C[i]++ emits
 * a second, entirely dead `ldrb` between the `adds #1` and the `strb` -- the
 * volatile tell, and the same global LinkMapListExchange_Start clears in the same 0..3 army
 * loop. */
void LinkCountSharedMaps(ProcPtr proc)
{
    int i;
    int j;
    int count;

    count = 0;

    for (i = 0; i < 4; i++)
    {
        if (SioIsPlayerLinked(i))
        {
            for (j = 0; j < 3; j++)
            {
                if (gUnknown_02025564.unk20[i * 3 + j].unk13 != 0xff)
                {
                    count++;
                    gUnknown_0300449C[i]++;
                }
            }
        }
    }

    if (count != 0)
        Proc_Goto(proc, 0);
}
asm(".global sub_08031FD8\n.thumb_set sub_08031FD8, LinkCountSharedMaps\n");
