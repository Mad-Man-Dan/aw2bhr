#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044804.
 * sub_08044804 @ 0x08044804
 */

#include "proc.h"

/* The CoPowerSonja shape (src/decomp/c_080447EC.c) with a payment in front:
 * half of the current army's gPlayers record's first word goes to
 * AddPlayerFunds, then the 0x1F6 message, then the child is started under the
 * caller's proc. `lsrs r1,r1,#1` and not `asrs` -- unk00 is already declared
 * unsigned. */
void CoPowerColinGoldRush(ProcPtr parent)
{
    AddPlayerFunds(gUnknown_030033EC, gPlayers[gUnknown_030033EC].funds >> 1);
    PlayMusicOrSfx2(0x1F6);
    StartCoPowerWhiteFlash(parent);
}
asm(".global sub_08044804\n.thumb_set sub_08044804, CoPowerColinGoldRush\n");
