#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807823C.
 * CampaignMapNoOp @ 0x0807823C
 */

#include "proc.h"

/* A bare `bx lr` -- it does nothing at all, so its arity is invisible from the
 * body and can only come from a call site. Wave 43 found one: EndOfGame_FinishCampaignMap
 * emits `ldrb r0, [r5, #2]; subs r0, #0x8a; bl CampaignMapNoOp`, materialising an
 * int argument in r0 immediately before the call. The earlier reading -- "no
 * caller anywhere in the ROM reaches it by `bl`, so `void (void)` is all the
 * evidence there is" -- was correct when written and is now falsified. Taking
 * the parameter is byte-neutral here: the body reads nothing either way. */

void CampaignMapNoOp(int a)
{
}

asm(".global sub_0807823C\n.thumb_set sub_0807823C, CampaignMapNoOp\n");
