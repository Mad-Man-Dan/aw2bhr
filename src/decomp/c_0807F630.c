#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F630.
 * sub_0807F630 @ 0x0807F630
 */

#include "proc.h"
/* MATCHED (wave 57, W57-C). trymatch reports `.rodata` pool words that must be
 * PLACED: the promotion's data/promoted.json entry needs
 *     "rodata": ["0x081D9378"]
 * then tools/split_rodata.py and tools/gen_lds.py before building.
 *
 * Confirms a selected batting order and hands it to the gUnknown_086165C0 proc:
 * fills gPlayers[1..n]'s unk1d / unk1a / unk00 from the picked list,
 * then copies eleven fields of this proc onto the one it starts.
 *
 * `struct Unk807CE5C` is copied VERBATIM from src/decomp/c_0807CE5C.c, which is
 * the sole caller and already declares `void CoSelect_AssignRemainingAndFinish(struct Unk807CE5C *)`
 * locally. Same tag, same layout, nothing reshaped.
 *
 * The `+ 1` on every gPlayers subscript rides in the 0x56 / 0x59 / 0x3c
 * displacements (0x3c + 0x1a, 0x3c + 0x1d, 0x3c + 0x00) -- the one-based
 * indexing that array carries everywhere.
 *
 * gUnknown_081D9378 IS NOT A GLOBAL. It is this unit's -fforce-addr .rodata
 * word holding &gPlayers, which is why the ROM's chain is three deep
 * (`ldr rN,=word; ldr rM,[rN]; ldr rK,[rM]`). Naming gPlayers honestly
 * emits exactly that: agbcc parks one `.LC` word in this unit's .rodata and all
 * five inline pool slots point at it. Do not declare a gUnknown_081D93xx
 * symbol -- see the wave-53 note in include/unknown-globals.h.
 *
 * TWO SPELLINGS ARE LOAD-BEARING, both isolated with compile_probe:
 *
 *  - The unk3c byte is read TWICE, not bound to a local. Binding it costs the
 *    ROM's `adds r0, r4, #0`: with one pseudo the compare reads the load's own
 *    register, while two reads leave CSE's copy behind, which is what the ROM
 *    has. This is the wave-49 "NOT binding a VALUE to a local is a lever" rule
 *    with the sign the other way round -- here NOT binding is what matches.
 *    `if ((t = arr[i]) == 0xff)` does NOT produce the copy; only two reads do.
 *
 *  - `case 1:` must SHARE THE LABEL with `default:`, not be a separate
 *    `case 1: v = 1; break;`. Both spell the same machine code once
 *    cross-jumping has merged the duplicate blocks, and both give the five-entry
 *    jump table (four cases plus a default is below agbcc's tablejump
 *    threshold, so dropping `case 1` altogether emits a compare chain instead).
 *    But the separate block is an extra basic block, and that RENUMBERS THE
 *    SPILL SLOTS: the five `proc + K` addresses move from sp+0xc/0x10/8/0x14/4
 *    to sp+0x14/4/0x10/8/0xc. Same instructions, different immediates.
 *
 * The five `proc + K` addresses really are computed in the loop PREHEADER even
 * though only proc+0x52 and proc+0x64 are read inside the loop -- that is
 * agbcc's own placement, not something the source arranges. */

struct Unk807CE5C
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
    /* 0x44 */ u8 filler_44[0x04];
    /* 0x48 */ u16 unk48;
    /* 0x4a */ u8 filler_4a[0x02];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
    /* 0x50 */ u8 filler_50[0x02];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[0x04];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6a */ u16 unk6a;
};
/* Promoted but undeclared; signature taken from src/decomp/c_0807F8D0.c. */
void StartBlockWarRoomSelection(ProcPtr);

void CoSelect_AssignRemainingAndFinish(struct Unk807CE5C *p)
{
    struct Unk807CE5C *q;
    int i;
    int k;
    int v;

    if (p->unk64 < sub_0803BD14())
    {
        k = 0;

        for (i = 0; i < GetMapArmyCount(gPlaySt.mapID); i++)
        {
            if (gUnknown_085C77A0[gPlaySt.mapID].unk3c[i] == 0xff)
            {
                if (i == p->unk64)
                {
                    gPlayers[i + 1].co =
                        gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c];
                }
                else if (i < p->unk64)
                {
                    gPlayers[i + 1].co = gUnknown_030058D4[k];
                    k++;
                }
                else
                {
                    if (gUnknown_030059C0[i] == 0)
                        gPlayers[i + 1].teamColor = gUnknown_03005958[i] + 1;
                    else
                        gPlayers[i + 1].teamColor = PickArmyTeamColor(i + 1, 1);

                    switch (gPlayers[i + 1].teamColor)
                    {
                    case 1:
                    default:
                        v = 1;
                        break;
                    case 2:
                        v = 3;
                        break;
                    case 3:
                        v = 8;
                        break;
                    case 4:
                        v = 6;
                        break;
                    case 5:
                        v = 0xb;
                        break;
                    }

                    gPlayers[i + 1].co = v;
                }
            }
            else
            {
                gPlayers[i + 1].co =
                    gUnknown_085C77A0[gPlaySt.mapID].unk3c[i];
            }

            gPlayers[i + 1].teamColor =
                PickArmyTeamColor(i + 1, GetCoDefaultTeamColor(gPlayers[i + 1].co));
            gPlayers[i + 1].funds = 0;
        }

        gUnknown_030033EC = p->unk64 + 1;

        q = (struct Unk807CE5C *)Proc_Find(gUnknown_086165C0);
        Proc_Goto(q, 0);
        StartBlockWarRoomSelection(q);
        Proc_StartBlocking(gUnknown_08616710, q);

        q->unk52 = p->unk52;
        q->unk4e = p->unk4e;
        q->unk58 = p->unk58;
        q->unk5c = p->unk5c;
        q->unk60 = p->unk60;
        q->unk64 = p->unk64;
        q->unk66 = p->unk66;
        q->unk40 = p->unk40;
        q->unk48 = p->unk48;
        q->unk68 = p->unk68;
        q->unk6a = p->unk6a;
    }
}
asm(".global sub_0807F630\n.thumb_set sub_0807F630, CoSelect_AssignRemainingAndFinish\n");
