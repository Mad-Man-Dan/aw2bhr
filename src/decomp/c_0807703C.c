#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807703C.
 * sub_0807703C @ 0x0807703C
 */

#include "proc.h"
#include "hardware.h"

/* Wave 35 (W35-B). The map-cursor frame body. gUnknown_081CC598 and
 * gUnknown_081CC59C are NOT globals -- they are `-fforce-addr` address-constant
 * words holding &gpKeySt and &gUnknown_0202FDFC respectively; see the note
 * beside gUnknown_0202FDFC in unknown-globals.h. That is what the apparent
 * double indirection `ldr r0, =sym; ldr r0, [r0]; ldrh` is.
 *
 * unk08 is an idle counter: it advances while no direction key is held and is
 * pinned at 4 once the idle handler has fired. `unk10 = 0` and `unk0c = -1`
 * share one constant (`movs r0, #0` then `subs r0, #1`). */
void WorldMapCursor_Loop(ProcPtr proc)
{
    u16 k = gpKeySt->held;

    UpdateWorldMapCursor(k);

    if ((k & 0xF0) == 0)
        gUnknown_0202FDFC.unk08++;
    else
        gUnknown_0202FDFC.unk08 = 0;

    if (gUnknown_0202FDFC.unk08 > 3)
    {
        SnapWorldMapCursorToMarker(proc);
        gUnknown_0202FDFC.unk08 = 4;
    }
    else if (gUnknown_0202FDFC.unk10 == 1)
    {
        gUnknown_0202FDFC.unk10 = 0;
        gUnknown_0202FDFC.unk0c = -1;
        MoveWorldMapSelectionFrame(0x20, 0x20, 3);
        EndDifficultyStars();
    }

    if ((gpKeySt->pressed & 1) && gUnknown_0202FDFC.unk10 != 0)
    {
        SetHeaderBannerTransitionFrames(5);
        SetDifficultyStarsPalette(2);
        SetWorldMapScopePalette(2);
        EndWorldMapNationPanel();
        PlayMusicOrSfx2(0x71);
        Proc_Break(proc);
    }
    else if (gpKeySt->pressed & 2)
    {
        PlayMusicOrSfx2(0x66);
        Proc_Goto(proc, 5);
    }
    else
    {
        SetBgScrollShadow(3, gUnknown_0202FDFC.unk00, gUnknown_0202FDFC.unk02);
    }
}
asm(".global sub_0807703C\n.thumb_set sub_0807703C, WorldMapCursor_Loop\n");
