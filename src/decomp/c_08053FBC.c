#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08053FBC.
 * sub_08053FBC @ 0x08053FBC
 */

/* MATCHED in Wave 66, W66-E. The Wave 36 decomposition was correct; the
 * missing spelling was the dynamic boolean subscript
 * `unk00[gUnknown_03004580[a1][3] == 2][phase]`. Unlike an if/else or a
 * conditional selecting two complete elements, it computes the row and phase
 * scalings once and branches around only the conditional +4, exactly as the
 * ROM does. The two row exclusions must be nested ifs: spelling them as
 * `row != 0 && row != 1` lets fold combine them into one unsigned range test
 * and makes the function four bytes short.
 *
 * Other settled evidence:
 *   - gUnknown_08136128 / 0813612C / 08136130 are NOT globals. They are agbcc's
 *     own `-fforce-addr` address-constant pool, three consecutive words holding
 *     &gUnknown_03004580, 0x085D6C88 and &gUnknown_02029C04 (dereferenced in
 *     baserom.gba). The honest spelling -- naming the three real objects -- is
 *     right: the build emits `.rodata` relocations with addends 0, 4 and 8 in
 *     exactly the ROM's positions. Note gUnknown_03004580 is ALSO reached
 *     through an ordinary inline pool word later in the same function, so one
 *     source spelling really does produce both forms.
 *   - The whole first half is settled: `cmd == 0xff`, the
 *     gUnknown_03004508 == gUnknown_08551E12[cursor] gate, StartFigureFireScript(a1,
 *     cmd), the cursor bump and the gUnknown_02029BEC flag store.
 *   - gUnknown_02029BEC[2][5] tiles exactly up to gUnknown_02029C00[2], which
 *     tiles up to gUnknown_02029C04[2]; all three are declared with that
 *     evidence in unknown-globals.h, as is struct Unk085D6C88's new unk00.
 *   - `& 1` on gUnknown_02029C04[a1] reuses the constant 1 the ROM already has
 *     live from `cursor == 1`; that falls out on its own.
 * The configured verdict is exact with relocation-equivalent `.rodata` words
 * at 0x08136128, 0x0813612C and 0x08136130. */
void StepShotFireTimeline(u16 a1)
{
    u16 cmd;

    cmd = gUnknown_020296B0[a1].unk0c[gUnknown_02029C00[a1]];

    if (cmd != 0xff
        && gUnknown_03004508 == gUnknown_08551E12[gUnknown_02029C00[a1]])
    {
        u16 v;

        StartFigureFireScript(a1, cmd);
        gUnknown_02029C00[a1]++;
        gUnknown_02029BEC[a1][cmd] = 1;

        if (gUnknown_03004580[a1][1] != 0)
        {
            if (gUnknown_03004580[a1][1] != 1)
            {
                if (gUnknown_02029C00[a1] == 1)
                {
                    v = gUnknown_085D6C88[gUnknown_03004580[a1][1]]
                            .unk00[gUnknown_03004580[a1][3] == 2]
                                  [gUnknown_02029C04[a1] & 1];

                    if (v != 0)
                    {
                        PlayMusicOrSfx(gUnknown_085D6C88[gUnknown_03004580[a1][1]]
                            .unk00[gUnknown_03004580[a1][3] == 2]
                                  [gUnknown_02029C04[a1] & 1]);
                        gUnknown_02029C04[a1]++;
                    }
                }
            }
        }
    }
}
asm(".global sub_08053FBC\n.thumb_set sub_08053FBC, StepShotFireTimeline\n");
