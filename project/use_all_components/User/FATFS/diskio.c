/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"         /* Obtains integer types */
#include "diskio.h"     /* Declarations of disk functions */
#include "spi_flash.h"
#include "utils.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM     0   /* Example: Map Ramdisk to physical drive 0 */
#define DEV_SPI_FLASH       1   /* Example: Map MMC/SD card to physical drive 1 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;

    switch (pdrv)
    {
    case DEV_RAM :
        break;

    case DEV_SPI_FLASH :
        if (sFLASH_ID == SPI_FLASH_ReadDeviceID())
        {
            stat = 0;
        }
        else
        {
            stat = STA_NOINIT;
        }
        break;

    default:
        stat = STA_NOINIT;
    }
    return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
    BYTE pdrv               /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;

    switch (pdrv)
    {
    case DEV_RAM :
        stat = STA_NOINIT;
        break;

    case DEV_SPI_FLASH :

        SPI_FLASH_Configu();

        Delay_us(500);

        SPI_FLASH_WakeUp();

        stat = disk_status(pdrv);

        break;

    default:
        stat = STA_NOINIT;

    }
    return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    LBA_t sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res;

    switch (pdrv)
    {
    case DEV_RAM :
        res = RES_NOTRDY;
        break;

    case DEV_SPI_FLASH :
        // 将前2MB的空间保留，只使用后6MB的空间
        sector += 512;

        SPI_FLASH_BufferRead(buff, sector << 12, count << 12);

        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
    }

    return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    LBA_t sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DRESULT res;
    LBA_t writeAddr;
    int i;

    if (!count)
    {
        return RES_PARERR;
    }

    switch (pdrv)
    {
    case DEV_RAM :
        res = RES_NOTRDY;
        break;

    case DEV_SPI_FLASH:
        sector += 512;
        writeAddr = sector << 12;

        for (i = 0; i < count; i++)
        {
            SPI_FLASH_EraseSector((sector + i) << 12);
        }

        SPI_FLASH_BufferWrite((u8 *)buff, writeAddr, count << 12);
        res = RES_OK;
        break;

    default:
        res = RES_PARERR;

    }

    return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;

    switch (pdrv)
    {
    case DEV_RAM :
        res = RES_NOTRDY;
        break;

    case DEV_SPI_FLASH :
        switch (cmd)
        {
        case GET_SECTOR_COUNT:
            *(DWORD *)buff = 1536; // 6MB
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            *(WORD *)buff = 4096;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(DWORD *)buff = 1;
            res = RES_OK;
            break;
				
				case CTRL_SYNC:
					res = RES_OK;
					break;

        default:
            res = RES_PARERR;
        }
        break;

    default:
        res = RES_PARERR;
    }

    return res;
}


__weak DWORD get_fattime(void)
{
    /* 返回当前时间戳 */
    return ((DWORD)(2015 - 1980) << 25)     /* Year 2015 */
           | ((DWORD)1 << 21)              /* Month 1 */
           | ((DWORD)1 << 16)              /* Mday 1 */
           | ((DWORD)0 << 11)              /* Hour 0 */
           | ((DWORD)0 << 5)                 /* Min 0 */
           | ((DWORD)0 >> 1);              /* Sec 0 */
}
