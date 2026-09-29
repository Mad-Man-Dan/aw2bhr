#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08062FF4.
 * sub_08062FF4 @ 0x08062FF4
 */

#include "hardware.h"
/* The AGB SDK's MultiBootParam, as used by MultiBootMain. */
struct MbParam
{
    u32 system_work[5];
    u8 handshake_data;
    u8 padding;
    u16 handshake_timeout;
    u8 probe_count;
    u8 client_data[3];
    u8 palette_data;
    u8 response_bit;
    u8 client_bit;
    u8 reserved1;
    const u8 *boot_srcp;
    const u8 *boot_endp;
    u8 *masterp;
    u8 *reserved2[3];
    u32 system_work2[4];
    u8 sendflag;
    u8 probe_target_bit;
    u8 check_wait;
    u8 server_type;
};
#define REG_SIOMULTI(i) (*(vu16 *)(REG_BASE + REG_OFFSET_SIOMULTI0 + (i) * 2))
#define MultiBootInit(mp) sub_08062FB8((struct Unk08062FB8 *)(mp))
#define MultiBootSend(mp, d) sub_080633E4((struct Unk08062FB8 *)(mp), d)
#define MultiBootCheckComplete(mp) sub_08063518((struct Unk08062FB8 *)(mp))
#define MultiBootHandShake(mp) sub_08063528((struct Unk08062FB8 *)(mp))
#define MultiBootStartProbe(mp) sub_08063430((struct Unk08062FB8 *)(mp))
#define MultiBootWaitSendDone sub_0806362C
#define gUnknown_03001864 gUnknown_030005EC
void sub_08063430(struct Unk08062FB8 *);
int sub_08063528(struct Unk08062FB8 *);

/* The SDK's MultiBootMain: polls the link port once per call and steps the
 * multiboot state machine (probe clients, send header, send handshake data,
 * start the transfer). Returns 0 or an error code. The source is the SDK's
 * own, kept as published: its for-loops and gotos are what produce the
 * loop shapes in the ROM. */
int MultiBootMain(struct MbParam *mp)
{
    int i;
    int j;
    int k;

    if (MultiBootCheckComplete(mp))
    {
        return 0;
    }

    if (mp->check_wait > 15)
    {
        mp->check_wait--;
        return 0;
    }

output_burst:
    if (mp->sendflag)
    {
        mp->sendflag = 0;

        i = REG_SIOCNT & 0xfc;
        if (i != 8)
        {
            MultiBootInit(mp);
            return i ^ 8;
        }
    }

    if (mp->probe_count >= 0xe0)
    {
        i = MultiBootHandShake(mp);
        if (i)
        {
            return i;
        }

        if (mp->server_type == 1 && mp->probe_count > 0xe1 && MultiBootCheckComplete(mp) == 0)
        {
            MultiBootWaitSendDone();
            goto output_burst;
        }

        if (MultiBootCheckComplete(mp) == 0)
        {
            if (mp->handshake_timeout == 0)
            {
                MultiBootInit(mp);
                return 0x71;
            }
            mp->handshake_timeout--;
        }

        return 0;
    }

    switch (mp->probe_count)
    {
        case 0:
            k = 0x0e;
            for (i = 3; i != 0; i--)
            {
                if (REG_SIOMULTI(i) != 0xffff)
                {
                    break;
                }
                k >>= 1;
            }

            k &= 0x0e;
            mp->response_bit = k;

            for (i = 3; i != 0; i--)
            {
                j = REG_SIOMULTI(i);
                if (mp->client_bit & (1 << i))
                {
                    if (j != ((0x72 << 8) | (1 << i)))
                    {
                        k = 0;
                        break;
                    }
                }
            }

            mp->client_bit &= k;

            if (k == 0)
            {
                mp->check_wait = 15;
            }

            if (mp->check_wait)
            {
                mp->check_wait--;
            }
            else
            {
                if (mp->response_bit != mp->client_bit)
                {
                    MultiBootStartProbe(mp);
                    goto case_1;
                }
            }

        output_master_info:
            return MultiBootSend(mp, (0x62 << 8) | mp->client_bit);

        case_1:
        case 1:
            mp->probe_target_bit = 0;
            for (i = 3; i != 0; i--)
            {
                j = REG_SIOMULTI(i);
                if ((j >> 8) == 0x72)
                {
                    gUnknown_03001864[i - 1] = j;
                    j &= 0xff;
                    if (j == (1 << i))
                    {
                        mp->probe_target_bit |= j;
                    }
                }
            }

            if (mp->response_bit != mp->probe_target_bit)
            {
                goto output_master_info;
            }

            mp->probe_count = 2;
            return MultiBootSend(mp, (0x61 << 8) | mp->probe_target_bit);

        case 2:
            for (i = 3; i != 0; i--)
            {
                if (mp->probe_target_bit & (1 << i))
                {
                    j = REG_SIOMULTI(i);
                    if (j != gUnknown_03001864[i - 1])
                    {
                        mp->probe_target_bit ^= 1 << i;
                    }
                }
            }
            goto output_header;

        case 0xd0:
            k = 1;
            for (i = 3; i != 0; i--)
            {
                j = REG_SIOMULTI(i);
                mp->client_data[i - 1] = j;
                if (mp->probe_target_bit & (1 << i))
                {
                    if ((j >> 8) != 0x72 && (j >> 8) != 0x73)
                    {
                        MultiBootInit(mp);
                        return 0x60;
                    }
                    if (j == gUnknown_03001864[i - 1])
                    {
                        k = 0;
                    }
                }
            }

            if (k == 0)
            {
                return MultiBootSend(mp, (0x63 << 8) | mp->palette_data);
            }

            mp->probe_count = 0xd1;

            k = 0x11;
            for (i = 3; i != 0; i--)
            {
                k += mp->client_data[i - 1];
            }
            mp->handshake_data = k;
            return MultiBootSend(mp, (0x64 << 8) | (k & 0xff));

        case 0xd1:
            for (i = 3; i != 0; i--)
            {
                j = REG_SIOMULTI(i);
                if (mp->probe_target_bit & (1 << i))
                {
                    if ((j >> 8) != 0x73)
                    {
                        MultiBootInit(mp);
                        return 0x60;
                    }
                }
            }

            i = MultiBoot((struct Unk08062FB8 *)mp);

            if (i == 0)
            {
                mp->probe_count = 0xe0;
                mp->handshake_timeout = 400;
                return 0;
            }
            MultiBootInit(mp);
            mp->check_wait = 15 * 2;
            return 0x70;

        default:
            for (i = 3; i != 0; i--)
            {
                if (mp->probe_target_bit & (1 << i))
                {
                    j = REG_SIOMULTI(i);
                    if ((j >> 8) != (0x61 + 1 - (mp->probe_count >> 1)) ||
                        ((j & 0xff) != (1 << i)))
                    {
                        mp->probe_target_bit ^= 1 << i;
                    }
                }
            }

            if (mp->probe_count == 0xc4)
            {
                mp->client_bit = mp->probe_target_bit & 0x0e;
                mp->probe_count = 0;
                goto output_master_info;
            }

        output_header:
            if (mp->probe_target_bit == 0)
            {
                MultiBootInit(mp);
                return 0x50;
            }

            mp->probe_count += 2;
            if (mp->probe_count == 0xc4)
            {
                goto output_master_info;
            }
            i = MultiBootSend(mp, (mp->masterp[mp->probe_count - 4 + 1] << 8) | mp->masterp[mp->probe_count - 4]);

            if (i)
            {
                return i;
            }
            if (mp->server_type == 1)
            {
                MultiBootWaitSendDone();
                goto output_burst;
            }
            return 0;
    }
}
asm(".global sub_08062FF4\n.thumb_set sub_08062FF4, MultiBootMain\n");
