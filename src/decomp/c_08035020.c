#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035020.
 * sub_08035020 @ 0x08035020, sub_0803504C @ 0x0803504C, PlayWeatherChangeSound @ 0x08035064
 */

void ApplyWeatherPalette(u16 a)
{
    LoadMapObjectGraphics(a, 0x48);
    ApplyPaletteExt(gUnknown_0849BD20[a].unk00, 0, 0x100);
}
asm(".global sub_08035020\n.thumb_set sub_08035020, ApplyWeatherPalette\n");

void WeatherChange_CommitWeather(struct Unk03001470 *p)
{
    gPlaySt.weather = p->unk20;
    ApplyWeatherPalette(gPlaySt.weather);
}
asm(".global sub_0803504C\n.thumb_set sub_0803504C, WeatherChange_CommitWeather\n");

/* Named per Xenesis's AW2 Subroutine List: "Code that retrieves the Sound
 * Effects for weather change IDs". The old PlayWeatherChangeSound symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged. */
void PlayWeatherChangeSound(struct Unk03001470 *p)
{
    PlayMusicOrSfx2(gUnknown_0849BD20[p->unk20].unk04);
}

asm(".global sub_08035064\n.thumb_set sub_08035064, PlayWeatherChangeSound\n");
