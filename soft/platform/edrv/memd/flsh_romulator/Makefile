## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/memd/flsh_romulator

LOCAL_API_DEPENDS := \
                     platform \
                     platform/base/common \
                     platform/chip/hal \
                     platform/chip/defs \
                     platform/edrv/memd \
                     platform/edrv \
                     platform/base/sx \
                     platform/base/std

C_SRC := drv_flsh.c
S_SRC :=

LOCAL_EXPORT_FLAG += USER_DATA_BASE=${USER_DATA_BASE}
LOCAL_EXPORT_FLAG += USER_DATA_SIZE=${USER_DATA_SIZE}

## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
