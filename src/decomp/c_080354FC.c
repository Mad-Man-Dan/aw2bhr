#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080354FC.
 * LoadWeatherData @ 0x080354FC
 */

/* Family F005 (data/families.json): 19 sixteen-byte wrappers shaped
 * `push {lr}; bl S; bl S; pop {r0}; bx r0` -- two calls, nothing between
 * them, `pop {r0}` so void.
 * Two statements and not `g(f())`: the second callee takes no arguments (it
 * opens with a `bl`, a pool `ldr` or a `movs` into r0, never a read of r0),
 * so there is no parameter for the first call's result to reach. `g(f())`
 * would be byte-identical only if `g` had a parameter, and none of the 19
 * second callees does -- see the F005 block in unknown-functions.h. */

/* Named per Xenesis's AW2 Subroutine List: "Loads weather data (OBJ Tile,
 * XY positioning, etc)". The old LoadWeatherData symbol is kept as a linker
 * alias below so every other unit keeps resolving it unchanged. */
void LoadWeatherData(void)
{
    InitSnowParticles();
    InitRainParticles();
}

asm(".global sub_080354FC\n.thumb_set sub_080354FC, LoadWeatherData\n");
