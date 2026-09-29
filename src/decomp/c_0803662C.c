#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803662C.
 * sub_0803662C @ 0x0803662C
 */

void InstallMapFrameCallbacks(void)
{
    sub_0801F00C();
    SetMapLayersDefault();
    sub_08011B18();
    sub_08011B34((void *)sub_08022048);
    sub_08011B34((void *)UpdateTerrainAnimation);
    sub_08011B34((void *)UpdateFuelAmmoGraphics);
    sub_08011B34((void *)UpdateWeatherParticles);
    sub_08011B34((void *)sub_080246B4);
    sub_08011B34((void *)sub_08024720);
    sub_08011B34((void *)sub_08022A6C);
    sub_08011B34((void *)sub_08043590);
    sub_080366D0(MapVBlankCallback);
    sub_080366C4(MapMainLoopCallback);
}
asm(".global sub_0803662C\n.thumb_set sub_0803662C, InstallMapFrameCallbacks\n");
