/* This file is is generated by a shell script.  DO NOT EDIT! */

/* Handle embedded relocs for MIPS.
   Copyright 1994 Free Software Foundation, Inc.
   Written by Ian Lance Taylor <ian@cygnus.com> based on generic.em.

This file is part of GLD, the Gnu Linker.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#define TARGET_IS_mipsidt

#include "libiberty.h"
#include "bfd.h"
#include "sysdep.h"
#include "bfdlink.h"

#include "ld.h"
#include "config.h"
#include "ldmain.h"
#include "ldemul.h"
#include "ldfile.h"
#include "ldmisc.h"

static void gldmipsidt_before_parse PARAMS ((void));
static void gldmipsidt_after_open PARAMS ((void));
static void check_sections PARAMS ((bfd *, asection *, PTR));
static void gldmipsidt_after_allocation PARAMS ((void));
static char *gldmipsidt_get_script PARAMS ((int *isfile));

static void
gldmipsidt_before_parse()
{
#ifndef TARGET_			/* I.e., if not generic.  */
  ldfile_output_architecture = bfd_arch_mips;
#endif /* not TARGET_ */
}

/* This function is run after all the input files have been opened.
   We create a .rel.sdata section for each input file with a non zero
   .sdata section.  The BFD backend will fill in these sections with
   magic numbers which can be used to relocate the data section at run
   time.  This will only do the right thing if all the input files
   have been compiled using -membedded-pic.  */

static void
gldmipsidt_after_open ()
{
  bfd *abfd;

  if (! command_line.embedded_relocs
      || link_info.relocateable)
    return;

  for (abfd = link_info.input_bfds; abfd != NULL; abfd = abfd->link_next)
    {
      asection *datasec;

      datasec = bfd_get_section_by_name (abfd, ".sdata");

      /* Note that we assume that the reloc_count field has already
         been set up.  We could call bfd_get_reloc_upper_bound, but
         that returns the size of a memory buffer rather than a reloc
         count.  We do not want to call bfd_canonicalize_reloc,
         because although it would always work it would force us to
         read in the relocs into BFD canonical form, which would waste
         a significant amount of time and memory.  */
      if (datasec != NULL && datasec->reloc_count > 0)
	{
	  asection *relsec;

	  relsec = bfd_make_section (abfd, ".rel.sdata");
	  if (relsec == NULL
	      || ! bfd_set_section_flags (abfd, relsec,
					  (SEC_ALLOC
					   | SEC_LOAD
					   | SEC_HAS_CONTENTS
					   | SEC_IN_MEMORY))
	      || ! bfd_set_section_alignment (abfd, relsec, 2)
	      || ! bfd_set_section_size (abfd, relsec,
					 datasec->reloc_count * 4))
	    einfo ("%F%B: can not create .rel.sdata section: %E");
	}

      /* Double check that all other data sections are empty, as is
         required for embedded PIC code.  */
      bfd_map_over_sections (abfd, check_sections, (PTR) datasec);
    }
}

/* Check that of the data sections, only the .sdata section has
   relocs.  This is called via bfd_map_over_sections.  */

static void
check_sections (abfd, sec, sdatasec)
     bfd *abfd;
     asection *sec;
     PTR sdatasec;
{
  if ((bfd_get_section_flags (abfd, sec) & SEC_CODE) == 0
      && sec != (asection *) sdatasec
      && sec->reloc_count != 0)
    einfo ("%F%X: section %s has relocs; can not use --embedded-relocs",
	   abfd, bfd_get_section_name (abfd, sec));
}

/* This function is called after the section sizes and offsets have
   been set.  If we are generating embedded relocs, it calls a special
   BFD backend routine to do the work.  */

static void
gldmipsidt_after_allocation ()
{
  bfd *abfd;

  if (! command_line.embedded_relocs
      || link_info.relocateable)
    return;

  for (abfd = link_info.input_bfds; abfd != NULL; abfd = abfd->link_next)
    {
      asection *datasec, *relsec;
      char *errmsg;

      datasec = bfd_get_section_by_name (abfd, ".sdata");

      if (datasec == NULL || datasec->reloc_count == 0)
	continue;

      relsec = bfd_get_section_by_name (abfd, ".rel.sdata");
      ASSERT (relsec != NULL);

      if (! bfd_mips_ecoff_create_embedded_relocs (abfd, &link_info,
						   datasec, relsec,
						   &errmsg))
	{
	  if (errmsg == NULL)
	    einfo ("%B%X: can not create runtime reloc information: %E",
		   abfd);
	  else
	    einfo ("%X%B: can not create runtime reloc information: %s",
		   abfd, errmsg);
	}
    }
}

static char *
gldmipsidt_get_script(isfile)
     int *isfile;
{			     
  *isfile = 0;

  if (link_info.relocateable == true && config.build_constructors == true)
    return
concat(
"OUTPUT_FORMAT(\"ecoff-bigmips\", \"ecoff-bigmips\",\n\
	      \"ecoff-littlemips\")\n\
 SEARCH_DIR(/usr/local/mips-idt-ecoff/lib);\n\
ENTRY(start)\n\
SECTIONS\n\
{\n\
  .text : {\n\
    ;\n\
    *(.init)\n\
    ;\n\
    *(.text)\n\
    *(.rel.sdata)\n\
    *(.fini)\n\
    ;\n\
    ;\n\
  }\n\
 ","  .rdata : {\n\
    *(.rdata)\n\
  }\n\
  .data : {\n\
    *(.data)\n\
    CONSTRUCTORS\n\
  }\n\
  .lit8 : {\n\
    *(.lit8)\n\
  }\n\
  .lit4 : {\n\
    *(.lit4)\n\
  }\n\
 ","  .sdata : {\n\
    *(.sdata)\n\
  }\n\
  .sbss : {\n\
    *(.sbss)\n\
    *(.scommon)\n\
  }\n\
  .bss : {\n\
    *(.bss)\n\
    *(COMMON)\n\
  }\n\
}\n\n", NULL)
  ; else if (link_info.relocateable == true) return
"OUTPUT_FORMAT(\"ecoff-bigmips\", \"ecoff-bigmips\",\n\
	      \"ecoff-littlemips\")\n\
 SEARCH_DIR(/usr/local/mips-idt-ecoff/lib);\n\
ENTRY(start)\n\
SECTIONS\n\
{\n\
  .text : {\n\
    ;\n\
    *(.init)\n\
    ;\n\
    *(.text)\n\
    *(.rel.sdata)\n\
    *(.fini)\n\
    ;\n\
    ;\n\
  }\n\
  .rdata : {\n\
    *(.rdata)\n\
  }\n\
  .data : {\n\
    *(.data)\n\
  }\n\
  .lit8 : {\n\
    *(.lit8)\n\
  }\n\
  .lit4 : {\n\
    *(.lit4)\n\
  }\n\
  .sdata : {\n\
    *(.sdata)\n\
  }\n\
  .sbss : {\n\
    *(.sbss)\n\
    *(.scommon)\n\
  }\n\
  .bss : {\n\
    *(.bss)\n\
    *(COMMON)\n\
  }\n\
}\n\n"
  ; else if (!config.text_read_only) return
concat(
"OUTPUT_FORMAT(\"ecoff-bigmips\", \"ecoff-bigmips\",\n\
	      \"ecoff-littlemips\")\n\
 SEARCH_DIR(/usr/local/mips-idt-ecoff/lib);\n\
ENTRY(start)\n\
SECTIONS\n\
{\n\
  . = 0xa0012000;\n\
  .text : {\n\
     _ftext = . ;\n\
    *(.init)\n\
     eprol  =  .;\n\
    *(.text)\n\
    PROVIDE (__runtime_reloc_start = .);\n\
    *(.rel.sdata)\n\
    PROVIDE (__runtime_reloc_stop = .);\n\
    *(.fini)\n\
     etext  =  .;\n\
     _etext  =  .;\n\
","  }\n\
  . = .;\n\
  .rdata : {\n\
    *(.rdata)\n\
  }\n\
   _fdata = ALIGN(16);\n\
  .data : {\n\
    *(.data)\n\
    CONSTRUCTORS\n\
  }\n\
   _gp = ALIGN(16) + 0x8000;\n\
  .lit8 : {\n\
    *(.lit8)\n\
  }\n\
  .lit4 : {\n\
    *(.lit4)\n\
  }\n\
  .sdata : {\n\
    *(.sdata)\n\
  }\n\
","   edata  =  .;\n\
   _edata  =  .;\n\
   _fbss = .;\n\
  .sbss : {\n\
    *(.sbss)\n\
    *(.scommon)\n\
  }\n\
  .bss : {\n\
    *(.bss)\n\
    *(COMMON)\n\
  }\n\
   end = .;\n\
   _end = .;\n\
}\n\n"
, NULL)
  ; else if (!config.magic_demand_paged) return
concat (
"OUTPUT_FORMAT(\"ecoff-bigmips\", \"ecoff-bigmips\",\n\
	      \"ecoff-littlemips\")\n\
 SEARCH_DIR(/usr/local/mips-idt-ecoff/lib);\n\
ENTRY(start)\n\
SECTIONS\n\
{\n\
  . = 0xa0012000;\n\
  .text : {\n\
     _ftext = . ;\n\
    *(.init)\n\
     eprol  =  .;\n\
    *(.text)\n\
    PROVIDE (__runtime_reloc_start = .);\n\
    *(.rel.sdata)\n\
    PROVIDE (__runtime_reloc_stop = .);\n\
    *(.fini)\n\
     etext  =  .;\n\
     _etext  =  .;\n\
 ","  }\n\
  . = .;\n\
  .rdata : {\n\
    *(.rdata)\n\
  }\n\
   _fdata = ALIGN(16);\n\
  .data : {\n\
    *(.data)\n\
    CONSTRUCTORS\n\
  }\n\
   _gp = ALIGN(16) + 0x8000;\n\
  .lit8 : {\n\
    *(.lit8)\n\
 ","  }\n\
  .lit4 : {\n\
    *(.lit4)\n\
  }\n\
  .sdata : {\n\
    *(.sdata)\n\
  }\n\
   edata  =  .;\n\
   _edata  =  .;\n\
   _fbss = .;\n\
  .sbss : {\n\
    *(.sbss)\n\
    *(.scommon)\n\
 ","  }\n\
  .bss : {\n\
    *(.bss)\n\
    *(COMMON)\n\
  }\n\
   end = .;\n\
   _end = .;\n\
}\n\n"
, NULL)
  ; else return
concat (
"OUTPUT_FORMAT(\"ecoff-bigmips\", \"ecoff-bigmips\",\n\
	      \"ecoff-littlemips\")\n\
 SEARCH_DIR(/usr/local/mips-idt-ecoff/lib);\n\
ENTRY(start)\n\
SECTIONS\n\
{\n\
  . = 0xa0012000;\n\
  .text : {\n\
     _ftext = . ;\n\
    *(.init)\n\
     eprol  =  .;\n\
    *(.text)\n\
    PROVIDE (__runtime_reloc_start = .);\n\
    *(.rel.sdata)\n\
    PROVIDE (__runtime_reloc_stop = .);\n\
    *(.fini)\n\
     etext  =  .;\n\
     _etext  =  .;\n\
 ","  }\n\
  . = .;\n\
  .rdata : {\n\
    *(.rdata)\n\
  }\n\
   _fdata = ALIGN(16);\n\
  .data : {\n\
    *(.data)\n\
    CONSTRUCTORS\n\
  }\n\
   _gp = ALIGN(16) + 0x8000;\n\
  .lit8 : {\n\
    *(.lit8)\n\
  }\n\
  .lit4 : {\n\
    *(.lit4)\n\
 ","  }\n\
  .sdata : {\n\
    *(.sdata)\n\
  }\n\
   edata  =  .;\n\
   _edata  =  .;\n\
   _fbss = .;\n\
  .sbss : {\n\
    *(.sbss)\n\
    *(.scommon)\n\
  }\n\
  .bss : {\n\
    *(.bss)\n\
    *(COMMON)\n\
  }\n\
   end = .;\n\
   _end = .;\n\
}\n\n"
, NULL)
; }

struct ld_emulation_xfer_struct ld_mipsidt_emulation = 
{
  gldmipsidt_before_parse,
  syslib_default,
  hll_default,
  after_parse_default,
  gldmipsidt_after_open,
  gldmipsidt_after_allocation,
  set_output_arch_default,
  ldemul_default_target,
  before_allocation_default,
  gldmipsidt_get_script,
  "mipsidt",
  "ecoff-bigmips"
};
