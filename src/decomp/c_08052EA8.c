#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052EA8.
 * sub_08052EA8 @ 0x08052EA8
 */

/* An infinite main loop: the `b` at the end is unconditional and there is no
 * exit path, so the whole tail after sub_080152C0 is a `while (1)` body.
 * sub_080152C0's first parameter is declared s32, so the ProcCmd table needs
 * the usual cast at the call. */
void BattleAnimFrameDriver(void)
{
    sub_0808BBA4();
    ResetDisplayState();
    FlushDisplayRegisters();
    ClearTileRigistry();
    InitSlotScripts();
    sub_080152C0((s32)gUnknown_08553754, 0);

    while (1)
    {
        ClearOamShadow();
        RunAllSlotScripts();
        DrawSpriteScripts();
        FlushTiles();
        FlushDisplayRegisters();
        TickSpriteScripts();
    }
}
asm(".global sub_08052EA8\n.thumb_set sub_08052EA8, BattleAnimFrameDriver\n");
