#ifndef OR1K_SIM_H
#define OR1K_SIM_H

#include "symcat.h"

/* GDB register numbers. */
#define PPC_REGNUM	32
#define PC_REGNUM	33
#define SR_REGNUM	34

/* Misc. profile data.  */
typedef struct {
} OR1K_MISC_PROFILE;

#define GETTWI XCONCAT2(GETT,WI)
#define SETTWI XCONCAT2(SETT,WI)

#define EXIT_ERROR       1
#define EXIT_UNSPECIFIED 2

#endif /* OR1K_SIM_H */
