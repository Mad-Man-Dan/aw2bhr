#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800520C.
 * sub_0800520C @ 0x0800520C, sub_080052D8 @ 0x080052D8, sub_080053A8 @ 0x080053A8, sub_08005474 @ 0x08005474
 */

void sub_0800520C(void)
{
    gActiveMap->flags &= 0xFEFF;
    gActiveMap->designSlot = 0;
    if (LoadDesignRoomName(gActiveMap->designSlot, gDesignRoomName) != 1)
    {
        if (!sub_080051EC((const char *)gActiveMap->designName))
        {
            sub_08004E38((char *)gActiveMap->designName, (const char *)gTextTable[0x9FE]);
            gActiveMap->flags |= 0x100;
            StartEventScript(gUnknown_08488164);
        }
        else
        {
            StartEventScript(gUnknown_084882E4);
        }
    }
    else
    {
        if (!sub_080051EC((const char *)gActiveMap->designName))
        {
            sub_08004E38((char *)gActiveMap->designName, (const char *)gTextTable[0x9FE]);
            gActiveMap->flags |= 0x100;
            StartEventScript(gUnknown_08488224);
        }
        else
        {
            StartEventScript(gUnknown_08488394);
        }
    }
}

void sub_080052D8(void)
{
    gActiveMap->flags &= 0xFEFF;
    gActiveMap->designSlot = 1;
    if (LoadDesignRoomName(gActiveMap->designSlot, gDesignRoomName) != 1)
    {
        if (!sub_080051EC((const char *)gActiveMap->designName))
        {
            sub_08004E38((char *)gActiveMap->designName, (const char *)gTextTable[0x9FF]);
            gActiveMap->flags |= 0x100;
            StartEventScript(gUnknown_08488164);
        }
        else
        {
            StartEventScript(gUnknown_084882E4);
        }
    }
    else
    {
        if (!sub_080051EC((const char *)gActiveMap->designName))
        {
            sub_08004E38((char *)gActiveMap->designName, (const char *)gTextTable[0x9FF]);
            gActiveMap->flags |= 0x100;
            StartEventScript(gUnknown_08488224);
        }
        else
        {
            StartEventScript(gUnknown_08488394);
        }
    }
}

void sub_080053A8(void)
{
    gActiveMap->flags &= 0xFEFF;
    gActiveMap->designSlot = 2;
    if (LoadDesignRoomName(gActiveMap->designSlot, gDesignRoomName) != 1)
    {
        if (!sub_080051EC((const char *)gActiveMap->designName))
        {
            sub_08004E38((char *)gActiveMap->designName, (const char *)gTextTable[0xA00]);
            gActiveMap->flags |= 0x100;
            StartEventScript(gUnknown_08488164);
        }
        else
        {
            StartEventScript(gUnknown_084882E4);
        }
    }
    else
    {
        if (!sub_080051EC((const char *)gActiveMap->designName))
        {
            sub_08004E38((char *)gActiveMap->designName, (const char *)gTextTable[0xA00]);
            gActiveMap->flags |= 0x100;
            StartEventScript(gUnknown_08488224);
        }
        else
        {
            StartEventScript(gUnknown_08488394);
        }
    }
}

/* `movs r3, #0` appears at the TOP OF EACH ARM, not once before the branch, so
 * the returned 0 is assigned inside both arms rather than at the declaration.
 * That is what frees r4 to hold the 0x200/0x400/0x800 mask; with a single
 * `int result = 0;` initialiser agbcc keeps r3 live across the whole body and
 * the masks land in r3 instead (measured with compile_probe, -4 bytes). */
int DesignRoomRefreshSlotFlag(int a1)
{
    int result;

    if (LoadDesignRoomName((u8)a1, gDesignRoomName) == 1)
    {
        result = 0;
        switch (a1)
        {
        case 0:
            gActiveMap->flags |= 0x200;
            break;
        case 1:
            gActiveMap->flags |= 0x400;
            break;
        case 2:
            gActiveMap->flags |= 0x800;
            break;
        }
    }
    else
    {
        result = 0;
        switch (a1)
        {
        case 0:
            gActiveMap->flags &= 0xFDFF;
            break;
        case 1:
            gActiveMap->flags &= 0xFBFF;
            break;
        case 2:
            gActiveMap->flags &= 0xF7FF;
            break;
        }
    }
    return result;
}
asm(".global sub_08005474\n.thumb_set sub_08005474, DesignRoomRefreshSlotFlag\n");
