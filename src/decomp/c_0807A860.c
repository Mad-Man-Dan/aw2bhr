#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A860.
 * sub_0807A860 @ 0x0807A860
 */

#include "proc.h"

void ResultsScreen_ShowVictoryQuote(void)
{
    struct Unk03001470 *p;

    p = StartTextBox(0x10, 0xF, gBG0TilemapBuffer,
                     GetVictoryQuoteTextId(gPlayers[GetResultsArmy()].co,
                                  GetCampaignMissionId()),
                     0x8000, 0x41);
    p->unk3a = 2;
}
asm(".global sub_0807A860\n.thumb_set sub_0807A860, ResultsScreen_ShowVictoryQuote\n");
