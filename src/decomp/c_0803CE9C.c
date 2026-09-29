#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CE9C.
 * sub_0803CE9C @ 0x0803CE9C
 */

/* Family F068, third member. The family is defined by shape, not by callee:
 * this one calls sub_0803CE28 rather than DefeatOtherTeamsAndEndMatch, and `varies` lists
 * both the callee and both immediates. */

void sub_0803CE9C(void)
{
    sub_0803CE28(0, 0);
}
