#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039850.
 * sub_08039850 @ 0x08039850
 */

#include "proc.h"
/* The scripted-line lookup for the current map: walk
 * gUnknown_0849D62C[map - 0x8a], a -1-terminated run of 8-byte records, and
 * play the first whose terrain, whatever GetCoPowerUseCount counts, and (when it is
 * not the wildcard 0) army all match the proc's army. Returns whether one
 * fired -- CoPowerSequence_ShowQuote falls back to the random line on 0.
 *
 * The table pointer being NULL is a separate early-out from the run being
 * empty, and both land on the same `movs r0,#0`. The -1 sentinel is
 * materialised (`movs r1,#1; rsbs r1,r1,#0`) rather than compared as an
 * immediate because the member is s8 and sign-extends past `cmp`'s 8-bit
 * unsigned range; the loop then keeps the constant live in r7. */
struct Unk39850Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x54);
    /* 0x54 */ int unk54;
};

u8 TryShowScriptedCoPowerQuote(ProcPtr procPtr)
{
    struct Unk39850Proc *proc = procPtr;
    const struct Unk0849D62C *e;

    e = gUnknown_0849D62C[gPlaySt.mapID - 0x8a];

    if (e != NULL)
    {
        while (e->unk01 != -1)
        {
            if (e->unk01 == gPlayers[proc->unk54].co
             && e->unk02 == GetCoPowerUseCount(proc->unk54)
             && (e->unk00 == 0 || e->unk00 == proc->unk54))
            {
                ShowCoQuote(e->unk04);
                return 1;
            }

            e++;
        }
    }

    return 0;
}
asm(".global sub_08039850\n.thumb_set sub_08039850, TryShowScriptedCoPowerQuote\n");
