#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071420.
 * sub_08071420 @ 0x08071420, sub_08071488 @ 0x08071488, sub_080714FC @ 0x080714FC
 */

/*
 * MPlayVolumeControl. MATCHES under the DEFAULT compiler -- no entry in
 * data/compiler-overrides.json is needed or wanted.
 *
 * Wave 38 (W38-A). The brief predicted this whole block would need old_agbcc
 * the way its two promoted neighbours MPlayTempoControl and MP_clear_modM do. It does
 * not: this is byte-for-byte under agbcc as built, and under old_agbcc too.
 * See the DISCRIMINATOR note in data/compiler-overrides.json -- only the two
 * siblings with a CALL in the loop body diverge.
 *
 * Straight pokeemerald/FE m4a source. Three things worth keeping:
 *   - `mplayInfo->ident++` reuses the register the guard's compare already
 *     loaded (`adds r0,r3,#1`), and the closing store is the CONSTANT, not the
 *     loaded value, so both ends share the one `.4byte 0x68736D53` pool word.
 *     That is why this reads `= MPLAY_ID_NUMBER` rather than needing the
 *     `u32 ident` local that MPlayTempoControl does.
 *   - `volume / 4` splits across the loop guard: the u16 truncation is the
 *     parameter's PROMOTE_MODE normalisation at entry (`lsls r6,r2,#0x10`) and
 *     the divide is LICM-hoisted into the preheader (`lsrs r6,r6,#0x12`).
 *     Do not try to author that split -- it is written by the loop optimiser.
 *   - the single `ldrb r3,[r1]` feeds both the MPT_FLG_EXIST test and the
 *     `|= MPT_FLG_VOLCHG`. CSE, from the nested `if`.
 *
 * `mov r7,sb; mov r6,r8` is the five-plus-live-values frame, not a loop signal:
 * trackBits, volume/4, 0x80, 3, i, track and bit are live at once.
 */
void MPlayVolumeControl(struct MusicPlayerInfo * mplayInfo, u16 trackBits, u16 volume)
{
    s32 i;
    u32 bit;
    struct MusicPlayerTrack * track;

    if (mplayInfo->ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->ident++;

    i = mplayInfo->trackCount;
    track = mplayInfo->tracks;
    bit = 1;

    while (i > 0)
    {
        if (trackBits & bit)
        {
            if (track->flags & MPT_FLG_EXIST)
            {
                track->volX = volume / 4;
                track->flags |= MPT_FLG_VOLCHG;
            }
        }

        i--;
        track++;
        bit <<= 1;
    }

    mplayInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_08071420\n.thumb_set sub_08071420, MPlayVolumeControl\n");

/*
 * MPlayPitchControl. MATCHES under the DEFAULT compiler -- no override entry.
 * Wave 38 (W38-A); shared skeleton documented in
 * work/MPlayVolumeControl/MPlayVolumeControl.c, discriminator in
 * data/compiler-overrides.json.
 *
 * The two stores are keyShiftX (+0x0b) and pitX (+0x0d) from one s16 parameter,
 * and the split reads exactly like MPlayVolumeControl's `volume / 4`: the parameter
 * is zero-extended at entry (PROMOTE_MODE, so s16 and u16 are identical THERE
 * and the sign lives in the second shift pair), r6 keeps the raw low half for
 * pitX, and `pitch >> 8` is LICM-hoisted into the preheader as
 * `lsls r0,r6,#0x10; asrs r7,r0,#0x18` -- sign-extend to 16 then arithmetic
 * shift 8. That is the s16 `>> 8`, NOT a byte extract; do not read the pair as
 * a mask.
 */
void MPlayPitchControl(struct MusicPlayerInfo * mplayInfo, u16 trackBits, s16 pitch)
{
    s32 i;
    u32 bit;
    struct MusicPlayerTrack * track;

    if (mplayInfo->ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->ident++;

    i = mplayInfo->trackCount;
    track = mplayInfo->tracks;
    bit = 1;

    while (i > 0)
    {
        if (trackBits & bit)
        {
            if (track->flags & MPT_FLG_EXIST)
            {
                track->keyShiftX = pitch >> 8;
                track->pitX = pitch;
                track->flags |= MPT_FLG_PITCHG;
            }
        }

        i--;
        track++;
        bit <<= 1;
    }

    mplayInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_08071488\n.thumb_set sub_08071488, MPlayPitchControl\n");

/*
 * MPlayPanpotControl. MATCHES under the DEFAULT compiler -- no override entry.
 * Wave 38 (W38-A); shared skeleton documented in
 * work/MPlayVolumeControl/MPlayVolumeControl.c.
 *
 * The simplest member of the family: one `strb` into panX (+0x15) and the
 * `|= MPT_FLG_VOLCHG`. The s8 parameter is zero-extended whole at entry
 * (`lsls #0x18; lsrs #0x18`) rather than hoisted, because nothing is computed
 * from it -- so this one's preheader carries only the two hoisted constants
 * 0x80 and 3 and no LICM-derived value at all. Reading the preheader that way
 * is the cheapest confirmation that the body has no arithmetic in it.
 */
void MPlayPanpotControl(struct MusicPlayerInfo * mplayInfo, u16 trackBits, s8 pan)
{
    s32 i;
    u32 bit;
    struct MusicPlayerTrack * track;

    if (mplayInfo->ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->ident++;

    i = mplayInfo->trackCount;
    track = mplayInfo->tracks;
    bit = 1;

    while (i > 0)
    {
        if (trackBits & bit)
        {
            if (track->flags & MPT_FLG_EXIST)
            {
                track->panX = pan;
                track->flags |= MPT_FLG_VOLCHG;
            }
        }

        i--;
        track++;
        bit <<= 1;
    }

    mplayInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_080714FC\n.thumb_set sub_080714FC, MPlayPanpotControl\n");
