## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/lcdd/s6d0154/ebc


LOCAL_API_DEPENDS ?= platform \
                     platform/base/common \
                     platform/base/sx \
                     platform/chip/hal \
                     platform/chip/defs \
                     platform/chip/regs/${CT_ASIC} \
                     platform/edrv/lcdd \
					 platform/edrv/pmd \
                     target


C_SRC := drv_lcd.c lcd_if.c lcd_init.c
S_SRC := ${notdir ${wildcard *.S}}


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
