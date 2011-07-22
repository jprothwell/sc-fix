## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## ------------------------------------- ##
##	Include Stack Dependency Macro   ##
## ------------------------------------- ##
include stack.mk

# Name of the module
LOCAL_NAME := platform/stack

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := \
	platform \
	platform/chip/hal \
	platform/base/common \
	platform/chip/defs \

# Compile the sub-modules, except when the "stack" must be used as a library.
ifeq ($(findstring platform/stack/packed,${GLOBAL_BINARY_LIBS}),)

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := \
	platform/stack/api \
	platform/stack/cb \
	platform/stack/cc \
	platform/stack/cod \
	platform/stack/e2p \
	platform/stack/fsm \
	platform/stack/lap \
	platform/stack/mm \
	platform/stack/rlp \
	platform/stack/rr \
	platform/stack/sms \
	platform/stack/ss \
	platform/stack/l1 \
	platform/stack/sim

ifneq ($(findstring -D__GPRS__,${MYCPPFLAGS}),)
LOCAL_MODULE_DEPENDS += \
	platform/stack/llc \
	platform/stack/rld \
	platform/stack/rlu \
	platform/stack/sm \
	platform/stack/snd
endif

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := yes

C_SRC += stack_map.c stk_profile.c

else # GLOBAL_BINARY_LIBS

ifeq "${WAPMMS_SUPPORT}" "1"

ifeq "${NUMBER_OF_SIM}" "1"
LOCAL_BINARY_LIBS    += platform/stack/packed/stack_${CT_ASIC}/stack_singlesim_gprs
LOCAL_MODULE_DEPENDS := platform/stack/packed/stack_${CT_ASIC}/stack_singlesim_gprs
endif

ifeq "${NUMBER_OF_SIM}" "2"
LOCAL_BINARY_LIBS    += platform/stack/packed/stack_${CT_ASIC}/stack_dualsim_gprs
LOCAL_MODULE_DEPENDS := platform/stack/packed/stack_${CT_ASIC}/stack_dualsim_gprs
endif

ifeq "${NUMBER_OF_SIM}" "3"
LOCAL_BINARY_LIBS    += platform/stack/packed/stack_${CT_ASIC}/stack_threesim_gprs
LOCAL_MODULE_DEPENDS := platform/stack/packed/stack_${CT_ASIC}/stack_threesim_gprs
endif

ifeq "${NUMBER_OF_SIM}" "4"
LOCAL_BINARY_LIBS    += platform/stack/packed/stack_${CT_ASIC}/stack_foursim_gprs
LOCAL_MODULE_DEPENDS := platform/stack/packed/stack_${CT_ASIC}/stack_foursim_gprs
endif

else # !WAPMMS_SUPPORT

ifeq "${NUMBER_OF_SIM}" "1"
LOCAL_BINARY_LIBS    += platform/stack/packed/stack_${CT_ASIC}/stack_singlesim
LOCAL_MODULE_DEPENDS := platform/stack/packed/stack_${CT_ASIC}/stack_singlesim
endif

ifeq "${NUMBER_OF_SIM}" "2"
LOCAL_BINARY_LIBS    += platform/stack/packed/stack_${CT_ASIC}/stack_dualsim
LOCAL_MODULE_DEPENDS := platform/stack/packed/stack_${CT_ASIC}/stack_dualsim
endif

ifeq "${NUMBER_OF_SIM}" "3"
LOCAL_BINARY_LIBS    += platform/stack/packed/stack_${CT_ASIC}/stack_threesim
LOCAL_MODULE_DEPENDS := platform/stack/packed/stack_${CT_ASIC}/stack_threesim
endif

ifeq "${NUMBER_OF_SIM}" "4"
LOCAL_BINARY_LIBS    += platform/stack/packed/stack_${CT_ASIC}/stack_foursim
LOCAL_MODULE_DEPENDS := platform/stack/packed/stack_${CT_ASIC}/stack_foursim
endif

endif # WAPMMS_SUPPORT

endif # GLOBAL_BINARY_LIBS

# This is a top-level module
IS_TOP_LEVEL := yes

AUTO_XCP2H ?= yes

LOCAL_EXPORT_FLAG += __XCPU__ 

export WITHOUT_WSHADOW := 1
export WITHOUT_WSIGN_COMPARE := 1

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H := 

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
