##############################################################################
# Include customvars.mk if it exists
##############################################################################
ifneq "$(wildcard $(SOFT_WORKDIR)/env/compilation/customvars.mk)" ""
include ${SOFT_WORKDIR}/env/compilation/customvars.mk
endif

#############################################################################
# Check if CT_TARGET is correctly defined and inlcude corresponding target.def
# FIXME: That section should disappear for the MMI.
#############################################################################
ifeq "$(wildcard $(SOFT_WORKDIR)/target/$(CT_TARGET)/target.def)" ""
    $(error "!!!! CT_TARGET not valid - CT_TARGET=${CT_TARGET} is not supported or not well defined !!!!")
else
# Include the Target definition
include ${SOFT_WORKDIR}/target/${CT_TARGET}/target.def
endif

#############################################################################
# Check if CT_ASIC is correctly defined and include corresponding ${CT_ASIC}.def
# FIXME: That section should disappear for the MMI.
#############################################################################
ifeq "$(wildcard $(SOFT_WORKDIR)/platform/chip/defs/$(CT_ASIC)_$(CT_ASIC_CFG).def)" ""
    $(error "!!!! No ASIC named: CT_ASIC=$(CT_ASIC)_$(CT_ASIC_CFG) !!!!")
else
# Include the ASIC definition
include ${SOFT_WORKDIR}/platform/chip/defs/${CT_ASIC}_${CT_ASIC_CFG}.def
endif

##############################################################################
# Include the ROM def file if it exists
##############################################################################
ifneq "$(wildcard $(SOFT_WORKDIR)/$(CHIP_ROM_DEF_FILE))" ""
ifneq "$(CHIP_ROM_DEF_FILE)" ""
include $(SOFT_WORKDIR)/$(CHIP_ROM_DEF_FILE)
endif
endif

#############################################################################
# Include the file which defines which modules must be used as lib.
#############################################################################
include ${SOFT_WORKDIR}/env/compilation/teamvars.mk
GLOBAL_BINARY_LIBS += ${TEAM_BINARY_LIBS}

#############################################################################
# Include the file which defines some properties for the platform
#############################################################################
ifneq "$(wildcard $(SOFT_WORKDIR)/platform/platform.def)" ""
include $(SOFT_WORKDIR)/platform/platform.def
endif

########################################################################
# MAKELEVEL=0 Things to do only once
# Variables that are defined only once should be in this section and 
# exported to sub-make
########################################################################
ifeq ($(MAKELEVEL),0)
ifeq "${MULTIPLE_APPLICATIONS}" "1"
	SHELL := bash
    APP_PWD := $(shell pwd -P)
	# resolve symlinks
    APP_SWD := $(shell cd ${SOFT_WORKDIR}/application; pwd -P)
    APP_PWD_NO_ROOT := $(patsubst $(APP_SWD)/%,%,$(APP_PWD))
    APP_PWD_NO_ROOT_SPLIT := $(subst /, ,$(APP_PWD_NO_ROOT))
    APP_PWD_NO_ROOT_SPLIT := $(subst \, ,$(APP_PWD_NO_ROOT_SPLIT))
    export CT_APP := $(firstword $(APP_PWD_NO_ROOT_SPLIT))
else  # !MULTIPLE_APPLICATIONS
	export CT_APP := _
endif # !MULTIPLE_APPLICATIONS
endif
########################################################################
# End MAKELEVEL=0 Things to do only once
########################################################################

#############################################################################
# Check if CT_MMI is correctly defined and include corresponding ${CT_MMI}.def
#############################################################################
ifeq "${MULTIPLE_APPLICATIONS}" "1"

ifeq "$(wildcard $(SOFT_WORKDIR)/application/${CT_APP}/mmi_cfg/${CT_MMI}.def)" ""
    $(error "!!!! No MMI named: CT_MMI=${CT_MMI} in ${SOFT_WORKDIR}/application/${CT_APP}/mmi_cfg directory !!!!")
else
# Include the MMI definition
include ${SOFT_WORKDIR}/application/${CT_APP}/mmi_cfg/${CT_MMI}.def
endif

else  # !MULTIPLE_APPLICATIONS

ifneq "${CT_ERES}" ""

ifeq "$(wildcard $(SOFT_WORKDIR)/application/target_res/${CT_ERES}/mmi_cfg/mmi_config.def)" ""
    $(error "!!!! No MMI Config in ${SOFT_WORKDIR}/application/target_res/${CT_ERES}/mmi_cfg directory !!!!")
else
# Include the MMI definition
MMI_CUSTOMER := application/target_res/${CT_ERES}/mmi_customer
MMI_CONFIG   := application/target_res/${CT_ERES}/mmi_cfg
endif

else #CT_ERES

ifeq "$(wildcard $(SOFT_WORKDIR)/application/mmi_cfg/mmi_config.def)" ""
    $(error "!!!! No MMI Config in ${SOFT_WORKDIR}/application/mmi_cfg directory !!!!")
else
# Include the MMI definition
MMI_CUSTOMER := application/mmi_customer
MMI_CONFIG   := application/mmi_cfg
endif

endif #CT_ERES

MMI_DEFAULT_CUSTOMER := application/mmi_customer

include ${SOFT_WORKDIR}/${MMI_CONFIG}/mmi_config.def
include ${SOFT_WORKDIR}/${MMI_CONFIG}/conversion.mk

endif # !MULTIPLE_APPLICATIONS

#############################################################################
# Compile all the lists of flags (except for the local list).
#############################################################################
ALL_EXPORT_FLAG := ${GLOBAL_EXPORT_FLAG} ${TARGET_EXPORT_FLAG} ${CHIP_EXPORT_FLAG}

COMPILEVARS_DEFINED:=yes
