#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BD78.
 * sub_0803BD78 @ 0x0803BD78, sub_0803BDBC @ 0x0803BDBC
 */

/* A leaf -- no `push` at all -- that arms gPlaySt for mode 3.
 *
 * The run of `adds r0,#1` / `subs r0,#1` steps on one address register is what
 * consecutive byte fields cost once the displacement leaves `strb`'s 5-bit
 * range; it is addressing, not a member array. unk28 is a whole-word `str` in
 * the middle of that run, which is why it is broken out of filler_0e in
 * include/unknown-globals.h. */
void sub_0803BD78(void)
{
    gPlaySt.gameMode = 3;
    gPlaySt.fog = 1;
    gPlaySt.coPowersEnabled = 1;
    gPlaySt.coAbilities = 1;
    gPlaySt.randomWeatherOn = 1;
    gPlaySt.weather = 0;
    gPlaySt.unk2e = 0;
    gPlaySt.defaultWeather = 0;
    gPlaySt.turnLimit = 0;
    gPlaySt.propertyFunds = 1000;
    gPlaySt.captureLimit = 0;
    if (gPlaySt.savingEnabled != 0)
        gPlaySt.animOpts = 1;
}

/* Re-arm gPlaySt across a InitVersusPlayState reset, carrying unk0c over it
 * by hand -- the byte is read into a callee-saved register before the call and
 * written straight back after, which is the whole reason r4 is pushed.
 *
 * The four zeroed bytes at 0x39..0x3c are spelled as unk38[1..4]: the array is
 * already proved by EventOp_SetArmyAiControlled's variable index, and a constant index folds
 * to the same displacement a separate scalar field would give, so this does
 * not disturb that model. */
void Versus_InitPlayState(void)
{
    u8 saved = gPlaySt.bgmOn;

    InitVersusPlayState();
    gPlaySt.bgmOn = saved;
    gPlaySt.aiControlled[1] = 0;
    gPlaySt.aiControlled[2] = 0;
    gPlaySt.aiControlled[3] = 0;
    gPlaySt.aiControlled[4] = 0;
    gPlaySt.fog = 1;
    gPlaySt.coPowersEnabled = 1;
    gPlaySt.coAbilities = 1;
    gPlaySt.weather = 0;
    gPlaySt.randomWeatherOn = 1;
    gPlaySt.unk00 = 1;
}
asm(".global sub_0803BDBC\n.thumb_set sub_0803BDBC, Versus_InitPlayState\n");
