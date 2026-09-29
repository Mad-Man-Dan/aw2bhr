#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B578.
 * sub_0803B578 @ 0x0803B578, sub_0803B588 @ 0x0803B588, sub_0803B5A4 @ 0x0803B5A4
 */

#include "proc.h"
/* A proc callback with no `bl` callers anywhere in asm/ -- it is reached only
 * through a ProcCmd table -- so the parameter is the proc pointer every such
 * entry point receives, and include/unknown-functions.h deliberately does not
 * declare it. It reads one slot, the halfword at +0x64 that FadeOutMusic writes
 * when it starts this proc's script, and replays it as a song id.
 *
 * unk64 is `s16` HERE and `u16` in the two other files that describe the same
 * slot (src/decomp/c_0803B55C.c, src/decomp/c_0803B608.c). That is a real
 * disagreement and this file has the stronger evidence: the ROM reads the slot
 * `movs r1,#0; ldrsh r0,[r0,r1]`, and a `u16` member cannot produce a
 * sign-extending load -- it would give `adds r0,#0x64; ldrh r0,[r0]` (0x64 is
 * past `ldrh`'s scaled 5-bit displacement) plus a shift pair for the
 * conversion, four instructions where the ROM spends three. The other two files
 * only ever STORE to the slot or decrement it, and a bare `strh` is identical
 * for both signednesses, so neither of them is evidence against this. The
 * struct is local to this file per the project rule for pointer-parameter
 * types, so nothing else moves. `pop {r0}; bx r0` -> void. */

struct UnkB578Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x64);
    /* 0x64 */ s16 unk64;
};

/* Fade the BGM out over `a` frames and start the ProcScr_FadeSound proc to run
 * the fade down, parking the speed in the proc's +0x64 slot scaled by 16. It
 * no-ops when the requested-song slot still holds the 0xFFFF sentinel, i.e.
 * when there is nothing playing to fade. Its counterpart
 * src/decomp/c_0803B5E8.c is the `FadeOutMusic(2)` wrapper.
 *
 * THE PARAMETER IS `int`, AND include/unknown-functions.h DECLARED IT `s16`
 * FROM WAVE 7 UNTIL THIS ONE. The prologue is a bare `adds r4, r0, #0`: the
 * argument is copied to a callee-saved register with no masking whatsoever, and
 * PROMOTE_MODE fires unconditionally, so any sub-word parameter has to carry
 * `lsls #0x10; lsrs #0x10` right there. Measured rather than argued -- the
 * `s16` spelling compiles to a DIFFERENT function, not merely a longer one: it
 * folds the entry zero-extension and the `<< 4` into `asrs r4, r4, #0xc` and
 * loses the separate `lsls r1, r4, #0x10` in the m4aMPlayFadeOut argument setup.
 * The `int` spelling below is byte-exact.
 * The `lsls #0x10; lsrs #0x10` further down IS PROMOTE_MODE, but at the CALL,
 * not at entry: m4aMPlayFadeOut's second parameter is `u16`, and the pair lands
 * after gUnknown_03005AE0 is already in r0, i.e. inside the argument setup --
 * the "conversion at a use" shape, same as PlayMusicOrSfx2's.
 * `a = 2` is a plain default for a zero argument and reuses r4, so it is an
 * assignment to the parameter and not a second local.
 * `pop {r4}; pop {r0}; bx r0` -> void. */

struct UnkB5A4Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x64);
    /* 0x64 */ u16 unk64;
};

void PlayMusicAfterFade_PlayMusic(struct UnkB578Proc *proc)
{
    PlayMusic(proc->unk64);
}
asm(".global sub_0803B578\n.thumb_set sub_0803B578, PlayMusicAfterFade_PlayMusic\n");

/* Stop everything: blank the requested-song slot to the 0xFFFF sentinel and
 * call m4aMPlayAllStop, whose body is a fixed eleven-iteration loop stopping every
 * m4a track -- m4aMPlayAllStop. The pair is the hard stop that
 * src/decomp/c_0803B5F4.c's lone `gUnknown_030005CA = 0xFFFF` is the soft half
 * of. `ldr r2,=0xFFFF; adds r0,r2,#0` is the ordinary materialisation of a
 * literal too wide for `movs`. `pop {r0}; bx r0` -> void. */

void StopAllMusic(void)
{
    gUnknown_030005CA = 0xFFFF;
    m4aMPlayAllStop();
}
asm(".global sub_0803B588\n.thumb_set sub_0803B588, StopAllMusic\n");

void FadeOutMusic(int a)
{
    struct UnkB5A4Proc *proc;

    if (gUnknown_030005CA != 0xFFFF)
    {
        if (a == 0)
            a = 2;

        m4aMPlayFadeOut(gUnknown_03005AE0, a);

        proc = Proc_Start(ProcScr_FadeSound, PROC_TREE_3);
        proc->unk64 = a << 4;
    }
}
asm(".global sub_0803B5A4\n.thumb_set sub_0803B5A4, FadeOutMusic\n");
