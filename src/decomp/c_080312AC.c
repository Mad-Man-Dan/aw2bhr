#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080312AC.
 * sub_080312AC @ 0x080312AC
 */

#include "hardware.h"
#include "proc.h"
/* Wave 62 (W62-A): MATCHED under the configured profile.  An explicit byte
 * offset for the palette walk takes r8 and makes gUnknown_0849B018 rematerialize
 * at both reads.  Its source order is significant: initialize it after the
 * x % 30 pointer computation, then advance it by 0x20 at the loop bottom. */

struct Unk080312ACProc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void LinkLobbySlots_Loop(struct Unk080312ACProc *proc)
{
    int n;
    int i;
    u16 x;
    u8 *p;
    int palOffset;

    n = 0;

    x = gGameClock & 0x3f;

    if ((s16)gUnknown_0849B060->unk02 == 3)
        return;

    i = 0;
    p = &gUnknown_0849B0C4[x % 30];
    palOffset = 0x31c;

    for (; i <= 3; i++)
    {
        if (!SioIsPlayerLinked(i))
            goto done;

        PutSpriteExt(0, (0x50 + i * 0x28) & 0x1ff, 0x40, gUnknown_0849B258[i],
                     (i + 8) << 12);
        n++;

        if (i == gUnknown_0849B018->unk06)
        {
            DrawOamObject(gUnknown_0849B27C[i], 0x58 + i * 0x28, 0x58 - *p, 0, 0);
            *(u16 *)((u8 *)gPal + palOffset) = gUnknown_081D3E68[(x >> 1) & 0xf];
            EnablePaletteSync();
        }

        palOffset += 0x20;
    }

done:

    if (n != 0)
        PutSpriteExt(0, 0x58, 0x38, gUnknown_0849B268[n], 0x8000);

    if (gUnknown_0849B018->unk06 != 0)
        return;

    if (proc->unk58 == 0)
    {
        if (SioAreAllLinkedPlayersStatus5())
        {
            proc->unk58 = 1;
            Proc_Start(gUnknown_0849B1A0, proc);
        }

        if (proc->unk58 == 0)
            return;
    }

    if (SioAreAllLinkedPlayersStatus5() == 0)
    {
        proc->unk58 = 0;
        Proc_EndEach(gUnknown_0849B1A0);
    }
}
asm(".global sub_080312AC\n.thumb_set sub_080312AC, LinkLobbySlots_Loop\n");
