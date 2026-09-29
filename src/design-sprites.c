#include "global.h"
#include "hardware.h"
#include "map.h"

void sub_080032EC(int a1, int a2, int a3)
{
    u16 kind[4];
    s8 xo[4];
    s8 yo[4];
    int o1;
    int o2;
    int r;
    int s;
    int c3;

    sub_0808B6E8(kind, gUnknown_0808D760, 8);
    sub_0808B6E8(xo, gUnknown_0808D768, 4);
    sub_0808B6E8(yo, gUnknown_0808D76C, 4);

    switch (gActiveMap->armyPanelAffineState[a1])
    {
    case 0:
        if (GetArmyHq(kind[a1], &o1, &o2) != 0)
        {
            gActiveMap->armyPanelAffineState[a1] = 0xA;
            gActiveMap->armyPanelScale[a1] = 5;
            gActiveMap->armyPanelAngle[a1] = 0;
        }
        break;
    case 0xA:
        r = gActiveMap->armyPanelAngle[a1] - 0x20;
        if (r < 0)
            r = gActiveMap->armyPanelAngle[a1] + 0x148;
        gActiveMap->armyPanelAngle[a1] = r;
        s = gActiveMap->armyPanelScale[a1] + 0xA;
        if (s > 0xFF)
        {
            s = 0x100;
            gActiveMap->armyPanelAffineState[a1] = 0x14;
            gActiveMap->armyPanelAngle[a1] = 0;
        }
        gActiveMap->armyPanelScale[a1] = s;
        break;
    case 0x14:
        if (GetArmyHq(kind[a1], &o1, &o2) == 0)
        {
            gActiveMap->armyPanelAffineState[a1] = 0x1E;
            gActiveMap->armyPanelAngle[a1] = 0;
            gActiveMap->armyPanelScale[a1] = 0x100;
        }
        break;
    case 0x1E:
        r = gActiveMap->armyPanelAngle[a1] - 0x20;
        if (r < 0)
            r = gActiveMap->armyPanelAngle[a1] + 0x148;
        gActiveMap->armyPanelAngle[a1] = r;
        s = gActiveMap->armyPanelScale[a1] - 0x20;
        if (s <= 0)
        {
            s = 1;
            gActiveMap->armyPanelAffineState[a1] = 0;
        }
        gActiveMap->armyPanelScale[a1] = s;
        break;
    }

    if (gActiveMap->armyPanelAffineState[a1] != 0)
    {
        SetObjAffine(a1 + 1,
            Div(COS_Q12(gActiveMap->armyPanelAngle[a1]) << 4,
                gActiveMap->armyPanelScale[a1] != 0 ? gActiveMap->armyPanelScale[a1] : 2),
            Div(-SIN_Q12(gActiveMap->armyPanelAngle[a1]) << 4,
                gActiveMap->armyPanelScale[a1] != 0 ? gActiveMap->armyPanelScale[a1] : 2),
            Div(SIN_Q12(gActiveMap->armyPanelAngle[a1]) << 4,
                gActiveMap->armyPanelScale[a1] != 0 ? gActiveMap->armyPanelScale[a1] : 2),
            Div(COS_Q12(gActiveMap->armyPanelAngle[a1]) << 4,
                gActiveMap->armyPanelScale[a1] != 0 ? gActiveMap->armyPanelScale[a1] : 2));

        c3 = 0x300;
        DrawOamObject(a1 + 0x3E,
            ((a2 + xo[a1] - 7) & 0x1FF) | ((a1 + 1) << 9),
            ((a3 + yo[a1] - 6) & 0xFF) | c3,
            0, 0);
    }
}

void sub_080035C8(void)
{
    u16 v[4];
    u16 *p;
    sub_0808B6E8(v, gUnknown_0808D774, 8);
    asm volatile(
        "movs r0, #15\n\t"
        "mov r8, r0\n\t"
        "movs r6, #0\n\t"
        "ldr r1, =gUnknown_08485CC8\n\t"
        "mov sl, r1"
        : : : "r0", "r1", "r6", "r8", "r10", "memory");
    p = v;
    asm volatile(
        "ldr r0, =0x1ff\n\t"
        "mov r9, r0\n"
        "1:\n\t"
        "ldrh r0, [r7]\n\t"
        "add r7, #2\n\t"
        "mov r4, r8\n\t"
        "add r4, #1\n\t"
        "movs r1, #128\n\t"
        "lsl r1, r1, #3\n\t"
        "orr r4, r1\n\t"
        "add r6, #1\n\t"
        "lsl r1, r6, #2\n\t"
        "add r1, sl\n\t"
        "ldr r5, [r1]\n\t"
        "bl sub_08001D04\n\t"
        "mov r3, r0\n\t"
        "lsl r3, r3, #12\n\t"
        "movs r0, #23\n\t"
        "mov r1, r9\n\t"
        "and r0, r1\n\t"
        "mov r1, r4\n\t"
        "mov r2, r5\n\t"
        "bl PutOamHi\n\t"
        "movs r0, #17\n\t"
        "add r8, r0\n\t"
        "cmp r6, #3\n\t"
        "ble 1b"
        : "+r"(p) : : "r0", "r1", "r2", "r3", "r4", "r5", "r6",
                "r8", "r9", "r10", "memory");
}
