## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/chip/patch

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS ?= platform platform/base/common \
					 platform/chip/regs/${CT_ASIC} \
					 platform/chip/defs \
					 platform/chip/bcpu \
                     platform/chip/bcpu/spc/ \
                     platform/chip/bcpu/spp/ \
                     platform/chip/bcpu/spal/ \
					 platform/chip/hal \
					 platform/vpp 
                     
# list all the modules that need to be compiled prior to using this module
#
ifneq ($(findstring CHIP_BYPASS_ROM=1,$(TARGET_EXPORT_FLAG))$(findstring REWRITE_ROM_AT_BOOT,$(TARGET_EXPORT_FLAG)),)
# we are building a norom or fpga_softrom target
LOCAL_MODULE_DEPENDS := platform/chip/patch/no
else
ifeq ($(wildcard ${CT_ASIC}),)
# directory for current asic does not exist
LOCAL_MODULE_DEPENDS := platform/chip/patch/no
else
LOCAL_MODULE_DEPENDS := platform/chip/patch/${CT_ASIC}
endif
endif

# This is a top-level module
IS_TOP_LEVEL = yes

## ------------------------------------- ##
##      List all your sources here       ##
## ------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
