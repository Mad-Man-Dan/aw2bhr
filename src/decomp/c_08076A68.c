#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076A68.
 * sub_08076A68 @ 0x08076A68
 */

#include "proc.h"

/* PARKED at 69.8%, candidate 112 bytes vs 116 (-4). The STATEMENT STRUCTURE is
 * settled and the whole residual is ONE instruction plus the alignment padding
 * it pulls in. Everything up to and including `bl IsHardCampaignMode` -- every store,
 * both pool-loaded calls, the flag test -- is byte-identical.
 *
 * The ROM has one extra register-to-register COPY at the merge point:
 *     ROM   `adds r4, r7, #0`   (v copied into a second pseudo)
 *           `adds r0, r4, #0`   (argument taken from the copy)
 *           `bl AddWorldMapMarker`
 *           `adds r0, r6, #0` / `adds r0, #0x12` / `adds r4, r4, r0`
 *           `ldrb r0, [r4]` / `movs r1, #1` / `orrs r0, r1` / `strb r0, [r4]`
 *     here  `adds r0, r7, #0`
 *           `bl AddWorldMapMarker`
 *           `adds r0, r6, #0` / `adds r0, #0x12` / `adds r0, r7, r0`
 *           `ldrb r1, [r0]` / `movs r2, #1` / `orrs r1, r2` / `strb r1, [r0]`
 * so the ROM's address lands in the callee-saved r4 and its temporaries stay in
 * r0/r1, while this candidate's address lands in r0 and the temporaries move up
 * to r1/r2. That register choice is a consequence of the missing copy, not a
 * second fact: the ROM's add ties its destination to v's copy, and the copy is
 * what makes r4 available to tie to. -4 bytes = 2 for the copy, 2 for the
 * `.short 0x0000` alignment word it pushes the function over into.
 *
 * The copy is the wave-17 "one local where the original had N" signature read
 * backwards, but the obvious cause is RULED OUT BY PROBE: introducing a second
 * local (`i = v; AddWorldMapMarker(i); ...unk12[i] |= 1;`) produces assembly
 * IDENTICAL to this one -- gcc 2.9 copy-propagates it away. Also ruled out by
 * probe: writing the subscript with v as the FIRST operand
 * (`v[gUnknown_0202FDFC.unk12] |= 1`), which was aimed at the c_0807610C.c
 * operand-tie mechanism and changed nothing at all.
 * What has NOT been tried is anything that keeps both pseudos live across the
 * merge so the copy cannot be coalesced.
 *
 * WAVE 34 (W34-J) RETYPED SetupWorldMapScreen BECAUSE OF THIS FUNCTION. It calls that
 * callee in the MIDDLE of its body, after two other calls have clobbered r0,
 * and the ROM spends `adds r0, r5, #0` putting the proc back -- an instruction
 * a `void(void)` prototype cannot emit. The callee's other two C callers both
 * had their proc already in r0, so the wrong prototype was invisible. See the
 * note in unknown-functions.h; SetupWorldMapForResume and SetupWorldMapAfterMission were re-verified
 * byte-for-byte after the change.
 *
 * Resets gUnknown_0202FDFC's camera block, reloads the shared tile buffer, then
 * marks one of two fixed slots busy depending on IsHardCampaignMode. The `movs r7, #0`
 * at the top is one constant serving both the three `strh` zeroes and v's
 * initialiser, which is ordinary CSE and not a shared local. */
void SetupWorldMapForNewCampaign(ProcPtr proc)
{
    int v = 0;

    gUnknown_0202FDFC.unk00 = 0;
    gUnknown_0202FDFC.unk02 = 0;
    gUnknown_0202FDFC.unk08 = 0;
    gUnknown_0202FDFC.unk0c = -1;
    gUnknown_0202FDFC.unk10 = 0;
    gUnknown_0202FDFC.unk04 = 0x37;
    gUnknown_0202FDFC.unk06 = 0x50;

    Decompress(gUnknown_081D0BAC, gUnknown_08614280);
    RegisterDataMove(gUnknown_08614280, (void *)0x0600F000, 0x1000);
    SetupWorldMapScreen(proc);

    if (IsHardCampaignMode())
        v = 0x22;

    AddWorldMapMarker(v);
    gUnknown_0202FDFC.unk12[v] |= 1;
}
asm(".global sub_08076A68\n.thumb_set sub_08076A68, SetupWorldMapForNewCampaign\n");

/* WAVE 36 (W36-E): NOT re-attempted. Read in full against the ROM and the C
 * above is the natural spelling of every statement -- a restart from scratch
 * reproduces it line for line, so the brief's "delete the body" instruction
 * has no purchase here and an agent inheriting this should not spend an
 * attempt confirming that. The residual is still the one `adds r4, r7, #0`
 * copy at the merge and the two axes the note above rules out are still ruled
 * out. One thing this wave DID settle that touches it: gUnknown_08614280 is a
 * real ROM pointer VARIABLE, not a force-addr word -- WorldMapNationPanel_Refresh matched
 * this wave with `(u16 *)gUnknown_08614280` and the ROM's own pool word is
 * `.word gUnknown_08614280`, so the `void *` declaration is right and the
 * five-symbols-hold-0x0200FC50 coincidence the wave-36 brief flagged is not a
 * force-addr tell here. */
