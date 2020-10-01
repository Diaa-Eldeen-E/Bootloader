
/* Memory layout for the bootloader */

MEMORY
{
    /*FLASH (RX) : ORIGIN = 0x00000000, LENGTH = 0x00100000*/

    BL_ROM (RX) : ORIGIN = 0x00000000, LENGTH = 64K
    APP0_ROM (RX) : ORIGIN = 0x10000, LENGTH = 256K
    APP1_ROM (RX) : ORIGIN = 0x50000, LENGTH = 256K
    SRAM (RWX)  : ORIGIN = 0x20000000, LENGTH = 0x00040000
}


SRAM_END = 0x20040000;
__blrom_start = ORIGIN(APP0_ROM);
__blrom_size = LENGTH(APP0_ROM);
__app0rom_start = ORIGIN(APP0_ROM);
__app0rom_size = LENGTH(APP0_ROM);
__app1rom_start = ORIGIN(APP1_ROM);
__app1rom_size = LENGTH(APP1_ROM);
