## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/camdold/sp0828

ifeq (${CAMERA_IF}, spi)
LOCAL_MODULE_DEPENDS := ${LOCAL_NAME}/spi
else
LOCAL_MODULE_DEPENDS := ${LOCAL_NAME}/paralell
endif

IS_TOP_LEVEL := yes

LOCAL_API_DEPENDS ?= \
    target \
	platform \
	platform/base/common \
	platform/base/sx \
	platform/chip/hal \
	platform/chip/defs \
	platform/edrv/camd \
	platform/edrv/pmd \
	platform/edrv/camdold \


C_SRC :=
S_SRC :=


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
