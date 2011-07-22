## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/edrv/btd

LOCAL_API_DEPENDS := platform \
	 	 platform/base/common \
		 platform/chip/hal \
		 platform/base/sx \
		 platform/base/std

ifneq ($(findstring rda,${BT_MODEL}),)
LOCAL_MODULE_DEPENDS += platform/edrv/btd/rdaCommon
LOCAL_MODULE_DEPENDS += platform/edrv/btd/rdaDriver
LOCAL_EXPORT_FLAG 	 += __BT_RDABT__
endif # rda

ifneq ($(findstring rda586,${BT_MODEL}),)
ifneq ($(findstring rda587,${BT_MODEL}),)
$(error You can NOT put __RDA_BT_5868_SERIES__ and __RDA_BT_587x_SERIES__ in BT_MODEL list at the same time!!!)
endif # rda587
endif # rda586

ifneq ($(findstring rda586,${BT_MODEL}),)

	LOCAL_EXPORT_FLAG += __RDA_BT_5868_SERIES__
	
	ifneq ($(findstring rda5868e,${BT_MODEL}),)
		LOCAL_EXPORT_FLAG += __RDA_CHIP_R10_5868E__
	endif # rda5868e
	
endif # rda587x

ifneq ($(findstring rda587,${BT_MODEL}),)

	LOCAL_EXPORT_FLAG += __RDA_BT_587x_SERIES__

	ifneq ($(findstring rda5870,${BT_MODEL}),)
		LOCAL_EXPORT_FLAG += __RDA_CHIP_R8_5870__
	endif # rda5870

	ifneq ($(findstring rda5872,${BT_MODEL}),)
		ifneq ($(findstring rda5872p,${BT_MODEL}),)
			LOCAL_EXPORT_FLAG += __RDA_CHIP_R11_5872P__
		endif # rda5872p
		ifeq ($(findstring rda5872p,${BT_MODEL}),)
			LOCAL_EXPORT_FLAG += __RDA_CHIP_R10_5872__
		endif # rda5872
	endif # rda5872
	


endif # rda587x

ifneq ($(findstring rda5875,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_BT_5875_SERIES__
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R11_5875__
endif # rda5872

ifeq ($(BT_SHARE_CRYSTAL),1)
	LOCAL_EXPORT_FLAG += __RDA_SHARE_CRYSTAL__
endif # BT_CRYSTAL_SHARED

# This is a top-level module
IS_TOP_LEVEL = yes

# This needs to deliver environment related files also
DELIV_ENV_FILES := 0

ifeq "${RELEASE_MODULE}" "1"
# In case of LIB_RELEASE, list all the modules
ifneq (${BT_MODEL},)
LOCAL_DEPEND_LIBS := \
			platform/edrv/btd/rdaCommon \
			
endif # (${BT_MODEL},)
endif #RELEASE_MODULE

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= yes

# Generates the CoolProfile headers automatically.
#AUTO_XCP2H ?= yes

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := yes

# Compilation mode for the version string.
LOCAL_EXPORT_FLAG += BTD_BT_MODEL=${BT_MODEL}

# Flags exported throughout the build
LOCAL_EXPORT_FLAG +=


## ------------------------------------------------------------------------- ##
##	                       Source files                                      ##
## ------------------------------------------------------------------------- ##
# No source files in the top-level itself...
S_SRC :=
C_SRC := btd_map.c
INPUT_FILES :=  

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


