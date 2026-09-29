#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08005F4C.
 * sub_08005F4C @ 0x08005F4C
 */

#include "hardware.h"
/* Design-room item ring. Several state switches run back to back in one
 * frame, so a phase entered by one switch is serviced by the next.
 *
 * The ring loops index gDesignRing[slot] directly and wrap `slot` to 0: loop.c
 * treats that set-to-constant as a biv update and strength-reduces the address
 * into the ROM's walking pointer, with the reset hoisted as a base register.
 * A hand-written walking pointer gives a different preheader. The mark loops
 * in 0x2C/0x2D also keep the function-wide slot/i live, which is what puts
 * e/slot/i in r4/r5/r6. */

#define SAFE_SCALE(s) ((s) != 0 ? (s) : 2)
#define SET_RING_AFFINE(xs, ys)                                              \
    SetObjAffine(0, Div(COS_Q12(0) * 16, SAFE_SCALE(xs)),                    \
                    Div(-SIN_Q12(0) * 16, SAFE_SCALE(ys)),                   \
                    Div(SIN_Q12(0) * 16, SAFE_SCALE(xs)),                    \
                    Div(COS_Q12(0) * 16, SAFE_SCALE(ys)))

/* gUnknown_0200B0D0 is gDesignRing under its linker name (aw2bhr.lds:
 * `gDesignRing = gUnknown_0200B0D0`). State 0x2C's sub_080077EC argument must
 * name it differently from every other gDesignRing reference: with one name,
 * gcse's PRE sees the argument's address as available at the later ring loops,
 * hoists `&.LC` above the `frame < 0` test and leaves a stray -fforce-addr
 * .rodata word plus a spilled pointer. The ROM has neither. */
extern struct DesignRingEntry gUnknown_0200B0D0[];

void DesignRoomMode_Ring(void)
{
    struct DesignRingEntry *e;
    int count, visible;
    int i, slot, frame, done;
    struct KeySt *key;

    if (gActiveMap->stateChanged)
    {
        gActiveMap->flags |= 0x40;
        gActiveMap->stateChanged = 0;
        gActiveMap->state = 0;
        gActiveMap->tilePanelYSpeed = 0;
        gActiveMap->ringIndex = 0;
        gActiveMap->flags &= ~0x20;
        gActiveMap->flags &= ~3;
        DesignRoomBuildItemList(gActiveMap->editMode,
                     gActiveMap->editMode == 0 ? gActiveMap->propertyArmy
                                                : gActiveMap->unitArmy);
        DesignRoomLoadTerrainNamePalettes();
        sub_08003A80(0, 0xF0, 0xA0, 0xA0);
        gUnknown_030030E0.bits.target1_enable_bg1 = 1;
        gUnknown_030030E0.bits.target1_enable_bg3 = 1;
        gUnknown_030030E0.bits.target1_enable_obj = 1;
        gUnknown_030030E0.bits.target2_enable_bg1 = 1;
        gUnknown_030030E0.bits.target2_enable_bg3 = 1;
        gUnknown_030030E0.bits.target2_enable_obj = 1;
        gUnknown_03001FFC = 8;
        gUnknown_03002020 = 0x10;
        gUnknown_03002B28 = 0;
    }

    if (gActiveMap->editMode == 0)
    {
        count = 10;
        visible = 9;
    }
    else
    {
        count = 8;
        visible = 7;
    }

    switch (gActiveMap->state)
    {
    case 0:
        gActiveMap->state = 2;
        gActiveMap->flags |= 0x40;
        DesignRoomBuildRing(gActiveMap->editMode, -1);
        DesignRoomHideTilePanel();
        DesignRoomHideCoordBox();
        PlayMusicOrSfx2(0x65);
        DesignRoomSaveSelection();
        break;

    case 2:
        gUnknown_03002EFC -= 8;
        if (gUnknown_03002EFC <= 0x77)
        {
            gUnknown_03002EFC = 0x78;
            gActiveMap->state = 4;
            DesignRoomGetPreviousRingIndex();
            slot = gActiveMap->ringIndex;
            for (i = 0; i < visible; i++)
            {
                e = &gDesignRing[slot];
                slot++;
                e->flags |= 9;
                e->flags &= ~0x30;
                if (slot >= count)
                    slot -= count;
            }
        }
        break;

    case 4:
        done = 0;
        slot = gActiveMap->ringIndex;
        for (i = 0; i < visible; i++)
        {
            e = &gDesignRing[slot];
            e->y -= 0x300;
            if ((e->y >> 8) < 0x70)
                e->y = 0x7000;
            frame = e->targetX - (e->x >> 8);
            if (frame < 0)
            {
                e->xVelocity -= 0x200;
                e->x += e->xVelocity;
                if ((e->x >> 8) <= e->targetX)
                    e->x = e->targetX << 8;
            }
            else if (frame > 0)
            {
                e->xVelocity += 0x200;
                e->x += e->xVelocity;
                if ((e->x >> 8) >= e->targetX)
                    e->x = e->targetX << 8;
            }
            else
            {
                done++;
                if (done >= visible)
                    gActiveMap->state = 0x32;
            }
            slot++;
            if (slot >= count)
                slot -= count;
        }
        break;

    case 0x32:
        e = &gDesignRing[DesignRoomGetPreviousRingIndex()];
        e->x = gUnknown_084886F8[gActiveMap->editMode]
                                [gActiveMap->editMode == 0 ? 9 : 7] << 8;
        gActiveMap->flags |= 0x10;
        gActiveMap->state = 0x33;
        if (gActiveMap->editMode == 0)
            DesignRoomLoadTerrainNamePalettes();
        /* fall through */
    case 0x33:
        if (gpKeySt->pressed & SELECT_BUTTON)
        {
            gActiveMap->state = gActiveMap->editMode == 0 ? 0x5A : 0x50;
            break;
        }
        if (gpKeySt->pressed & L_BUTTON)
        {
            if (gActiveMap->editMode == 0)
                gActiveMap->state = 0x5A;
            break;
        }
        if (gpKeySt->pressed & R_BUTTON)
        {
            if (gActiveMap->editMode == 1)
                gActiveMap->state = 0x50;
            break;
        }

        if (gpKeySt->held & DPAD_RIGHT)
        {
            gActiveMap->state = 0x34;
            slot = visible;
            if (gActiveMap->editMode == 0)
            {
                slot += gActiveMap->terrainListIndex;
                if (slot > 16)
                    slot -= 17;
            }
            else
            {
                slot += gActiveMap->unitListIndex;
                if (slot > 19)
                    slot -= 20;
            }
            i = DesignRoomGetPreviousRingIndex();
            gDesignRing[i].itemId = gUnknown_0200B224[slot].unk00;
            gDesignRing[i].spriteSlot = i;
            gDesignRing[i].flags |= 9;
            PlayMusicOrSfx2(100);
        }
        else if (gpKeySt->held & DPAD_LEFT)
        {
            gActiveMap->state = 0x35;
            if (gActiveMap->editMode == 0)
            {
                slot = gActiveMap->terrainListIndex - 1;
                if (slot < 0)
                    slot += 17;
            }
            else
            {
                slot = gActiveMap->unitListIndex - 1;
                if (slot < 0)
                    slot += 20;
            }
            i = DesignRoomGetPreviousRingIndex();
            gDesignRing[i].itemId = gUnknown_0200B224[slot].unk00;
            gDesignRing[i].spriteSlot = i;
            gDesignRing[i].flags |= 9;
            PlayMusicOrSfx2(100);
        }

        if (gActiveMap->editMode == 0)
            slot = gActiveMap->ringIndex + 4;
        else
            slot = gActiveMap->ringIndex + 3;

        /* agbcc kept the dead value above; both arms are identical and the
         * later jump pass merges them. */
        if (slot)
            key = gpKeySt;
        else
            key = gpKeySt;

        if ((key->repeated | key->pressed) & (DPAD_UP | DPAD_DOWN))
            gActiveMap->state = 0x28;
        else if (key->pressed & A_BUTTON)
        {
            gActiveMap->flags &= ~0x20;
            gActiveMap->state = 0x3C;
        }
        else if (key->pressed & B_BUTTON)
        {
            gActiveMap->flags |= 0x20;
            gActiveMap->state = 0x3C;
        }
        break;
    }

    switch (gActiveMap->state)
    {
    case 0x34:
        i = DesignRoomGetPreviousRingIndex();
        e = &gDesignRing[i];
        e->targetX = gUnknown_084886F8[gActiveMap->editMode]
                                      [gActiveMap->editMode == 0 ? 9 : 7];
        e->x = e->targetX << 8;
        e->y = 0x7000;
        slot = gActiveMap->ringIndex;
        for (i = 0; i < count; i++)
        {
            e = &gDesignRing[slot];
            e->targetX += (s8)gUnknown_08488748[gActiveMap->editMode][i];
            e->xVelocity = 0;
            slot++;
            if (slot >= count)
                slot -= count;
        }
        gActiveMap->state = 0x36;
        if (gActiveMap->editMode == 0)
        {
            gActiveMap->terrainListIndex++;
            if (gActiveMap->terrainListIndex > 16)
                gActiveMap->terrainListIndex -= 17;
        }
        else
        {
            gActiveMap->unitListIndex++;
            if (gActiveMap->unitListIndex > 19)
                gActiveMap->unitListIndex -= 20;
        }
        break;

    case 0x35:
        i = DesignRoomGetPreviousRingIndex();
        e = &gDesignRing[i];
        e->targetX = gUnknown_084886F8[gActiveMap->editMode][0]
                   - (gActiveMap->editMode == 0 ? 0x18 : 0x21);
        e->x = e->targetX << 8;
        e->y = 0x7000;
        slot = gActiveMap->ringIndex;
        for (i = 0; i < count; i++)
        {
            e = &gDesignRing[slot];
            e->targetX += gUnknown_0848875C[gActiveMap->editMode][i];
            e->xVelocity = 0;
            slot++;
            if (slot >= count)
                slot -= count;
        }
        gActiveMap->state = 0x37;
        if (gActiveMap->editMode == 0)
        {
            gActiveMap->terrainListIndex--;
            if (gActiveMap->terrainListIndex < 0)
                gActiveMap->terrainListIndex += 17;
        }
        else
        {
            gActiveMap->unitListIndex--;
            if (gActiveMap->unitListIndex < 0)
                gActiveMap->unitListIndex += 20;
        }
        break;
    }

    switch (gActiveMap->state)
    {
    case 0x36:
    case 0x37:
        done = 0;
        slot = gActiveMap->ringIndex;
        for (i = 0; i < count; i++)
        {
            e = &gDesignRing[slot];
            frame = e->targetX - (e->x >> 8);
            if (frame < 0)
            {
                frame = (-0x600 - e->xVelocity) >> 2;
                e->xVelocity += frame;
                if (e->xVelocity <= -0x480)
                    e->xVelocity = -0x480;
                e->x += e->xVelocity;
                if ((e->x >> 8) <= e->targetX)
                    e->x = e->targetX << 8;
            }
            else if (frame > 0)
            {
                frame = (0x600 - e->xVelocity) >> 2;
                e->xVelocity += frame;
                if (e->xVelocity >= 0x480)
                    e->xVelocity = 0x480;
                e->x += e->xVelocity;
                if ((e->x >> 8) >= e->targetX)
                    e->x = e->targetX << 8;
            }
            else
            {
                done++;
                if (done >= count)
                {
                    if (gActiveMap->state == 0x36)
                    {
                        gActiveMap->ringIndex++;
                        if (gActiveMap->ringIndex > visible)
                            gActiveMap->ringIndex = 0;
                    }
                    if (gActiveMap->state == 0x37)
                    {
                        gActiveMap->ringIndex--;
                        if (gActiveMap->ringIndex < 0)
                            gActiveMap->ringIndex = visible;
                    }
                    gActiveMap->state = 0x32;
                }
            }
            slot++;
            if (slot >= count)
                slot -= count;
        }
        break;

    case 0x3C:
        PlayMusicOrSfx2((gActiveMap->flags & 0x20) ? 0x66 : 0x65);
        for (i = 0; i < count; i++)
            gDesignRing[i].flags &= ~1;
        gActiveMap->flags &= ~0x10;
        sub_08007B74();
        gActiveMap->stateTimer = 0x1E;
        gActiveMap->state = 0x41;
        gActiveMap->flags &= ~0x40;
        /* fall through */
    case 0x41:
        gUnknown_03002EFC += 6;
        if (gUnknown_03002EFC >= 0xA0)
        {
            gUnknown_03002EFC = 0xA0;
            gActiveMap->state = 0x46;
            DesignRoomShowTilePanel();
            DesignRoomShowCoordBox();
            if (gActiveMap->flags & 0x20)
            {
                if (gActiveMap->flags & 1)
                {
                    gActiveMap->selectedTerrain = gActiveMap->savedTerrain;
                    gActiveMap->terrainListIndex = gActiveMap->savedTerrainListIndex;
                    gActiveMap->propertyArmy = gActiveMap->savedPropertyArmy;
                }
                if (gActiveMap->flags & 2)
                {
                    gActiveMap->cursorUnit = gActiveMap->savedUnit;
                    gActiveMap->unitListIndex = gActiveMap->savedUnitListIndex;
                    gActiveMap->unitArmy = gActiveMap->savedUnitArmy;
                }
            }
            else if (gActiveMap->editMode == 0)
            {
                slot = gActiveMap->ringIndex + 4;
                if (slot > 9)
                    slot = gActiveMap->ringIndex - 6;
                gActiveMap->selectedTerrain = gDesignRing[slot].itemId;
                if (gActiveMap->flags & 2)
                {
                    gActiveMap->cursorUnit = gActiveMap->savedUnit;
                    gActiveMap->unitListIndex = gActiveMap->savedUnitListIndex;
                    gActiveMap->unitArmy = gActiveMap->savedUnitArmy;
                }
            }
            else
            {
                slot = gActiveMap->ringIndex + 3;
                if (slot > 7)
                    slot = gActiveMap->ringIndex - 5;
                {
                    /* Three locals are load-bearing. The (u16) copy keeps CSE
                     * from folding x into v, so the ROM's `adds r1,r0,#0` and
                     * compare on the copy survive. y is set twice, so regmove
                     * cannot fold the AND into v in place. */
                    int v = gDesignRing[slot].itemId;
                    int x;
                    int y;

                    x = (u16)v;
                    if (x == 0x19)
                        gActiveMap->cursorUnit = x;
                    else
                    {
                        y = v & 0x3F;
                        y |= (gActiveMap->unitArmy - 1) << 6;
                        gActiveMap->cursorUnit = y;
                    }
                }
                if (gActiveMap->flags & 1)
                {
                    gActiveMap->selectedTerrain = gActiveMap->savedTerrain;
                    gActiveMap->terrainListIndex = gActiveMap->savedTerrainListIndex;
                    gActiveMap->propertyArmy = gActiveMap->savedPropertyArmy;
                }
            }
            sub_08002E3C();
        }
        break;
    }

    switch (gActiveMap->state)
    {
    case 0x46:
        if (gActiveMap->tilePanelY <= 0x6A)
            DesignRoomSetMode(1);
        break;

    case 0x50:
        sub_08007B74();
        gActiveMap->ringIndex = 0;
        for (i = 0; i < 10; i++)
            gDesignRing[i].flags |= 0x30;
        gActiveMap->selectionAffineScale = 0x100;
        gActiveMap->state++;
        break;

    case 0x51:
        frame = gActiveMap->selectionAffineScale;
        frame -= 0x40;
        if (frame < 0x10)
            frame = 0x10;
        gActiveMap->selectionAffineScale = frame;
        SET_RING_AFFINE(0x100, frame);
        if (frame <= 0x10)
        {
            for (i = 0; i < 10; i++)
                gDesignRing[i].flags &= ~0x31;
            gActiveMap->state = 0x64;
            gActiveMap->editMode = 0;
        }
        break;

    case 0x5A:
        sub_08007B74();
        gActiveMap->ringIndex = 0;
        for (i = 0; i < 10; i++)
            gDesignRing[i].flags |= 0x30;
        gActiveMap->selectionAffineScale = 0x100;
        gActiveMap->state++;
        break;

    case 0x5B:
        frame = gActiveMap->selectionAffineScale;
        frame -= 0x40;
        if (frame < 0x10)
            frame = 0x10;
        gActiveMap->selectionAffineScale = frame;
        SET_RING_AFFINE(0x100, frame);
        if (frame <= 0x10)
        {
            for (i = 0; i < 10; i++)
                gDesignRing[i].flags &= ~0x31;
            gActiveMap->state = 0x64;
            gActiveMap->editMode = 1;
        }
        break;
    }

    switch (gActiveMap->state)
    {
    case 0x64:
        DesignRoomBuildItemList(gActiveMap->editMode,
                     gActiveMap->editMode == 0 ? gActiveMap->propertyArmy
                                                : gActiveMap->unitArmy);
        DesignRoomBuildRing(gActiveMap->editMode, -1);
        DesignRoomSaveSelection();
        DesignRoomLoadTerrainNamePalettes();
        gActiveMap->state++;
        gActiveMap->stateTimer = 10;
        break;

    case 0x65:
        if (gActiveMap->stateTimer-- <= 0)
            gActiveMap->state++;
        break;

    case 0x66:
        PlayMusicOrSfx2(0x65);
        gActiveMap->state = 4;
        DesignRoomBuildItemList(gActiveMap->editMode,
                     gActiveMap->editMode == 0 ? gActiveMap->propertyArmy
                                                : gActiveMap->unitArmy);
        DesignRoomGetPreviousRingIndex();
        slot = gActiveMap->ringIndex;
        for (i = 0; i < count - 1; i++)
        {
            e = &gDesignRing[slot];
            slot++;
            e->flags = (e->flags | 1) & ~0x30;
            if (slot >= count)
                slot -= count;
        }
        break;
    }

    switch (gActiveMap->state)
    {
    case 0x28:
    {
        int kind;

        if (gActiveMap->editMode == 0)
        {
            slot = gActiveMap->ringIndex + 4;
            if (slot > 9)
                slot = gActiveMap->ringIndex - 6;
            e = &gDesignRing[slot];
            if (sub_0800C7E8(e->itemId) == 0 || e->itemId == 0x10 || e->itemId == 0x11)
            {
                gActiveMap->state = 0x33;
                break;
            }
            kind = gActiveMap->propertyArmy;
            if (gpKeySt->repeated & DPAD_UP)
            {
                kind++;
                if (kind > 4)
                    kind = 0;
                PlayMusicOrSfx2(100);
            }
            else if (gpKeySt->repeated & DPAD_DOWN)
            {
                kind--;
                if (kind < 0)
                    kind = 4;
                PlayMusicOrSfx2(100);
            }
            gActiveMap->selectionAnimKind = kind;
            for (i = 0; i < 10; i++)
            {
                if (sub_0800C7E8(gDesignRing[i].itemId) && gDesignRing[i].itemId != 0x10
                    && gDesignRing[i].itemId != 0x11)
                    gDesignRing[i].flags |= 0x10;
            }
        }
        else
        {
            slot = gActiveMap->ringIndex + 3;
            if (slot > 7)
                slot = gActiveMap->ringIndex - 5;
            if (gDesignRing[slot].itemId == 0x19)
            {
                gActiveMap->state = 0x33;
                break;
            }
            kind = gActiveMap->unitArmy;
            if (gpKeySt->repeated & DPAD_UP)
            {
                kind++;
                if (kind > 4)
                    kind = 1;
                PlayMusicOrSfx2(100);
            }
            else if (gpKeySt->repeated & DPAD_DOWN)
            {
                kind--;
                if (kind < 1)
                    kind = 4;
                PlayMusicOrSfx2(100);
            }
            gActiveMap->selectionAnimKind = kind;
            for (i = 0; i < 10; i++)
            {
                if (gDesignRing[i].itemId != 0x19)
                    gDesignRing[i].flags |= 0x10;
            }
        }
        gActiveMap->selectionAnimFrame = 0;
        gActiveMap->selectionAffineScale = gUnknown_084886DC[gActiveMap->selectionAnimFrame++];
        SET_RING_AFFINE(0x100, 0x100);
        gActiveMap->state = 0x2A;
        break;
    }

    case 0x2A:
        frame = gUnknown_084886DC[gActiveMap->selectionAnimFrame++];
        gActiveMap->selectionAffineScale = frame;
        if (frame == 0)
        {
            gActiveMap->state++;
            if (gActiveMap->editMode == 0)
            {
                for (i = 0; i < 10; i++)
                    if (sub_0800C7E8(gDesignRing[i].itemId) && gDesignRing[i].itemId != 0x10
                        && gDesignRing[i].itemId != 0x11)
                        gDesignRing[i].flags |= 0x40;
            }
            else
            {
                for (i = 0; i < 10; i++)
                    if (gDesignRing[i].itemId != 0x19)
                        gDesignRing[i].flags |= 0x40;
            }
        }
        else
        {
            if (gActiveMap->editMode == 0)
            {
                for (i = 0; i < 10; i++)
                    if (sub_0800C7E8(gDesignRing[i].itemId) && gDesignRing[i].itemId != 0x10
                        && gDesignRing[i].itemId != 0x11)
                        gDesignRing[i].flags &= ~0x40;
            }
            else
            {
                for (i = 0; i < 10; i++)
                    if (gDesignRing[i].itemId != 0x19)
                        gDesignRing[i].flags &= ~0x40;
            }
        }
        gActiveMap->selectionAffineScale = frame;
        if (frame != 0)
            SET_RING_AFFINE(frame, 0x100);
        break;

    case 0x2B:
        frame = gUnknown_084886DC[gActiveMap->selectionAnimFrame++];
        gActiveMap->selectionAffineScale = frame;
        if (frame == 0)
            break;
        gActiveMap->state++;
        if (gActiveMap->editMode == 0)
        {
            for (i = 0; i < 10; i++)
                if (sub_0800C7E8(gDesignRing[i].itemId) && gDesignRing[i].itemId != 0x10
                    && gDesignRing[i].itemId != 0x11)
                    gDesignRing[i].flags &= ~0x40;
        }
        else
        {
            for (i = 0; i < 10; i++)
                if (gDesignRing[i].itemId != 0x19)
                    gDesignRing[i].flags &= ~0x40;
        }
        gActiveMap->selectionAffineScale = frame;
        SET_RING_AFFINE(frame, 0x100);
        break;

    case 0x2C:
        frame = gUnknown_084886DC[gActiveMap->selectionAnimFrame++];
        gActiveMap->selectionAffineScale = frame;
        if (frame == -1)
        {
            gActiveMap->state++;
            frame = 0x100;
        }
        if (frame < 0)
            goto mark;
        gActiveMap->state++;
        if (gActiveMap->editMode == 0)
        {
            slot = gActiveMap->ringIndex + 4;
            if (slot > 9)
                slot = gActiveMap->ringIndex - 6;
            sub_080077EC(gUnknown_0200B0D0[slot].itemId, gActiveMap->selectionAnimKind);
            sub_08007B74();
        }
        else
        {
            DesignRoomSetUnitArmy(gActiveMap->selectionAnimKind);
            sub_08007B74();
        }
        if (frame != 0)
            goto mark;
        if (gActiveMap->editMode == 0)
        {
            for (i = 0; i < 10; i++)
                if (sub_0800C7E8(gDesignRing[i].itemId) && gDesignRing[i].itemId != 0x10
                    && gDesignRing[i].itemId != 0x11)
                    gDesignRing[i].flags |= 0x40;
        }
        else
        {
            for (i = 0; i < 10; i++)
                if (gDesignRing[i].itemId != 0x19)
                    gDesignRing[i].flags |= 0x40;
        }
        goto scale;
    mark:
        if (gActiveMap->editMode == 0)
        {
            slot = gActiveMap->ringIndex;
            for (i = 0; i < 10; i++)
            {
                if (sub_0800C7E8(gDesignRing[slot].itemId) && gDesignRing[slot].itemId != 0x10
                    && gDesignRing[i].itemId != 0x11)
                    gDesignRing[slot].flags = (gDesignRing[slot].flags & ~0x40) | 8;
                slot++;
                if (slot > visible)
                    slot = 0;
            }
        }
        else
        {
            slot = gActiveMap->ringIndex;
            for (i = 0; i < 10; i++)
            {
                if (gDesignRing[slot].itemId != 0x19)
                {
                    gDesignRing[slot].flags &= ~0x40;
                    if ((gDesignRing[slot].itemId & 0x3F) == 1
                        || (gDesignRing[slot].itemId & 0x3F) == 2)
                        gDesignRing[slot].flags |= 8;
                }
                slot++;
                if (slot > visible)
                    slot = 0;
            }
        }
    scale:
        gActiveMap->selectionAffineScale = frame;
        SET_RING_AFFINE(frame, 0x100);
        break;

    case 0x2D:
        gActiveMap->state++;
        if (gActiveMap->editMode == 0)
        {
            slot = gActiveMap->ringIndex;
            for (i = 0; i < 10; i++)
            {
                if (sub_0800C7E8(gDesignRing[slot].itemId) && gDesignRing[slot].itemId != 0x10
                    && gDesignRing[i].itemId != 0x11)
                    gDesignRing[slot].flags = (gDesignRing[slot].flags & ~0x40) | 8;
                slot++;
                if (slot > visible)
                    slot = 0;
            }
        }
        else
        {
            slot = gActiveMap->ringIndex;
            for (i = 0; i < 10; i++)
            {
                if (gDesignRing[slot].itemId != 0x19)
                {
                    gDesignRing[slot].flags &= ~0x40;
                    if ((gDesignRing[slot].itemId & 0x3F) == 1
                        || (gDesignRing[slot].itemId & 0x3F) == 2)
                        gDesignRing[slot].flags |= 8;
                }
                slot++;
                if (slot > visible)
                    slot = 0;
            }
        }
        /* fall through */
    case 0x2E:
        gActiveMap->selectionAnimFrame++;
        frame = gUnknown_084886DC[gActiveMap->selectionAnimFrame];
        if (frame == -1)
        {
            frame = 0x100;
            gActiveMap->state = 0x30;
            gActiveMap->stateTimer = 2;
        }
        gActiveMap->selectionAffineScale = frame;
        SET_RING_AFFINE(frame, 0x100);
        break;

    case 0x30:
        if (gActiveMap->stateTimer-- <= 0)
        {
            for (i = 0; i < 10; i++)
                gDesignRing[i].flags &= ~0x70;
            gActiveMap->state = 0x33;
        }
        break;
    }
}
asm(".global sub_08005F4C\n.thumb_set sub_08005F4C, DesignRoomMode_Ring\n");
