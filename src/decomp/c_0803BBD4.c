#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BBD4.
 * sub_0803BBD4 @ 0x0803BBD4
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* MATCHED byte-for-byte (wave 37, W37-Q2).
 *
 * Rebuild gUnknown_03003F30[0..3]: "is slot i a live, present player?", with
 * slot 1 getting an extra escape hatch through IsAnyWorldMapMissionCleared.
 *
 * The DUPLICATED `GetSuspendIdForGameMode(i)` is real source, not a compiler artefact.
 * agbcc never CSEs a call, so the two `bl GetSuspendIdForGameMode` in the stream are two
 * source calls; the first one's result is only ever used on the i == 1 path,
 * and the else arm fetches it again. Writing it once above the `if` and
 * letting both arms share it produces one `bl`, which is 12 bytes short.
 *
 * The three narrowings are all prototype-driven and none of them is a source
 * cast: GetSuspendIdForGameMode returns s8 so its result is re-narrowed (`lsls #0x18;
 * lsrs #0x18` into the u8 `t`), GetSuspendFlag takes s8 so `t` is sign-extended
 * at the call (`lsls #0x18; asrs #0x18` -- and in the else arm agbcc folds
 * that onto the still-live shifted r0, which is why that arm is one
 * instruction shorter), and sub_08016E04 takes u16 so the already
 * zero-extended `t` goes through untouched.
 *
 * IsAnyWorldMapMissionCleared's `lsls #0x18; lsrs #0x18` before the zero test is what forced
 * its return type from s32 to bool8 -- see include/unknown-functions.h and
 * src/decomp/c_08078198.c. An s32 return gives a bare `cmp r0,#0` here.
 *
 * `gUnknown_03003F30[1] = 0` really is the constant index 1 while the two
 * `= 1` stores are `[i]`: the ROM reaches the zero store with `strb r0,[r6,#1]`
 * (a folded displacement) but both one-stores with `adds r1,r5,r6` (a variable
 * index), and the two `= 1` sites are duplicated rather than shared, so this is
 * not agbcc const-propagating i == 1 in one place and not the other. The `0`
 * itself is r0 left over from the `cmp r0,#0` -- agbcc reuses a register it
 * knows holds zero instead of emitting `movs r0,#0`. */

void MainMenu_0803BBD5(void)
{
    u8 i;
    u8 t;

    SetMainMenuCarouselPosition(gUnknown_0200C420.unk0d);

    for (i = 0; i <= 6; i++)
        gUnknown_03003F30[i] = 0;

    for (i = 0; i <= 3; i++)
    {
        t = GetSuspendIdForGameMode(i);

        if (i == 1)
        {
            if ((GetSuspendFlag(t) != 0 && sub_08016E04(t) != 0) || IsAnyWorldMapMissionCleared() != 0)
                gUnknown_03003F30[i] = 1;
            else
                gUnknown_03003F30[1] = 0;
        }
        else
        {
            t = GetSuspendIdForGameMode(i);
            if (GetSuspendFlag(t) != 0 && sub_08016E04(t) != 0)
                gUnknown_03003F30[i] = 1;
            else
                gUnknown_03003F30[i] = 0;
        }
    }
}

asm(".global sub_0803BBD4\n.thumb_set sub_0803BBD4, MainMenu_0803BBD5\n");
