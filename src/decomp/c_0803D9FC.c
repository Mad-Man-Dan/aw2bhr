#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D9FC.
 * sub_0803D9FC @ 0x0803D9FC
 */

#include "hardware.h"
/* MATCHED, wave 55 (W55-E), two attempts. A debug CO/army editor page.
 *
 * PROMOTION REQUIRES THE WHOLE .rodata BLOB PLACED -- 105 words,
 * 0x08091154 through 0x080912F4 inclusive, contiguous. Run
 * `python tools/trymatch.py DebugArmyEditor_Loop --diff` and paste the "rodata": [...]
 * list it prints into this function's data/promoted.json entry, then re-run
 * tools/split_rodata.py and tools/gen_lds.py before building. trymatch reports
 * `relocs: name different symbols that resolve to the same address`, which is
 * the match verdict, not a defect.
 *
 * THE ONE THING THAT COST AN ATTEMPT: the switch selector. The ROM does
 * `ldrh r0,[r7,#0x20]; subs r0,#1; lsls r0,#0x10; asrs r0,#0x10` -- an
 * UNSIGNED halfword load followed by a sign-extend of the DIFFERENCE. Plain
 * `switch (proc->unk20 - 1)` on an `s16` field gives `ldrsh` and no shift pair.
 * The `(s16)` cast on the whole subtraction is what produces both halves: the
 * narrowing lets GCC shorten the load to `ldrh` (only the low 16 bits survive)
 * and forces the explicit re-widening afterwards. Note this does NOT contradict
 * the field being `s16` -- the two cursor tests above load the same field both
 * ways in one statement (`ldrh` for the value that is stored back, `ldrsh` for
 * the comparison), which is exactly what an `s16` produces.
 *
 * See NOTES.md beside this file for how every symbol was resolved; the short
 * version:
 *
 *  - NONE of the nine gUnknown_080912xx "globals" are globals. They are STRING
 *    LITERALS, and the four gUnknown_080911xx are tables of them. All verified
 *    against baserom.gba. Nothing here is declared in include/unknown-globals.h
 *    and nothing should be.
 *  - gUnknown_080912E8/EC/F0/F4 ARE -fforce-addr words, holding 0x03003FC0,
 *    0x08499598, 0x03004080 and 0x03002EE0. The honest spelling names the real
 *    object and agbcc rebuilds the extra indirection itself.
 *  - 0x03002EE0 is `gpKeySt`, NOT an unnamed global. include/unknown-globals.h
 *    records that wave 20 invented a `gUnknown_03002EE0` for it and the SPLIT
 *    build caught it. Checked before writing, deliberately.
 *  - StepClampedWithWrap was already promoted (src/decomp/c_0803D990.c); its prototype
 *    was copied from the definition, not guessed from the call sites.
 *
 * The parameter is modelled as a plain struct rather than a PROC: the two
 * fields live at +0x1e and +0x20, which is INSIDE this tree's PROC_HEADER
 * (0x00..0x28), so it is not a proc in the c_08073E0C sense. Byte-neutral here
 * either way; flagging it because I could not prove it. */

struct Unk3D9FC
{
    /* 0x00 */ u8 filler_00[0x1E];
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
};
static const char *const sOnOff[] = { "OFF", "ON" };
static const char *const sColors[] = {
    "NEUT", "RED", "BLUE", "GREEN", "YELLOW", "BLACK"
};
static const char *const sCtrl[] = { "NON", "HUMAN", "COM" };
static const char *const sCoNames[] = {
    "CATH", "RYO", "MAX", "WHIP", "DOMI", "BILY", "KIKU", "ASKA", "EAGL",
    "MOP", "HELL", "KONG", "CAT", "SNAK", "HAWK", "HACH", "EWAN", "HANA",
    "YAMA"
};

void DebugArmyEditor_Loop(struct Unk3D9FC *proc)
{
    int delta;
    u8 army;

    delta = 0;
    army = proc->unk1E;

    PutAsciiStringSprites(8, 0x00, "FORCE   /");
    PutAsciiStringSprites(8, 0x08, "PLAY");
    PutAsciiStringSprites(8, 0x10, "MONEY");
    PutAsciiStringSprites(8, 0x18, "BREAK");
    PutAsciiStringSprites(8, 0x20, "SYOGUN");
    PutAsciiStringSprites(8, 0x28, "TEAM");
    PutAsciiStringSprites(8, 0x30, "SAKU");
    PutAsciiStringSprites(8, 0x38, "TURN");
    PutAsciiStringSprites(8, 0x40, "COLOR");

    DrawSpriteNumberFont2(0x50, 0x00, GetMapArmyCount(gPlaySt.mapID));
    DrawSpriteNumberFont2(0x40, 0x00, proc->unk1E);
    PutAsciiStringSprites(0x40, 0x08, sCtrl[gPlayers[army].aiControlled]);
    DrawSpriteNumberFont2(0x68, 0x10, gPlayers[army].funds);
    DrawSpriteNumberFont2(0x68, 0x18, gPlayers[army].coCharge);
    PutAsciiStringSprites(0x40, 0x20, sCoNames[gPlayers[army].co]);
    DrawSpriteNumberFont2(0x68, 0x28, gPlayers[army].team);
    PutAsciiStringSprites(0x50, 0x30, sOnOff[gPlaySt.fog]);
    DrawSpriteNumberFont2(0x40, 0x38, gUnknown_03004080);
    PutAsciiStringSprites(0x40, 0x40, sColors[gPlayers[army].teamColor]);
    PutAsciiStringSprites(0, proc->unk20 * 8, "/");

    if (gpKeySt->pressed & R_BUTTON)
    {
        do
        {
            proc->unk1E++;

            if (proc->unk1E > 4)
                proc->unk1E = 1;
        } while (gPlayers[proc->unk1E].aiControlled == 0);
    }

    if (gpKeySt->pressed & L_BUTTON)
    {
        do
        {
            proc->unk1E--;

            if (proc->unk1E <= 0)
                proc->unk1E = 4;
        } while (gPlayers[proc->unk1E].aiControlled == 0);
    }

    if (gpKeySt->repeated & DPAD_UP)
    {
        if (proc->unk20 > 1)
            proc->unk20--;
    }

    if (gpKeySt->repeated & DPAD_DOWN)
    {
        if (proc->unk20 <= 6)
            proc->unk20++;
    }

    if (gpKeySt->repeated & DPAD_LEFT)
        delta = 0xFF;

    if (gpKeySt->repeated & DPAD_RIGHT)
        delta = 1;

    if (gpKeySt->repeated & (DPAD_RIGHT | DPAD_LEFT))
    {
        switch ((s16)(proc->unk20 - 1))
        {
        case 0:
            gPlayers[army].aiControlled =
                StepClampedWithWrap(gPlayers[army].aiControlled, (s8)delta, 1, 2, 1);
            break;

        case 1:
            gPlayers[army].funds =
                StepClampedWithWrap(gPlayers[army].funds,
                             (s8)delta * 0xC350, 0, 0xF423F, 0);
            break;

        case 2:
            gPlayers[army].coCharge =
                StepClampedWithWrap(gPlayers[army].coCharge,
                             (s8)delta * 0x2710, 0, GetSuperCoPowerCost(army), 0);
            break;

        case 3:
            gPlayers[army].co =
                StepClampedWithWrap(gPlayers[army].co, (s8)delta, 0, 0x12, 1);
            break;

        case 4:
            gPlayers[army].team =
                StepClampedWithWrap(gPlayers[army].team, (s8)delta, 0, 3, 0);
            break;

        case 5:
            gPlaySt.fog =
                StepClampedWithWrap(gPlaySt.fog, (s8)delta, 0, 1, 1);
            break;

        case 6:
            gUnknown_03004080 =
                StepClampedWithWrap(gUnknown_03004080, (s8)delta, 0, 0x3E7, 1);
            break;
        }
    }

    if (gpKeySt->pressed & 3)
    {
        BuildEnemyArmyMasks();
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
asm(".global sub_0803D9FC\n.thumb_set sub_0803D9FC, DebugArmyEditor_Loop\n");
