## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := toolpool/t_flash_programmer/SourceDepends

# list all modules APIs that are necessary to compile this module
LOCAL_API_DEPENDS := \
                    platform \
                    platform/base/common \
                    platform/calib \
                    platform/csw \
                    platform/csw/driver \
                    platform/csw/base \
                    platform/csw/base/tm \
                    platform/chip/bcpu \
                    platform/chip/hal \
                    platform/chip/defs \
                    platform/chip/regs/${CT_ASIC} \
                    platform/chip/boot/${CT_ASIC} \
                    platform/chip/pal \
                    platform/chip/patch \
                    platform/edrv/dualsimd \
                    platform/edrv/lcdd \
                    platform/edrv/mcd \
                    platform/edrv/memd \
                    platform/edrv/pmd \
                    platform/edrv/rfd \
                    platform/stack \
                    target \

# Compile the sub-modules, except when the "csw" must be used as a library.
#ifeq ($(findstring platform/csw/packed,${GLOBAL_BINARY_LIBS}),)

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := 

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files

%/boot_loader.o: CT_MIPS16_CFLAGS=
%/boot_sector.o: CT_MIPS16_CFLAGS=

#else # GLOBAL_BINARY_LIBS

#LOCAL_BINARY_LIBS    += platform/csw/packed/csw_dualsim_gprs
#LOCAL_MODULE_DEPENDS := platform/csw/packed/csw_dualsim_gprs

#endif # GLOBAL_BINARY_LIBS

# This is a top-level module
IS_TOP_LEVEL := yes

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##
LOCAL_EXPORT_FLAG += \
	HAVE_CFW_CONFIG \
	MF32_BASE_REG_DISABLE \
	MF32_NO_RFM_RH \
	MF32_NO_RFM \
	MF32_CFW_MMI_TWO_TASKS \
	CPP_KEYWORD_SUPPORT
	
LOCAL_EXPORT_FLAG += _T_UPGRADE_PROGRAMMER

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
