dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.64)dnl
AC_INIT(Makefile.in)
sinclude(../common/acinclude.m4)
  
  case "${target_alias}" in
  or1k*-linux*)
    traps_obj=traps-linux.o
    ;;
  *)
    traps_obj=traps.o
    ;;
  esac

  case "${target_alias}" in
  or1knd-*)
    want_or1k_nodelay=true
    default_model=or1200nd
    ;;
  *)
    default_model=or1200
    ;;
  esac

SIM_AC_COMMON

SIM_AC_OPTION_ENDIAN(BIG_ENDIAN)
SIM_AC_OPTION_ALIGNMENT(STRICT_ALIGNMENT)
SIM_AC_OPTION_HOSTENDIAN
SIM_AC_OPTION_BITSIZE([32], [31], [32])
SIM_AC_OPTION_SCACHE([16384])
SIM_AC_OPTION_DEFAULT_MODEL([$default_model])
SIM_AC_OPTION_ENVIRONMENT
SIM_AC_OPTION_INLINE()
SIM_AC_OPTION_CGEN_MAINT

AC_SUBST(traps_obj)
AC_SUBST(sim_extra_cflags)
AC_SUBST(want_or1k64)
AC_SUBST(want_or1k_nodelay)

SIM_AC_OUTPUT
