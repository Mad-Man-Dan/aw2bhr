#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08046644.
 * sub_08046644 @ 0x08046644
 */

/* Five byte fields cleared or seeded, then a fixed five-call rebuild.
 *
 * The global pointer is re-loaded before every single store: each `strb`
 * through it may alias the pointer itself, so agbcc cannot keep it live. That
 * reload is in the ROM and is what five plain statements produce -- binding it
 * to a local first would remove all four extra `ldr`s.
 *
 * Every field needs `adds r0, #0xNN` rather than a displacement; strb's imm5
 * stops at 31 and all five offsets are past it. */
void IntelStatus_Init(void)
{
    gUnknown_084C1430->unk50 = 0;
    gUnknown_084C1430->unk53 = 0;
    gUnknown_084C1430->unk54 = 0;
    gUnknown_084C1430->unk5e = gUnknown_030033EC;
    gUnknown_084C1430->unk59 = 0;
    ClearBg0Tilemap();
    ClearBg2Tilemap();
    sub_08046030();
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
}
asm(".global sub_08046644\n.thumb_set sub_08046644, IntelStatus_Init\n");
