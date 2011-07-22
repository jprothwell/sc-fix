## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

LOCAL_NAME := platform/edrv/cammd

LOCAL_API_DEPENDS ?= \
    target \
	platform \
	platform/base/common \
	platform/base/sx \
	platform/chip/hal \
	platform/chip/defs \
	platform/edrv/camdold \
	platform/edrv/pmd \
	platform/edrv/i2cgpio \
	platform/edrv/camdold \
	platform/edrv/pmd \
	platform/edrv/lcdd \
	platform/csw \
	platform/mdi \
	platform/mdi/mmc \
	application/systeminc/mmi	\
	application/systeminc/interface/mmi \
	application/systeminc/interface/media \
	application/systeminc/interface/ps \
	application/systeminc/interface/hwdrv \
	application/systeminc/interface/config \
	${MMI_CONFIG} \
	application/coolmmi/mmi	\
	application/coolmmi/mmi/GUI \
	application/coolmmi/mmi_csdapp/mmiapi \
	application/coolmmi/media/common \
	application/coolmmi/mmi/Framework	\


C_SRC := ${notdir ${wildcard src/*.c}}
S_SRC := ${notdir ${wildcard *.S}}


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
