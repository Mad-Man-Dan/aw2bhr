#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078018.
 * sub_08078018 @ 0x08078018, sub_08078038 @ 0x08078038, sub_08078078 @ 0x08078078, sub_080780A0 @ 0x080780A0
 */

#include "proc.h"

/* One of the 0x08078 block's mode-teardown sequences: four `void (void)`
 * leaves in a row and then the family-F000 `Proc_EndEach(script)` tail that
 * src/decomp/c_08078240.c documents. Every `bl` here is a bare statement --
 * `pop {r0}` fixes this function as void and none of the four callees leaves
 * anything in r0 that is re-narrowed, so nothing is nested or forwarded. */

void EndAllWorldMapOverlayProcs(void)
{
    EndWorldMapMarkerDrawer();
    EndWorldMapScope();
    EndDifficultyStars();
    EndWorldMapSelectionFrame();
    Proc_EndEach(gUnknown_086143E0);
}
asm(".global sub_08078018\n.thumb_set sub_08078018, EndAllWorldMapOverlayProcs\n");

/* Two CpuFastSet FILL calls (0x01000200 = the fill bit plus a 0x200-word
 * count) clearing the two tilemap buffers gBG0TilemapBuffer and
 * gBG2TilemapBuffer point at, then two `void (void)` calls.
 *
 * The two zero sources are separate stack slots and the SECOND assignment is
 * written AFTER the first call, not beside the first assignment. That is
 * readable off the ROM rather than guessed: `movs r4, #0` parks the zero in a
 * callee-saved register and `str r4, [sp, #4]` lands between the two
 * CpuFastSets. Declaring both `= 0` together instead is 4 bytes short -- the
 * zero's live range no longer crosses a call, so it stays in r0, r4 goes to
 * 0x01000200 and the function pushes {r4, lr} where the ROM pushes
 * {r4, r5, lr}. Same mechanism as wave 23's "one binding local too few", read
 * in the other direction. */

void WorldMap_ClearBg0Bg2Tilemaps(void)
{
    u32 fill1;
    u32 fill2;

    fill1 = 0;
    CpuFastSet(&fill1, gBG0TilemapBuffer, 0x01000200);
    fill2 = 0;
    CpuFastSet(&fill2, gBG2TilemapBuffer, 0x01000200);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
}
asm(".global sub_08078038\n.thumb_set sub_08078038, WorldMap_ClearBg0Bg2Tilemaps\n");

/* The one-buffer version of WorldMap_ClearBg0Bg2Tilemaps: a single CpuFastSet FILL of the
 * gBG1TilemapBuffer tilemap buffer and one `void (void)` call. With only one
 * store the zero never crosses a call, so it stays in r0 and 0x01000200 is
 * loaded straight into r2 -- the plain `= 0` initialiser is right here and is
 * wrong in WorldMap_ClearBg0Bg2Tilemaps, which is why the two are written differently. */

void WorldMap_ClearBg1Tilemap(void)
{
    u32 fill = 0;

    CpuFastSet(&fill, gBG1TilemapBuffer, 0x01000200);
    BG_EnableSyncBG1();
}
asm(".global sub_08078078\n.thumb_set sub_08078078, WorldMap_ClearBg1Tilemap\n");

/* A proc method. GetMainMenuLock returns u8 (promoted in src/decomp/c_0803BD54.c)
 * and the ROM re-narrows its result `lsls #0x18; lsrs #0x18` before comparing
 * against 1, which is the caller-side corroboration of that return type.
 *
 * `movs r0, #0` / `movs r0, #1` split across a `b` would be a returned
 * comparison; here the unconditional `b` jumps over the else arm of a plain
 * `if`, and the function is void (`pop {r0}`). */

void WorldMap_ResumeUnlessMenuLocked(ProcPtr proc)
{
    if (GetMainMenuLock() == 1)
    {
        Proc_Goto(proc, 3);
    }
    else
    {
        AP_ClearAll();
        gUnknown_0202FDFC.unk10 = 0;
    }
}
asm(".global sub_080780A0\n.thumb_set sub_080780A0, WorldMap_ResumeUnlessMenuLocked\n");
