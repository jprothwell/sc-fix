## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/lcdd/fd54124bv

ifeq (${LCD_IF}, lcdd)
LOCAL_MODULE_DEPENDS := platform/edrv/lcdd/fd54124bv/lcdc
else
ifeq (${LCD_IF}, ebc)
LOCAL_MODULE_DEPENDS := platform/edrv/lcdd/fd54124bv/ebc
else
$(error unkown LCD_IF)
endif
endif


IS_TOP_LEVEL := yes

LOCAL_API_DEPENDS := \
	platform \
	platform/base/common \
	platform/chip/hal \
	platform/edrv/lcdd \
	platform/edrv/pmd \
	target \
	platform/base/sx \
	platform/chip/defs\


C_SRC :=
S_SRC :=


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
