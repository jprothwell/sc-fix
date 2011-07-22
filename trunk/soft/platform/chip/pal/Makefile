## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/chip/pal

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS ?= platform \
					 platform/chip/hal \
					 platform/chip/defs \
					 platform/base/common \
					 platform/base/sx \
					 platform/base/std \
					 platform/chip/bcpu \
					 platform/chip/bcpu/spc \
					 platform/chip/bcpu/spp \
					 platform/edrv/rfd \
					 platform/edrv/memd \
					 platform/calib \
					 platform/edrv/pmd \
					 platform/edrv/dualsimd \
					 platform/edrv/simd \
					 target \
# TODO: Remove the 'dualsimd' dependency when 'simd' is validated.

ifeq "${DUAL_SIM_SUPPORT}" "1"
#LOCAL_EXPORT_FLAG += DUAL_SIM_MODE
endif

# Compilation mode for the version string.
comma :=,
empty :=#
space := $(empty) $(empty)#
TGT_SIM_ORDER := ${foreach SIM, ${USER_SIM_ORDER},PAL_SIM_ORDER_SIM_${SIM}$(comma)}
PAL_SIM_LIST:=$(subst $(space),$(empty),$(TGT_SIM_ORDER))
LOCAL_EXPORT_FLAG += PAL_SIM_LIST=${PAL_SIM_LIST}


# PAL uses the SIMD EDRV implementation instead of DUALSIMD and HAL.
#LOCAL_EXPORT_FLAG += PAL_SIMD_IMPLEMENTATION

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS ?=  

# Generate the CoolProfile header automatically during the make process.
AUTO_XCP2H ?= yes

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= yes

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := yes

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
