#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AB8C.
 * sub_0808AB8C @ 0x0808AB8C
 */

#include "hardware.h"

/* PARKED at 55.4%, -12 bytes (wave 46, W46-I). Was 5.4%.
 *
 * Scans the flash chip descriptor table for the id ReadFlashId returned and
 * installs the matching record's five routine pointers. Returns 1 when the scan
 * ran off the end of the table (no chip matched) and 0 on a match.
 *
 * The scan is a GOTO loop. agbcc rotates every `for`/`while`, copying the exit
 * test to the top as an entry guard and leaving a second copy at the bottom;
 * the ROM reaches its test through a `b` INTO the loop and has exactly one
 * copy. Eight top-tested spellings were swept and all eight rotate -- see the
 * rotation chapter in docs/agbcc-codegen.md. That respelling alone is worth
 * 5.4% -> 55.4% and makes the whole loop region byte-exact, including the
 * `adds r0, r1, #0; adds r0, #0x28; ldrb` address build that the union member
 * read produces.
 *
 * The `(struct Unk03005C78 *)` cast is required, not cosmetic: unk14 is
 * declared `u8 unk14[0x14]` deliberately (see the note on struct Unk0848548C in
 * include/unknown-globals.h -- embedding the struct moves unk28 to +0x2C). The
 * previous draft assigned `&(*p)->unk14` without it and no longer compiled at
 * all under -Werror.
 *
 * REMAINING DIFF, all twelve bytes one fact: the ROM re-reads `*cursor` from
 * memory before each of the six tail stores --
 *
 *     ldr r1, =gUnknown_03005C74
 *     ldr r0, [r2]          @ *cursor, re-read every time
 *     ldr r0, [r0]          @ ->unk00
 *     str r0, [r1]
 *
 * -- where this candidate keeps the record pointer the loop already loaded and
 * uses displacements off it (`ldr r0, [r2, #N]`). Six loads x 2 bytes is exactly
 * the deficit, and every other byte of the tail agrees.
 *
 * RULED OUT, measured, do not repeat:
 *  - The toolchain. See the 0x0808A chapter in docs/agbcc-codegen.md: old_agbcc
 *    changes nothing anywhere in this block that helps, and this function's
 *    neighbours are byte-identical under it.
 *  - "A store to any global invalidates every memory value CSE is holding"
 *    (wave 33, W33-G) does NOT reach this: agbcc held the record pointer in a
 *    register across all six stores to neighbouring globals. That rule is about
 *    a value in MEMORY, not one already in a register.
 *
 * What would unblock it is a spelling that makes the loop's dereference die at
 * the loop exit so the tail has to load `*cursor` fresh. Writing the six tail
 * statements as `(*cursor)->unkNN` -- which is what this draft does -- is not
 * enough, because the loop has already put `*cursor` in a live pseudo. */
bool8 IdentifyFlash(void)
{
    u16 flashId;
    struct Unk0848548C **cursor;
    bool8 result;

    REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | 3;

    flashId = ReadFlashId();

    cursor = gUnknown_0848548C;
    result = TRUE;
    goto test;
next:
    cursor++;
test:
    if ((*cursor)->unk28.separate.makerId == 0)
        goto done;
    if (flashId != (*cursor)->unk28.joined)
        goto next;
    result = FALSE;
done:

    gUnknown_03005C74 = (*cursor)->unk00;
    gUnknown_03005C80 = (*cursor)->unk04;
    gUnknown_03005C84 = (*cursor)->unk08;
    gUnknown_03005C70 = (*cursor)->unk0c;
    gUnknown_03000F68 = (*cursor)->unk10;
    gUnknown_03005C78 = (struct Unk03005C78 *)(*cursor)->unk14;

    return result;
}
asm(".global sub_0808AB8C\n.thumb_set sub_0808AB8C, IdentifyFlash\n");
