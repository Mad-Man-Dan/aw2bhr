#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080402B4.
 * DestroyPipeSeam @ 0x080402B4, StartSiloLaunch @ 0x08040380
 */

#include "proc.h"
/* Reports the cell to StartExplosionEffect and then, for exactly two tile ids, rewrites
 * both map planes at that cell. The two `if`s are separate statements and not
 * an `else if` chain -- the ROM re-tests the snapshot after the first block
 * falls through rather than branching past it.
 *
 * The snapshot is taken BEFORE the StartExplosionEffect call and held in a
 * callee-saved register: gUnknown_08499590 is not const, so every store and
 * every call kills it, which is why the ROM reloads the base
 * (`ldr r1,[r6]`) for each of the four subsequent plane accesses while keeping
 * `y * 2`, 0x417A and 0xA22 live in r5, r8 and sb. That is the
 * `mov r7,sb; mov r6,r8` five-plus-live-values shape, not a loop.
 *
 * Plane addresses go through a struct laid over gUnknown_08499590 for the usual
 * reason: the ROM computes `(map + K) + idx`, which only a COMPONENT_REF
 * preserves. */
/* The sibling of DestroyPipeSeam next door: same snapshot-then-rewrite shape on
 * the two map planes, but keyed on tile 0x180 and driving the 0x0849FB8C
 * loader pair (LoadSiloMissileGraphics / StartSiloMissileLaunch) instead of StartExplosionEffect.
 *
 * 0x1CA is written twice and CSEd into r4 across both calls. The snapshot is
 * taken BEFORE them because gUnknown_08499590 is not const and every call kills
 * it -- which is also why the base is reloaded for each plane access afterwards
 * while `y * 2` and the plane constants stay live in sb/r8.
 *
 * Plane addresses go through a struct laid over gUnknown_08499590: the ROM
 * computes `(map + K) + idx`, which only a COMPONENT_REF preserves. */
/* Named per Xenesis's AW2 Subroutine List: "Subroutine that changes Tile
 * Type Map (0201F882) after a pipe seam is destroyed. Input: r0 = x-coord,
 * r1 = y-coord". The old DestroyPipeSeam symbol is kept as a linker alias
 * below so every other unit keeps resolving it unchanged. */
void DestroyPipeSeam(int x, int y, ProcPtr parent)
{
    u16 v = gMap->tile[
        gMap->rowOffset[y] + x];

    StartExplosionEffect(x, y, -3, parent);

    if (v == 0x162)
    {
        gMap->terrain[
            gMap->rowOffset[y] + x] = 1;
        gMap->tile[
            gMap->rowOffset[y] + x] = 0x122;
    }

    if (v == 0x163)
    {
        gMap->terrain[
            gMap->rowOffset[y] + x] = 1;
        gMap->tile[
            gMap->rowOffset[y] + x] = 0x123;
    }

    RebuildMapUnitLayers2();
    RecountArmyProperties();
}

asm(".global sub_080402B4\n.thumb_set sub_080402B4, DestroyPipeSeam\n");

void StartSiloLaunch(int x, int y, ProcPtr parent)
{
    u16 v = gMap->tile[
        gMap->rowOffset[y] + x];

    LoadSiloMissileGraphics(0x1CA, 5);
    StartSiloMissileLaunch(x, y, 0x1CA, 5, parent);

    if (v == 0x180)
    {
        gMap->terrain[
            gMap->rowOffset[y] + x] = 0x12;
        gMap->tile[
            gMap->rowOffset[y] + x] = 0x1A0;
    }

    RefreshPropertyTerrainBytes();
    RebuildMapUnitLayers2();
}
asm(".global sub_08040380\n.thumb_set sub_08040380, StartSiloLaunch\n");
