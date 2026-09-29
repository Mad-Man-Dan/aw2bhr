#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B5B8.
 * sub_0808B5B8 @ 0x0808B5B8
 */

#include "hardware.h"

/* Program a whole save slot: walk sub_0808B540 across as many sectors as the
 * slot is bytes long, retrying each sector once. The 64-byte stack local is
 * the relocated read routine SetReadFlash1 copies out of ROM.
 *
 * MATCHED in wave 79 (W79-D) after thirty waves parked. NEEDS THE FLASH BLOCK'S
 * OVERRIDE -- -O2 AND -fforce-addr removed, -O1 added -- now recorded in
 * data/compiler-overrides.json. The entry had been missing since wave 47 even
 * though the parked note asserted it was there, and every measurement taken on
 * this function used --profile o1, which removes -O2 but KEEPS -fforce-addr.
 * That is not the configuration its thirteen neighbours use. Under the real one
 * the same source went 40.1% -> 99.0%, and the residual four waves had been
 * attacking -- agbcc duplicating the outer loop's exit test into a guard plus a
 * do/while, where the ROM jumps into the loop -- does not exist here at all. It
 * was an artefact of -fforce-addr.
 *
 * THE RETRY LOOP IS STILL ENTERED BY A JUMP INTO ITS BODY, past the countdown,
 * and is spelled with a goto for that reason; that lever was worth 12 bytes in
 * wave 47 and is kept.
 *
 * THE COMMA IN THE gUnknown_03005C7C STORE IS LOAD-BEARING. The ROM's two pool
 * words are gUnknown_03005C7C first and gUnknown_08485678 second, i.e. the
 * store's DESTINATION address is created before the source's. gcc's
 * expand_assignment expands the LHS before it calls store_expr on the RHS, so
 * binding slotInfo inside the RHS puts the destination first; as its own
 * preceding statement it puts the source first and swaps the pool pair. That
 * was the last 2 of 192 bytes. Do not tidy it into two statements.
 *
 * gUnknown_08485678 +0x18 is a 32-bit SLOT size and gUnknown_084856A4 +0x18 a
 * 32-bit SECTOR size -- the same offset in two records 0x2C apart, not one
 * record read twice. One local const u8 * per site: a single shared local gives
 * one long-lived pseudo where the ROM rematerialises the address per statement.
 */
u16 ProgramFlashSector_AT(u16 sectorNum, const u8 *src)
{
    u16 buf[0x20];
    const u8 *slotInfo;
    const u8 *sectorInfo;
    u32 size;
    u16 n;
    u16 tries;
    u16 result;

    if (sectorNum > 0xf)
        return 0x80FF;

    SetReadFlash1(buf);

    REG_WAITCNT = (REG_WAITCNT & ~3) | gUnknown_084856A4[0x12];

    n = sectorNum << 5;

    gUnknown_03005C7C = (slotInfo = (const u8 *)gUnknown_08485678,
                         *(const u32 *)(slotInfo + 0x18));

    while (gUnknown_03005C7C != 0)
    {
        tries = 2;

        goto program;

        for (;;)
        {
            tries--;

            if (tries == 0)
                break;

        program:
            result = sub_0808B540(n, src);

            if (result == 0)
                break;
        }

        if (result != 0)
            break;

        sectorInfo = (const u8 *)gUnknown_084856A4;
        size = *(const u32 *)(sectorInfo + 0x18);

        gUnknown_03005C7C -= size;
        src += size;
        n++;
    }

    REG_WAITCNT = (REG_WAITCNT & ~3) | 3;

    return result;
}
asm(".global sub_0808B5B8\n.thumb_set sub_0808B5B8, ProgramFlashSector_AT\n");
