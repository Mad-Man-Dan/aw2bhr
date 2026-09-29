#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802B4D4.
 * sub_0802B4D4 @ 0x0802B4D4
 */

#include "proc.h"

void DrawArmyCaptureCounters(s16 a1, s16 a2, s16 a3)
{
    s8 i;
    s8 acc;

    if (Proc_Find(gUnknown_0849A02C) != 0)
        return;

    if (gPlaySt.captureLimit != 0)
    {
        acc = 7;

        for (i = 4; i > 0; i--)
        {
            if (IsPlayerAliveAndActive(i))
            {
                PutOamHi((a1 + gUnknown_0849A2A6[a3 * 3] + 4) & 0x1FF,
                             (a2 - acc) & 0xFF,
                             gUnknown_0848B688,
                             ((i + 8) << 12) | 0x361);
                DrawInfoPanelTwoDigitNumber(a1 + gUnknown_0849A2A6[a3 * 3] + 0x15,
                             a2 - acc,
                             gPlaySt.captureLimit - gPlayers[i].captures);
                acc += 7;
            }
        }

        PutOamHi((a1 + gUnknown_0849A2A6[a3 * 3] + 1) & 0x1FF,
                     (a2 - acc) & 0xFF,
                     gUnknown_0848B6B0,
                     0x1362);
    }
    else if (gPlaySt.turnLimit != 0)
    {
        acc = 7;

        for (i = 4; i > 0; i--)
        {
            if (IsPlayerAliveAndActive(i))
            {
                PutOamHi((a1 + gUnknown_0849A2A6[a3 * 3] + 4) & 0x1FF,
                             (a2 - acc) & 0xFF,
                             gUnknown_0848B688,
                             ((i + 8) << 12) | 0x361);
                DrawInfoPanelTwoDigitNumber(a1 + gUnknown_0849A2A6[a3 * 3] + 0x15,
                             a2 - acc,
                             gPlayers[i].captures);
                acc += 7;
            }
        }
    }
}
asm(".global sub_0802B4D4\n.thumb_set sub_0802B4D4, DrawArmyCaptureCounters\n");
