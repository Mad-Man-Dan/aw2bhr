#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803662C.
 * sub_0803662C @ 0x0803662C
 */

void InstallMapFrameCallbacks(void)
{
    EnableSpriteLayerMode();
    SetMapLayersDefault();
    sub_08011B18();
    AddVBlankHook((void *)UpdateUnitSheetAnimation);
    AddVBlankHook((void *)UpdateTerrainAnimation);
    AddVBlankHook((void *)UpdateFuelAmmoGraphics);
    AddVBlankHook((void *)UpdateWeatherParticles);
    AddVBlankHook((void *)sub_080246B4);
    AddVBlankHook((void *)AnimatePowerActiveCoPalettes);
    AddVBlankHook((void *)AnimateCursorPalette);
    AddVBlankHook((void *)AnimateCoPowerStatusPalette);
    sub_080366D0(MapVBlankCallback);
    sub_080366C4(MapMainLoopCallback);
}
asm(".global sub_0803662C\n.thumb_set sub_0803662C, InstallMapFrameCallbacks\n");
