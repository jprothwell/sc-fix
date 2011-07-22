## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/memd/flsh_s71ws256pd0_romu

LOCAL_API_DEPENDS ?= platform platform/base/common platform/chip/hal target \
                     platform/edrv/memd platform/edrv platform/base/sx \
                     platform/chip/defs

# Overwrites the flash model to generate the romulator driver emulating a
# particular type of flash.
FLSH_MODEL = flsh_s71ws256pd0


# This vpath is used to get the generic romulator driver source code.
vpath %.c . ../flsh_romulator
# This vpath is used to get the flash mapping header file.
vpath %.c . ../${FLSH_MODEL}

C_SRC := drv_flsh.c drv_flsh_map.c
S_SRC :=

LOCAL_EXPORT_FLAG += USER_DATA_BASE=${USER_DATA_BASE}
LOCAL_EXPORT_FLAG += USER_DATA_SIZE=${USER_DATA_SIZE}



## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
