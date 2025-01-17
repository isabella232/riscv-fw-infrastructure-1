OUTPUT_ARCH( "riscv" )

ENTRY( _start )

__comrv_overlay_storage_size = DEFINED(__comrv_overlay_storage_size) ? __comrv_overlay_storage_size : 0;

MEMORY
{
  flash (rxai!w) : ORIGIN = 0x20400000, LENGTH = 512M
  ram (wxa!ri)   : ORIGIN = 0x80000000, LENGTH = 16K
  ovl            : ORIGIN = 0xE0000000, LENGTH = __comrv_overlay_storage_size
}

PHDRS
{
  flash PT_LOAD;
  ram_init PT_LOAD;
  ram PT_NULL;
  ovl PT_NULL;
}

SECTIONS
{
  __stack_size = DEFINED(__stack_size) ? __stack_size : 2K;
  __comrv_cache_size = DEFINED(__comrv_cache_size) ? __comrv_cache_size : 0;
  __comrv_cache_alignment_size = DEFINED(__comrv_cache_alignment_size) ? __comrv_cache_alignment_size : 0;

  .init           :
  {
    KEEP (*(SORT_NONE(.init)))
  } >flash AT>flash :flash

  .text           :
  {
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(COMRV_TEXT_SEC)
    *(COMRV_TEXT_ASM)
    *(.gnu.linkonce.t.*)
  } >flash AT>flash :flash

  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >flash AT>flash :flash

  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);

  PSP_TEXT_SEC :
  {
    *(.psp_code_section)
    . = ALIGN(4);
  } >flash AT>flash :flash

  .rodata         :
  {
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    KEEP(*(COMRV_RODATA_SEC))
  } >flash AT>flash :flash

  /* this is a placeholder for comrv overlay groups.
     after linking, we'll use objcopy utility and change the
     address of .ovlgrpdata to this address */ 
  .reserved_ovl : ALIGN(4)
  {
    _OVERLAY_STORAGE_START_ADDRESS_ = .;
    . = LENGTH(ovl);
  } >flash AT>flash :flash

  . = ALIGN(4);

  .preinit_array  :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >flash AT>flash :flash

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >flash AT>flash :flash

  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >flash AT>flash :flash

  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >flash AT>flash :flash

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >flash AT>flash :flash

  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >flash AT>flash :flash

  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >ram AT>flash :ram_init

  .data          :
  {
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
  } >ram AT>flash :ram_init

  COMRV_DATA_SEC : 
  {
  
  }  >ram AT>flash :ram_init

  PSP_DATA_SEC : 
  {
    *(.psp_data_section)
    . = ALIGN(4);
 } >ram AT>flash :ram_init
  
  . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );

  PROVIDE( _fbss = . );
  PROVIDE( __bss_start = . );
  .bss            :
  {
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
  } >ram AT>ram :ram

  . = ALIGN(8);
  PROVIDE( _end = . );
  PROVIDE( end = . );

  .stack :
  {
    PROVIDE( _heap_end = . );
    . = __stack_size;
    PROVIDE( _sp = . );
  } >ram AT>ram :ram

 

  /* this area is used for loading and executing the comrv 
     overlay groups */
  .overlay_sec ALIGN(__comrv_cache_alignment_size) :
  {
    __OVERLAY_CACHE_START__ = . ;
    . = . + __comrv_cache_size; 
    __OVERLAY_CACHE_END__ = . ;
  }  >ram AT>ram :ram

  /* this is the location of all comrv overlay groups.
     after linking, we'll use objcopy utility and copy
     it to the .reserved_ovl section. we do this
     to make sure there is no address dependency after linking */ 
  .ovlgrps (OVERLAY) : 
  {
    OVERLAY_START_OF_OVERLAYS = .;
    *(.ovlinput.*)
    OVERLAY_END_OF_OVERLAYS = .;
  } >ovl AT>ovl :ovl
}
