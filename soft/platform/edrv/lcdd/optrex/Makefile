## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/lcdd/optrex

LOCAL_API_DEPENDS ?= platform \
                     platform/base/common \
                     platform/base/sx \
                     platform/chip/defs \
                     platform/chip/hal \
                     platform/lcdd \
					 platform/edrv/pmd \
                     target


LOCAL_ADD_INCLUDE := platform/edrv/lcdd/src

C_SRC := drv_lcd.c
S_SRC :=


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
