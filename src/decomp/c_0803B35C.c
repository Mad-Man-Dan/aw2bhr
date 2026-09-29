#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B35C.
 * sub_0803B35C @ 0x0803B35C, sub_0803B37C @ 0x0803B37C, sub_0803B3B0 @ 0x0803B3B0
 */

/* The setter twin of SetSoundMasterVolume (src/decomp/c_0803B350.c), one slot along:
 * park the new value in gUnknown_030005CE, then push it to the m4a mixer as
 * MPlayVolumeControl's (MPlayVolumeControl) volume, with 0xFFFF as the track mask.
 *
 * TWO statements and not one. The ROM keeps &gUnknown_030005CE in r2 across the
 * `strh` and then re-reads the halfword out of it (`ldrh r2, [r2]`) to build the
 * third argument, which is the ordinary reload of a non-volatile global across
 * a statement boundary; spelling the argument as the parameter `a` instead
 * would drop the `ldrh` entirely. Nothing here is a volatile tell -- the
 * absolute-address chain is intact and the re-read does not survive a store to
 * a DIFFERENT address.
 * `int` parameter per the existing declaration: a bare `strh` is identical for
 * int/u32/u16/s16, and the prologue does not narrow r0.
 * `pop {r0}; bx r0` -> void. */

void SetMusicVolume(int a)
{
    gUnknown_030005CE = a;
    MPlayVolumeControl(gUnknown_03005AE0, 0xFFFF, gUnknown_030005CE);
}
asm(".global sub_0803B35C\n.thumb_set sub_0803B35C, SetMusicVolume\n");

/* The sound-subsystem reset: set both mixer knobs to 0x100, re-run
 * m4aSoundInit (m4aSoundInit), and blank the current/requested song id pair to
 * the 0xFFFF sentinel that PlayMusic and ReplayPendingMusic test against.
 *
 * `movs r4,#0x80; lsls r4,r4,#1` is a PLAIN constant 0x100, not wave 23's
 * named-constant-local: the shift is minimal (0x100 is not an imm8) and both
 * halves write the same register. r4 is a callee-saved register only because
 * the one constant feeds two consecutive calls. Likewise `ldr r2,=0xFFFF;
 * adds r1,r2,#0` is one CSEd literal reaching two stores, not two literals.
 * `pop {r4}; pop {r0}; bx r0` -> void. */

void InitSoundSystem(void)
{
    SetSoundMasterVolume(0x100);
    SetMusicVolume(0x100);
    m4aSoundInit();

    gUnknown_030005C8 = 0xFFFF;
    gUnknown_030005CA = 0xFFFF;
}
asm(".global sub_0803B37C\n.thumb_set sub_0803B37C, InitSoundSystem\n");

/* One statement: index the twelve-word ROM table gUnknown_080910FC and hand the
 * entry to SoundMode_rev01 (m4aSoundMode), so this is "select sound mode preset
 * n". It belongs to the same m4a forwarder run as src/decomp/c_0803B3C8.c,
 * which reaches SoundMode_rev01 with a computed argument instead of a table one.
 *
 * `int` and not a narrow type, on wave 21's bare-prologue rule: the parameter
 * goes straight into `lsls r0, r0, #2` with no masking of the incoming
 * register, and PROMOTE_MODE would have put a shift pair in front of it for a
 * u8/u16. `pop {r0}; bx r0` -> void. */

void SetSoundModeByIndex(int a)
{
    SoundMode_rev01(gUnknown_080910FC[a]);
}
asm(".global sub_0803B3B0\n.thumb_set sub_0803B3B0, SetSoundModeByIndex\n");
