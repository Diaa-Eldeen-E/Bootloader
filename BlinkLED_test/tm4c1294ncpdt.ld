
INCLUDE "../memory_map.lds" 

/*
MEMORY
{
    FLASH (RX) : ORIGIN = 0x00000000, LENGTH = 0x00100000
    SRAM (RWX) : ORIGIN = 0x20000000, LENGTH = 0x00040000
}


SRAM_END = 0x20040000;
*/

REGION_ALIAS("FLASH", APP0_ROM);


SECTIONS
{
    .intvecs :
    {
	    *(.intvecs)
    }> FLASH AT> FLASH

    .text :
    {
	    *(.text)
	    *(.text.*)
	    . = ALIGN(4);
	    *(.ctors)
	    . = ALIGN(4);
	    *(.dtors)
	    . = ALIGN(4);
	    *(.init_array*)
	    *(.init)
	    *(.fini)
	    . = ALIGN(4);
    }> FLASH AT> FLASH

    .rodata : 
    {
	    *(.rodata)
	    *(.rodata.*)
	    . = ALIGN(4);
    }> FLASH AT> FLASH


    .ARM.exidx : 
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    .ARM.extab : 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    FLASH_DATA_START = LOADADDR(.data);

    .data :
    {

	    SRAM_DATA_START = .;
	    *(.data)
	    *(.data.*)
	    . = ALIGN(4);
	    SRAM_DATA_END = .;

	    __bss_start__ = .;
	    SRAM_BSS_START = .;
		
	    *(.bss)
	    *(.bss.*)
	    *(COMMON)
	    . = ALIGN(4);
	    __bss_end__ = .;
	    SRAM_BSS_END = .;
		    
    }> SRAM AT> FLASH

}

