#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080DFC.
 * sub_08080DFC @ 0x08080DFC, sub_08080E40 @ 0x08080E40, StartCoPowerScript @ 0x08080E74
 */

#include "hardware.h"
#include "proc.h"

void SuperCoPowerBg_Loop(void)
{
    if (gUnknown_03002020 <= 7 && DivRem(gUnknown_03001FF8, 0x40) == 0)
    {
        gUnknown_03002020++;
        gUnknown_03002B28--;
    }

    gUnknown_03001FF8 += 0x18;
}
asm(".global sub_08080DFC\n.thumb_set sub_08080DFC, SuperCoPowerBg_Loop\n");

void StartSuperCoPowerWhiteFade(ProcPtr proc)
{
    StartPalFadeToWhite(0, 0x10, proc);
    StartPalFadeToWhite(8, 0x10, proc);
    StartPalFadeToWhite(0x10, 0x10, proc);
    StartPalFadeToWhite(0x11, 0x10, proc);
}
asm(".global sub_08080E40\n.thumb_set sub_08080E40, StartSuperCoPowerWhiteFade\n");

void StartCoPowerScript(int a, int b, ProcPtr proc)
{
    gUnknown_03005970 = a;
    gUnknown_03005904 = b;

    if (b == 1)
        Proc_StartBlocking(gUnknown_08616794, proc);

    if (b == 2)
        Proc_StartBlocking(gUnknown_08616844, proc);
}

asm(".global sub_08080E74\n.thumb_set sub_08080E74, StartCoPowerScript\n");
