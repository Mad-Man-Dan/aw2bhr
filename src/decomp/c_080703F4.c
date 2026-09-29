#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080703F4.
 * sub_080703F4 @ 0x080703F4
 */

/* m4aSoundInit. The `& ~1` on sub_0806F7C8's address clears the THUMB bit so
 * the function can be copied as DATA into IWRAM; agbcc materialises -2 as
 * `movs r1,#2; rsbs r1,r1,#0`. CpuSet's third argument is the mode word --
 * 0x100 units with bit 26 (32-bit) set -- not an address.
 *
 * The loop bound is the ABSOLUTE SYMBOL gNumMusicPlayers, not the literal 11;
 * see its comment in unknown-globals.h. It is read AFTER the four setup calls,
 * not in the declaration's initialiser -- reading it up front is size-exact but
 * moves the `lsls`/`lsrs` pair into the prologue and the pool word two slots
 * earlier (measured, 75.0%).
 *
 * Being opaque is also what keeps gUnknown_03005BE0 out of a `.rodata`
 * address-constant word: with a literal bound the loop carries one more live
 * value, LICM hoists the address, and the store becomes a double indirection
 * through r7.
 */
void m4aSoundInit(void)
{
    s32 i;
    u16 n;

    CpuSet((void *)((s32)sub_0806F7C8 & ~1), gUnknown_03000FB0, 0x04000100);
    SoundInit_rev01(&gUnknown_03004790);
    MPlayExtender(gUnknown_030057D0);
    SoundMode_rev01(0x0094DB00);

    n = (u16)(u32)&gNumMusicPlayers;

    for (i = 0; i < n; i++)
    {
        struct MusicPlayerInfo *mplayInfo = gUnknown_08242308[i].info;

        MPlayOpen_rev01(mplayInfo, gUnknown_08242308[i].track, gUnknown_08242308[i].trackCount);
        mplayInfo->unk_0b = gUnknown_08242308[i].unk_0a;
        mplayInfo->memAccArea = &gUnknown_03005BE0;
    }
}
asm(".global sub_080703F4\n.thumb_set sub_080703F4, m4aSoundInit\n");
