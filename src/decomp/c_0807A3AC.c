#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A3AC.
 * sub_0807A3AC @ 0x0807A3AC
 */

int GetVictoryQuoteTextId(int a, int b)
{
    if (gPlaySt.gameMode == 1)
    {
        switch (b)
        {
        case 1: return 0x7C;
        case 2: return 0xB1;
        case 3: return 0xF4;
        case 4: return 0x13F;
        case 5: return 0x182;
        case 6: return 0x1D5;
        case 7: return 0x207;
        case 8:
            switch (a)
            {
            case 1: return 0x23A;
            case 4: return 0x23B;
            case 2: return 0x23C;
            }
            break;
        case 9: return 0x3A8;
        case 10: return 0x3DA;
        case 11: return 0x404;
        case 12: return 0x430;
        case 13: return 0x44E;
        case 14: return 0x46C;
        case 15:
            switch (a)
            {
            case 3: return 0x481;
            case 5: return 0x482;
            case 16: return 0x483;
            }
            break;
        case 16:
            switch (a)
            {
            case 3: return 0x4C8;
            case 5: return 0x4C9;
            case 16: return 0x4CA;
            }
            break;
        case 17: return 0x685;
        case 18: return 0x6B5;
        case 19: return 0x6D5;
        case 20: return 0x70A;
        case 21: return 0x72D;
        case 22: return 0x755;
        case 23:
            switch (a)
            {
            case 6: return 0x772;
            case 7: return 0x773;
            case 18: return 0x774;
            }
            break;
        case 24:
            switch (a)
            {
            case 6: return 0x7B1;
            case 7: return 0x7B2;
            case 18: return 0x7B3;
            }
            break;
        case 25: return 0x504;
        case 26: return 0x52F;
        case 27: return 0x56A;
        case 28: return 0x59A;
        case 29: return 0x5BB;
        case 30: return 0x5EF;
        case 31:
            switch (a)
            {
            case 8: return 0x614;
            case 9: return 0x615;
            case 17: return 0x616;
            }
            break;
        case 32:
            switch (a)
            {
            case 8: return 0x654;
            case 9: return 0x655;
            case 17: return 0x656;
            }
            break;
        case 33:
            switch (a)
            {
            case 1: return 0x7F1;
            case 4: return 0x7F3;
            case 2: return 0x7F2;
            case 3: return 0x7F4;
            case 5: return 0x7F5;
            case 16: return 0x7F6;
            case 6: return 0x7F7;
            case 7: return 0x7F8;
            case 18: return 0x7F9;
            case 8: return 0x7FA;
            case 9: return 0x7FB;
            case 17: return 0x7FC;
            }
            break;
        case 34:
            switch (a)
            {
            case 1: return 0x895;
            case 4: return 0x897;
            case 2: return 0x896;
            case 3: return 0x898;
            case 5: return 0x899;
            case 16: return 0x89A;
            case 6: return 0x89B;
            case 7: return 0x89D;
            case 18: return 0x89C;
            case 8: return 0x89E;
            case 9: return 0x89F;
            case 17: return 0x8A0;
            }
            break;
        case 35: return 0x263;
        case 36: return 0x27E;
        case 37: return 0x2AF;
        case 38: return 0x2C9;
        case 39: return 0x2F9;
        case 40: return 0x321;
        case 41: return 0x33F;
        case 42:
            switch (a)
            {
            case 1: return 0x369;
            case 4: return 0x36A;
            case 2: return 0x36B;
            }
            break;
        }
    }

    return gUnknown_085D3DD0[a].victoryQuote;
}
asm(".global sub_0807A3AC\n.thumb_set sub_0807A3AC, GetVictoryQuoteTextId\n");
