##############################################################################
#  Coolsand main compilation rules file
##############################################################################
# behavior of compilation can be affected by the following:
# 	customvars.mk : this file included at the beginning of all Makefile
# 		allows to define variables that will modify the behavior of make
# 		ex: GLOBAL_BINARY_LIBS += platform/stack will prevent recompilation 
# 		    of the stack and use the lib version instead
# 		    GLOBAL_EXPORT_FLAG += MY_FLAG=my_value will define the flag MY_FLAG 
# 		    at the my_value value in all calls to make
#
# 	cutomrules.mk: this file is included at the end of compilerules.mk
# 		It allows to add rules that are only usefull for a specific team or 
# 		individual
# 		ex: msim: prerequisite
# 				rule_action
# 			this rule used to use a simulator to simulate the HW is only usefull 
# 			for ASIC teams as they have access to the HW source code and simulator
#
#   For explanations about the Makefile variables, please see the SoftEnv 
#   manual in the wiki.
#

# Cancel most of the default suffix/implicit rules (To cancel all of them,
# we have to use the make option -r or --no-builtin-rules)
#.SUFFIXES: .hex .o .c .asm .S .h
.SUFFIXES:

# Do not treat all targets as intermediate files
#.SECONDARY:

########################################################################
# MAKELEVEL=0 Things to do only once
# Variables that are defined only once should be in this section and 
# exported to sub-make
########################################################################
ifeq ($(MAKELEVEL),0)

# force the local to be the default 'C' local in english, this really helps when parsing outputs
export LANG=C

########################################################################
#check that all the parameters are OK
########################################################################
# Checking that env file customvars.mk exists
# Checking that 3 main variables are well defined
# 		CT_RELEASE
# 		CT_ASIC
# 		CT_TARGET
# Checking that drivers exist for FLSH_MODEL, XCV_MODEL, PA_MODEL, SW_MODEL


# Check if CT_RELEASE is defined and valid
VALID_RELEASE_LIST :=release debug calib sa_profile cool_profile custom
SELECTED_RELEASE := $(filter $(VALID_RELEASE_LIST), $(CT_RELEASE))
ifeq "$(SELECTED_RELEASE)" ""
    $(error "!!!! CT_RELEASE=${CT_RELEASE} - Not a valid release type !!!!")
endif

# Check if a driver exists for the specified FLSH_MODEL
ifeq "$(wildcard $(SOFT_WORKDIR)/platform/edrv/memd/$(FLSH_MODEL))" ""
    $(error "\n!!!! FLSH_MODEL=$(FLSH_MODEL) not valid - No driver exist\nCT_TARGET=${CT_TARGET} is not well defined \n!!!!")
endif

# Check if a driver exists for the specified XCV_MODEL
ifeq "$(wildcard $(SOFT_WORKDIR)/platform/edrv/rfd/$(XCV_MODEL))" ""
    $(error "\n!!!! XCV_MODEL=$(XCV_MODEL) not valid - No driver exist\nCT_TARGET=${CT_TARGET} is not well defined \n!!!!")
endif

# Check if a driver exists for the specified PA_MODEL
ifeq "$(wildcard $(SOFT_WORKDIR)/platform/edrv/rfd/$(PA_MODEL))" ""
    $(error "\n!!!! PA_MODEL=$(PA_MODEL) not valid - No driver exist\nCT_TARGET=${CT_TARGET} is not well defined \n!!!!")
endif

# Check if a driver exists for the specified SW_MODEL
ifeq "$(wildcard $(SOFT_WORKDIR)/platform/edrv/rfd/$(SW_MODEL))" ""
    $(error "\n!!!! SW_MODEL=$(SW_MODEL) not valid - No driver exist\nCT_TARGET=${CT_TARGET} is not well defined \n!!!!")
endif

# Check if a driver exists for the specified CT_GENRES
VALID_RESGEN_LIST :=YES yes Yes NO no No
SELECTED_RESGEN := $(filter $(VALID_RESGEN_LIST), $(CT_RESGEN))
ifeq "$(SELECTED_RESGEN)" ""
    ifneq "${CT_RESGEN}" ""
        $(error "!!!! CT_RESGEN=${CT_RESGEN} - Not a valid resource generation option !!!!")
    endif    
endif

# Check if "RESGEN" is needed
ifneq "${CT_RESGEN}" "yes"
	export RESGEN := no
else
	export RESGEN := yes
endif

# Base path for storing all the generated files for tests
# Always the same dir for convenience.
export HEX_PATH ?= ${SOFT_WORKDIR}/hex

##############################################################################
# Constructs CYGPATH if we are under cygwin
##############################################################################
# Construct the proper CYGPATH building path function
# If we're on a unix machine we don't change the path
# If we are on a CYGWIN install we need to convert the 
# cygwin path into a windows path to use coolxml

## COMPAT_SOFT_WORKDIR = SOFT_WORKDIR version compatible with Windows and Cygwin/Linux env
# To be used with internal tools compiled for windows and linux
ifneq "$(shell uname | grep -c CYGWIN)" "0"
    export CYGWIN := 1
    export CYGPATH := cygpath -am
    export WIN_SOFT_WORKDIR := $(shell ${CYGPATH} ${SOFT_WORKDIR})
    export WIN_HEX_PATH := $(shell ${CYGPATH} ${HEX_PATH})
else
    export WIN_SOFT_WORKDIR := ${SOFT_WORKDIR}
    export WIN_HEX_PATH := ${HEX_PATH}
endif


##############################################################################
##  Silent Make Option
##############################################################################
ifneq "${VERBOSE}" "1"
    export MAKEFLAGS += -s --no-print-directory
    # Pipe STDOUT/STDERR to /dev/null if not VERBOSE
    # Conversely, have a null output "echo" for "CC ... (file)" stuff when VERBOSE 
    # is set
    export STDOUT_NULL := 1>/dev/null
    export STDERR_NULL := 2>/dev/null
    export ECHO := echo
else
    export ECHO := echo >/dev/null
endif # ! VERBOSE

ifeq "${CT_RAMRUN}" "1"
export CT_JAVA_ENABLE := 0
export CT_USE_JAVA_LIB :=0
export CT_USE_DEBUG_LIBGCJ := 0
else
export CT_JAVA_ENABLE := 0
export CT_USE_JAVA_LIB :=1
export CT_USE_DEBUG_LIBGCJ := 0
endif
##########################################
# Define the tools to use
##########################################
export CROSS ?= mips-elf-

export AS := $(CROSS)as
ifeq "${CCACHE_ENABLE}" "1"
export CC := ccache $(CROSS)gcc
else
export CC := $(CROSS)gcc
endif
export CPP := ${CC} -E
export C++ := $(CROSS)g++ 
ifeq "${CT_JAVA_ENABLE}" "1"
export GCJ := $(CROSS)gcj
export GCJH := gjavah
endif
export AR := $(CROSS)ar
export OBJCOPY := $(CROSS)objcopy
export OBJDUMP := $(CROSS)objdump
export NM := $(CROSS)nm
export LD := $(CROSS)ld
# To preprocess non-C-code files (no line numbers, no garbage, no predefined vars)
export LDPP := $(CROSS)cpp -C -P -undef 
export GDB ?= $(CROSS)gdbtui

export CP := cp -f
export FIND := find

#GCC_VERSION := $(shell $(CC) --version | head -n1 | sed 's/^[^(]*([^)]*) *\([0-9][0-9.]*\)[^0-9.].*$/\1/')

# Version checking function
# $(1): shell commands to generate the version string at the first line
# $(2): major version (the x in x.y)
# $(3): minor version (the y in x.y)
# Return: 2 if the version string is mismatched,
#         1 if the version is equal to or above the compared one,
#         0 otherwise.
VERSION_CHECK_FUNC = $(shell $(1) | \
    awk 'FNR==1 { num=match($$0,"^.* +([0-9]+)\\.([0-9]+)[^0-9].*$$",ary); \
                  if(num==0) {print 2; exit;} \
                  if(ary[1]<$(2) || (ary[1]==$(2) && ary[2]<$(3))) \
                      {print 0;} \
                  else {print 1;} \
                }')

GCC_VER_MAJOR := 4
GCC_VER_MINOR := 0
GCC_VER_CHECK := $(call VERSION_CHECK_FUNC, $(CC) --version, $(GCC_VER_MAJOR), $(GCC_VER_MINOR))

BINUTILS_VER_MAJOR := 2
BINUTILS_VER_MINOR := 19
BINUTILS_VER_CHECK := $(call VERSION_CHECK_FUNC, $(LD) --version, $(BINUTILS_VER_MAJOR), $(BINUTILS_VER_MINOR))

ifeq "$(findstring get_,${MAKECMDGOALS})" ""
PRINT_INFO = $(info $(1))
else
PRINT_INFO = 
endif

# Use GCC 4 ?
# Use the 2-19 binutils ?
# Remove all the unused input sections ?
$(call PRINT_INFO)
ifeq "${GCC_VER_CHECK}" "0"

ifneq "${BINUTILS_VER_CHECK}" "0"
$(error Binutils version is 2.19 or above whereas GCC version is 3)
endif
$(call PRINT_INFO,GCC version is 3)
export USE_GCC_4 := 0
export USE_BINUTILS_2_19 := 0
# Garbage collection of unused input sections is not supported
export ENABLE_GC_SECTIONS := 0

else # GCC_VER_CHECK != 0

ifeq "${GCC_VER_CHECK}" "1"

ifneq "${BINUTILS_VER_CHECK}" "1"
$(error Binutils version is below 2.19 whereas GCC version is 4 or above)
endif
$(call PRINT_INFO,GCC version is 4 or above)
export USE_GCC_4 := 1
export USE_BINUTILS_2_19 := 1
export ENABLE_GC_SECTIONS ?= 1

else # GCC_VER_CHECK != 1

ifeq "${GCC_VER_CHECK}" "2"
$(call PRINT_INFO,GCC version is unknown - assuming GCC 4)
export USE_GCC_4 := 1
export USE_BINUTILS_2_19 := 1
export ENABLE_GC_SECTIONS ?= 1
endif # GCC_VER_CHECK == 2

endif # GCC_VER_CHECK != 1

endif # GCC_VER_CHECK != 0
$(call PRINT_INFO)

# Discard all the unlisted input sections (even if they are used) ?
export DISCARD_UNLISTED_SECTIONS ?= 1
# Print the removed unused input sections ?
export PRINT_GC_SECTIONS ?= 0

ifeq "${USE_GCC_4}" "1"
C_SPECIFIC_CFLAGS    += -Woverride-init
endif

ifeq "${USE_GCC_4}" "1"
export LIBGCC_PATH := ${SOFT_WORKDIR}/env/lib/gcc/mips-elf/4.4.2/
else
export LIBGCC_PATH := ${SOFT_WORKDIR}/env/lib/gcc/mips-elf/3.4.6/
endif

# Limit objdump to these sections :
export OBJDUMP_DISPLAY_SECTS := .boot_code .calib .factory_settings .main_entry_section .sramtext .sramdata .sramucdata .ram .text .ucramdata .data .rotext .rodata .internal_rom .internal_tmp_rom .internal_rom.crc .sramtxt .sysIrqSection .bbIrqSection .sramtextmisc .proxy_memory .fast_cpu_memory_overlay_0 .fast_cpu_memory_overlay_1 .fast_cpu_memory_overlay_2 .fast_cpu_memory_overlay_3 .fast_cpu_memory_overlay_4 .fast_cpu_memory_overlay_5 .boot_rom_version_number

export OBJDUMP_OPTS := ${foreach sect, ${OBJDUMP_DISPLAY_SECTS}, -j ${sect}} -D -t

export WINEDEBUG := -all
ifeq "${CYGWIN}" "1"
export LZMA := ${SOFT_WORKDIR}/application/mmi_customer/ResGenerator/compress/lzma/lzma.exe
else
export LZMA := wine ${SOFT_WORKDIR}/application/mmi_customer/ResGenerator/compress/lzma/lzma.exe
endif

###############################################################################
# Generic environment stuff
###############################################################################
## Generic directory names
export BUILD_DIR := build/${CT_TARGET}
export OBJ_DIR := obj
export SRC_DIR := src
export LIB_DIR := lib
export DEPS_DIR := deps
export INC_DIR := include

# BUILD_ROOT is the base of the build tree,
# just like SOFT_WORKDIR is that of the source tree
# It defaults to  ${SOFT_WORKDIR}/${BUILD_DIR} when OBJECT_DIR is not set.
ifneq "$(OBJECT_DIR)" ""
export BUILD_ROOT := ${OBJECT_DIR}/${BUILD_DIR}
else
export BUILD_ROOT := ${SOFT_WORKDIR}/${BUILD_DIR}
endif

# Date to include in binaries as build date
# TODO_OPTIM?
export BUILD_TIME := $(shell date "+%Y%m%d%a%H%M")
export BUILD_DATE := $(shell date "+%Y%m%d")

## These targets simply call the bin rule 
# If the last target built is different from the last one, clean before building
export LAST_TARGET_FILE := ${BUILD_ROOT}/last_target.txt
export LAST_TARGET_EXIST := $(wildcard ${LAST_TARGET_FILE})
ifeq ($(strip $(LAST_TARGET_EXIST)),)
export LAST_TARGET :=
else
export LAST_TARGET := $(shell cat ${LAST_TARGET_FILE})
endif

# Allow the objects of different release types to coexist with each other
#export TARGET_TAG := ${CT_RELEASE}_${CT_TARGET}
export TARGET_TAG := ${CT_TARGET}

# Flag to put rom contents into flash elf file (not for ramrun)
ifeq "${CT_RAMRUN}" "1"
export ROM_IN_FLASH_ELF ?= 0
else
export ROM_IN_FLASH_ELF ?= 1
endif

# Flags to generate lods to clean special sectors
export CLEAN_USER_SECTOR ?= 0
export CLEAN_CALIB_SECTOR ?= 0
export CLEAN_FACT_SECTOR ?= 0

ifneq "${CLEAN_USER_SECTOR}${CLEAN_CALIB_SECTOR}${CLEAN_FACT_SECTOR}" "000"
# Generate sector cleaner lods only
override CT_MODULES := link
# Do NOT compress code (but the override cannot take effect in sub-make by exporting)
override CT_COMPRESS_CODE_SECTION := no
endif

# Do NOT compress code if this is a ramrun build, or this is not a full MMI build.
# NOTE: The following assignments cannot override the definition from command line.
ifeq "${CT_RAMRUN}" "1"
CT_COMPRESS_CODE_SECTION := no
endif # CT_RAMRUN
ifeq "$(findstring application,${LOCAL_MODULE_DEPENDS})" ""
CT_COMPRESS_CODE_SECTION := no
endif # LOCAL_MODULE_DEPENDS

export CT_COMPRESS_CODE_SECTION

#-----------------------------------------------------------------
# Optional Build:
#     Build designated modules only and link all others.
#
# CT_MODULES defines a comma-delimited module list, which should
# be rebuilt in this make, and all other modules can be linked
# with the libraries generated in the last make.
# ASSUMPTION:
#     LOCAL_MODULE_DEPENDS are located in the subdirectories of this module.
# KNOWN EXCEPTIONS:
#     platform -- target
#     platform/chip/hal -- platform/chip/regs/$(CT_ASIC)
# (All the exceptions can be figured out by specifying
# DUMP_DEP_NOT_IN_SUBDIR=1 in the command line)
#-----------------------------------------------------------------
export MODULES_MUST_BUILD :=

ifneq "$(CT_MODULES)" ""
NULL_STRING :=
SPACE := $(NULL_STRING) # end of the line
COMMA := ,
MODULES_MUST_BUILD := $(subst $(COMMA),$(SPACE),$(CT_MODULES))

# target_config.c needs to record the product name
ifneq "$(CT_PRODUCT)" ""
MODULES_MUST_BUILD += target
endif

# Define a fake directory hierachy for an exceptional prerequisite module,
# so as to show the dependency but not to rebuild the whole upper-level module
EXCEPTION_MODULE_1 := platform
EXCEPTION_MOD_DEP_1 := target
EXCEPTION_FAKE_SUBDIR_1 := platform/target

EXCEPTION_MODULE_2 := platform/chip/hal
EXCEPTION_MOD_DEP_2 := platform/chip/regs/$(CT_ASIC)
EXCEPTION_FAKE_SUBDIR_2 := platform/chip/hal/regs

ifneq "$(findstring $(SPACE)$(EXCEPTION_MOD_DEP_1)$(SPACE),$(SPACE)$(MODULES_MUST_BUILD)$(SPACE))" ""
MODULES_MUST_BUILD += $(EXCEPTION_FAKE_SUBDIR_1)
endif
ifneq "$(findstring $(SPACE)$(EXCEPTION_MODULE_1)$(SPACE),$(SPACE)$(MODULES_MUST_BUILD)$(SPACE))" ""
MODULES_MUST_BUILD += $(EXCEPTION_MOD_DEP_1)
endif

ifneq "$(findstring $(SPACE)$(EXCEPTION_MOD_DEP_2)$(SPACE),$(SPACE)$(MODULES_MUST_BUILD)$(SPACE))" ""
MODULES_MUST_BUILD += $(EXCEPTION_FAKE_SUBDIR_2)
endif
ifneq "$(findstring $(SPACE)$(EXCEPTION_MODULE_2)$(SPACE),$(SPACE)$(MODULES_MUST_BUILD)$(SPACE))" ""
MODULES_MUST_BUILD += $(EXCEPTION_MOD_DEP_2)
endif

MODULES_MUST_BUILD := $(sort $(MODULES_MUST_BUILD))
endif # CT_MODULES
#-----------------------------------------------------------------
# End of Optional Build
#-----------------------------------------------------------------

########################################################################
# safety check
########################################################################
ifeq ($(origin LOCAL_EXPORT_FLAG),command line)
$(error LOCAL_EXPORT_FLAG must not be overriden from the command line!)
endif

########################################################################
# End of MAKELEVEL=0. Things to do only once.
########################################################################
endif

ifeq ($(MAKELEVEL),40)
$(error Recursive make reached level 40, there is probably a cycle in your module/model dependancies)
endif #MAKELEVEL 40

# Check if compilevars.mk has been correctly included at the beginning of the Makefile 
ifndef COMPILEVARS_DEFINED
    $(error "!!!! Each Makefile must include $(SOFT_WORKDIR)/env/compilation/compilevars.mk  at the beginning !!!!")
endif

# Must not be exported to be taken into account
SHELL := /bin/bash 



#############################################################################
# Defaults for global options
#############################################################################
WITH_PERL ?= 1
# All files are compiled in mips16 format by default
# By default all files are compiled in MIPS16 format
# with dbg info + optimized
# with reduced regs
CT_MIPS16_CFLAGS ?= -mips16
WITH_REDUCED_REGS := 1
CT_ROMULATOR ?= no
# Default configuration to chip
CT_ASIC_CFG ?= chip

# for legacy Makefile compatibility
LOCAL_MODULE_DEPENDS += $(LOCAL_DEPEND_LIBS)

# Absolute path to PWD, from the Makefile
LOCAL_PATH := ${SOFT_WORKDIR}/${LOCAL_NAME}
# Corresponding path within the build tree
LOCAL_SHADOW_PATH := ${BUILD_ROOT}/${LOCAL_NAME}

# Module name.
MODULE_NAME := ${notdir ${LOCAL_NAME}}

# LOCAL_*_DIR == abstract for *_DIR, more generic (takes into account IS_ENTRY_POINT
# where we have no src/, etc...)
# For use as relative path
ifneq "${IS_ENTRY_POINT}" "yes"
	LOCAL_SRC_DIR := ${SRC_DIR}
	LOCAL_INC_DIR := ${INC_DIR}
	LOCAL_LIB_DIR := ${LIB_DIR}
else
	LOCAL_SRC_DIR := .
	LOCAL_INC_DIR := .
	LOCAL_LIB_DIR := .
endif #IS_ENTRY_POINT

ifeq "${IS_DLL_ENTRY}" "yes"
	LOCAL_SRC_DIR := .
	LOCAL_INC_DIR := .
	LOCAL_LIB_DIR := .
endif #IS_DLL_ENTRY


## Corresponding absolute paths
# In source tree
SRC_PATH := ${LOCAL_PATH}/${SRC_DIR}
INC_PATH := ${LOCAL_PATH}/${INC_DIR}
# This one can be overwritten by packed libraries.
BINLIB_PATH ?= ${LOCAL_PATH}/${LIB_DIR}
# in build tree
DEPS_PATH := ${LOCAL_SHADOW_PATH}/${DEPS_DIR}
LIB_PATH := ${LOCAL_SHADOW_PATH}/${LIB_DIR}
OBJ_PATH := ${LOCAL_SHADOW_PATH}/${OBJ_DIR}
LOCAL_LIB_PATH := ${LOCAL_SHADOW_PATH}/${LOCAL_LIB_DIR}
#TODO REMOVE REL PATH
DEPS_REL_PATH := ${DEPS_PATH}/${CT_RELEASE}
OBJ_REL_PATH := ${OBJ_PATH}/${CT_RELEASE}

## Srecmap config files
ifeq "${CT_ASICSIMU}" "1"
	MAP_FILE := ${SOFT_WORKDIR}/env/compilation/${CT_ASIC}_asicsimu_map_cfg
else
	MAP_FILE := ${SOFT_WORKDIR}/env/compilation/${CT_ASIC}_map_cfg
endif # CT_ASICSIMU



###############################################################################
# Dependency treatment
###############################################################################
# Split LOCAL_MODULE_DEPENDS between SRC_LIBS and BINARY_LIBS.
# Use defined binary libs as default (e.g. from customvars.mk)
# Allow local append for binary libs, then local override for src libs, with src 
# superseding any conflicting binary setting

# Sometimes we want the build to be done in a particular order, eg. when
# the build of a module creates a public header file.
## DO NOT Remove any duplicate.
#LOCAL_MODULE_DEPENDS:= ${sort ${LOCAL_MODULE_DEPENDS}}

LOCAL_MODULE_DEPENDS_SORTED := ${sort ${LOCAL_MODULE_DEPENDS}}
ifneq "${words ${LOCAL_MODULE_DEPENDS_SORTED}}" "${words ${LOCAL_MODULE_DEPENDS}}"
$(warning ${LOCAL_NAME} has duplicate modules listed in LOCAL_MODULE_DEPENDS!)
endif

# ----------------------------------------------------------------------
# List all the modules that are declared as BINARY in the dependency list
# ----------------------------------------------------------------------
BINARY_LIBS := ${filter ${GLOBAL_BINARY_LIBS}  ${LOCAL_BINARY_LIBS},${LOCAL_MODULE_DEPENDS}}
# remove the ones that are localy declared as source module
BINARY_LIBS := ${filter-out ${LOCAL_SRC_LIBS},${BINARY_LIBS}}
# Add the user libraries
#BINARY_LIBS += ${LOCAL_LIB}


LOCAL_BUILT_BINARY_LIBS :=

#-----------------------------------------------------------------
# Optional Build
#-----------------------------------------------------------------
NULL_STRING :=
SPACE := $(NULL_STRING) # end of the line
DEPS_NOT_IN_SUBDIR :=

ifeq "$(DUMP_DEP_NOT_IN_SUBDIR)" "1"
DEPS_NOT_IN_SUBDIR := \
    $(strip \
        $(foreach MOD_DEP, $(LOCAL_MODULE_DEPENDS), \
            $(if $(findstring $(SPACE)$(LOCAL_NAME)/,$(SPACE)$(MOD_DEP)$(SPACE)),,$(MOD_DEP)) \
         ) \
     )
endif # DUMP_DEP_NOT_IN_SUBDIR == 1

# ----------------------------------------------------------------------
# List all the modules that are declared as must-be-built
# ----------------------------------------------------------------------
ifeq "$(UPD_TOP_LIB_ONLY)" "1"
# Top-level libs are generated at makelevel 1
ifeq "$(MAKELEVEL)" "1"
TOP_LIB_DEPENDS := \
    $(foreach MOD_DEP, $(LOCAL_MODULE_DEPENDS), \
        $(foreach MOD_BUILD, $(MODULES_MUST_BUILD), \
            $(if \
                $(or $(findstring $(SPACE)$(MOD_DEP)$(SPACE),$(SPACE)$(MOD_BUILD)$(SPACE)), \
                     $(findstring $(SPACE)$(MOD_DEP)/,$(SPACE)$(MOD_BUILD)$(SPACE)), \
                 ), \
                $(MOD_BUILD), \
                \
             ) \
         ) \
     )
TOP_LIB_DEPENDS := \
    $(subst $(SPACE)$(EXCEPTION_FAKE_SUBDIR_1)$(SPACE),$(SPACE)$(EXCEPTION_DEP_MOD_1)$(SPACE),$(SPACE)$(TOP_LIB_DEPENDS)$(SPACE))
TOP_LIB_DEPENDS := \
    $(subst $(SPACE)$(EXCEPTION_FAKE_SUBDIR_2)$(SPACE),$(SPACE)$(EXCEPTION_DEP_MOD_2)$(SPACE),$(SPACE)$(TOP_LIB_DEPENDS)$(SPACE))
# Build designated modules and archive them to the top lib
LOCAL_MODULE_DEPENDS := $(TOP_LIB_DEPENDS)
# All the prerequisite modules should be rebuilt and no need to check linkable modules any more
MODULES_MUST_BUILD :=
endif # MAKELEVEL == 1
endif # UPD_TOP_LIB_ONLY == 1

ifeq "${CT_COMPRESS_CODE_SECTION}" "yes"
ifeq "${LINK_STAGE_2}" "yes"
MODULES_MUST_BUILD := application/coolmmi/mmi/cdflashus
endif # LINK_STAGE_2
endif # CT_COMPRESS_CODE_SECTION

ifneq "$(MODULES_MUST_BUILD)" ""
# Deferred expansion for function definition
CHECK_SUBMOD_MUST_BUILD_FUNC = \
    $(if \
        $(strip \
            $(foreach MOD_BUILD,$(MODULES_MUST_BUILD), \
                $(if $(findstring $(SPACE)$(MOD_BUILD)/,$(SPACE)$(1)$(SPACE)),x,) \
             ) \
         ), \
         , \
         $(1) \
     )
LIBS_MUST_LINK := \
    $(foreach MOD_DEP, $(LOCAL_MODULE_DEPENDS), \
        $(if \
            $(or $(findstring $(SPACE)$(MOD_DEP)$(SPACE),$(SPACE)$(MODULES_MUST_BUILD)$(SPACE)), \
                 $(findstring $(SPACE)$(MOD_DEP)/,$(SPACE)$(MODULES_MUST_BUILD)$(SPACE)), \
             ), \
            , \
            $(call CHECK_SUBMOD_MUST_BUILD_FUNC,$(MOD_DEP)) \
         ) \
     )
LOCAL_BUILT_BINARY_LIBS += $(filter-out $(BINARY_LIBS),$(LIBS_MUST_LINK))
endif # MODULES_MUST_BUILD
#-----------------------------------------------------------------
# End of Optional Build
#-----------------------------------------------------------------

# ----------------------------------------------------------------------
# List all the modules that are declared as source
# ----------------------------------------------------------------------
SRC_LIBS := ${filter-out ${BINARY_LIBS} ${LOCAL_BUILT_BINARY_LIBS},${LOCAL_MODULE_DEPENDS}}

# ----------------------------------------------------------------------
# Locally some extra libries can be defined using LOCAL_LIB
# ----------------------------------------------------------------------
# The convention for LOCAL_LIB is to declare them starting from $SOFT_WORDIR
# The name of such a library must be of the form lib<my_librarary_name>.a
# ex: LOCAL_LIB := platform/thirdparty/thisprovider/libstuff.a

# ----------------------------------------------------------------------
#  Build library file list, path and linker script inclusion flag
# ----------------------------------------------------------------------
# Src libs path and files : in $BUILD_ROOT
SRC_LIBRARY_PATH := ${foreach MODULE_PATH, ${SRC_LIBS}, -L${BUILD_ROOT}/${MODULE_PATH}/${LIB_DIR} }
SRC_LIBRARY_FILES := ${foreach MODULE_PATH, ${SRC_LIBS}, ${BUILD_ROOT}/${MODULE_PATH}/${LIB_DIR}/lib${notdir ${MODULE_PATH}}_${CT_RELEASE}.a }

ifeq "${LOCAL_LIBS_FIRST}" "1"
LIBS_LINK_LOCAL += ${foreach MODULE_PATH, ${BINARY_LIBS}, \
                        ${if ${wildcard ${BUILD_ROOT}/${MODULE_PATH}/${LIB_DIR}/lib${notdir ${MODULE_PATH}}_${CT_RELEASE}.a}, \
                            ${MODULE_PATH} } }
LOCAL_BUILT_BINARY_LIBS += ${LIBS_LINK_LOCAL}
BINARY_LIBS := ${filter-out ${LIBS_LINK_LOCAL}, ${BINARY_LIBS}}
endif # LOCAL_LIBS_FIRST

BINARY_LIBRARY_PATH := 
BINARY_LIBRARY_FILES := 

# Local built binary libs path and files: in $BUILD_ROOT
BINARY_LIBRARY_PATH += ${foreach MODULE_PATH, ${LOCAL_BUILT_BINARY_LIBS}, -L${BUILD_ROOT}/${MODULE_PATH}/${LIB_DIR} }
BINARY_LIBRARY_FILES += ${foreach MODULE_PATH, ${LOCAL_BUILT_BINARY_LIBS}, ${BUILD_ROOT}/${MODULE_PATH}/${LIB_DIR}/lib${notdir ${MODULE_PATH}}_${CT_RELEASE}.a }

# Binary libs path and files : in $SOFT_WORKDIR
BINARY_LIBRARY_PATH += ${foreach MODULE_PATH, ${BINARY_LIBS}, -L${SOFT_WORKDIR}/${MODULE_PATH}/${LIB_DIR} }
BINARY_LIBRARY_FILES += ${foreach MODULE_PATH, ${BINARY_LIBS}, ${SOFT_WORKDIR}/${MODULE_PATH}/${LIB_DIR}/lib${notdir ${MODULE_PATH}}_${CT_RELEASE}.a }

# Local libs path and files : in $SOFT_WORKDIR
LOCAL_ADD_LIBRARY_PATH := ${foreach MODULE_PATH, ${LOCAL_LIB}, -L${SOFT_WORKDIR}/${dir ${MODULE_PATH}}}
# LOCAL_LIBS is already a file list:
#LOCAL_ADD_LIBRARY_FILES := ${foreach MODULE_PATH, ${LOCAL_LIB}, ${SOFT_WORKDIR}/${MODULE_PATH}/${notdir ${MODULE_PATH}}}
LOCAL_ADD_LIBRARY_FILES := ${foreach FILE_PATH, ${LOCAL_LIB}, ${SOFT_WORKDIR}/${FILE_PATH}}

# Full libraries path used for linking -L<path_to_library>
FULL_LIBRARY_PATH := ${SRC_LIBRARY_PATH} ${BINARY_LIBRARY_PATH} ${LOCAL_ADD_LIBRARY_PATH}
# List all library files for dependencies checking full_path+"lib"+libname.a
FULL_LIBRARY_FILES := ${SRC_LIBRARY_FILES} ${BINARY_LIBRARY_FILES} ${LOCAL_ADD_LIBRARY_FILES}
# List all libraries used for linking -l(no path, just names without prefix lib)
FULL_LIBRARY_EXT := ${foreach MODULE_PATH, ${FULL_LIBRARY_FILES}, -l${patsubst lib%,%,${basename ${notdir ${MODULE_PATH}}}}}

# Used when building a toplevel with submodules only : all object files from submodules that go into the lib
ifeq "$(IS_TOP_LEVEL)" "yes"
FULL_LIBRARY_OBJECTS := ${foreach lib, ${LOCAL_MODULE_DEPENDS}, ${BUILD_ROOT}/${lib}/${OBJ_DIR}/${CT_RELEASE}/*.o} 
endif

########################################################
## Recursive make calls 
########################################################
# When building a target, we need to build the libraries of the modules 
# declared as source first
SRC_DIRS := ${foreach MODULE_PATH, ${SRC_LIBS}, ${MODULE_PATH}}

# For all dependencies in SRC, rules to call make in dependency modules
FULL_DEPENDENCY_COMPILE := ${foreach SUBDIR, ${SRC_DIRS}, echo && ${ECHO} "MAKE              ${SUBDIR}" && ${MAKE} -C ${SOFT_WORKDIR}/${SUBDIR} all && } echo

FULL_DEPENDENCY_CLEAN := ${foreach SUBDIR, ${SRC_DIRS}, ${MAKE} -C ${SOFT_WORKDIR}/${SUBDIR} clean;}
FULL_DEPENDENCY_ALLCLEAN := ${foreach SUBDIR, ${SRC_DIRS}, ${MAKE} -C ${SOFT_WORKDIR}/${SUBDIR} allclean;}
FULL_DEPENDENCY_DEPCLEAN := ${foreach SUBDIR, ${SRC_DIRS}, ${MAKE} -C ${SOFT_WORKDIR}/${SUBDIR} depclean;}

#################################
## Include path generation ######
#################################
# LOCAL_API_DEPENDS
# list all the include from LOCAL_API_DEPENDS
DEPENDENCY_INCLUDE_PATH := ${foreach module, ${LOCAL_API_DEPENDS}, -I${SOFT_WORKDIR}/${module}/${INC_DIR} }

# ADD all the include from LOCAL_MODULE_DEPENDS 
# (if we depend on a module, we depend on its include also)
DEPENDENCY_INCLUDE_PATH += ${foreach module, ${LOCAL_MODULE_DEPENDS}, -I${SOFT_WORKDIR}/${module}/${INC_DIR} }

# List LOCAL_ADD_INCLUDE header retrieving path
MYINCLUDEDIR := ${foreach tmpDir, ${LOCAL_ADD_INCLUDE}, -I${SOFT_WORKDIR}/${tmpDir}}

# Root include directory
ROOT_INCLUDE	:= ${SOFT_WORKDIR}/${INC_DIR}

# Final include path
# ROOT_INCLUDE at the end because DEPENDENCY_INCLUDE_PATH must be allowed to supersede it (e.g. for sxs_type.h)
INCLUDE_PATH	:= ${MYINCLUDEDIR} -I${LOCAL_INC_DIR} -I${LOCAL_SRC_DIR} ${DEPENDENCY_INCLUDE_PATH}  -I${ROOT_INCLUDE}

###############################################################################
# ROM elf files
###############################################################################
# Select chip internal ROM elf file if exists.
# This is the description of the objects present in the ROM of the chip.
ifneq "${CHIP_ROM_ELF_FILE}" ""

CHIP_ROM_ELF_FILE_FULL  := ${SOFT_WORKDIR}/${CHIP_ROM_ELF_FILE}

# If we want to remove some objects from the ROM elf file.
# This can be useful if we don't want to link with some objects of the ROM.
ifneq "${STRIP_ROM_OBJ2KEEP_FILE}${STRIP_ROM_OBJ2STRIP_FILE}" ""
# Only Keep symbols specified in STRIP_ROM_OBJ2KEEP_FILE  
# or Only Strip symbols specified in STRIP_ROM_OBJ2STRIP_FILE 
# This file will be generated just before linking
# Path to generate the stripped ROM elf file.
CHIP_ROM_ELF_FILE_STRIPPED := ${BUILD_ROOT}/stripped_rom_elf_file.elf
STRIP_ROM_OBJ2KEEP_FILE_PATHED := ${SOFT_WORKDIR}/${STRIP_ROM_OBJ2KEEP_FILE}
STRIP_ROM_OBJ2STRIP_FILE_PATHED := ${SOFT_WORKDIR}/${STRIP_ROM_OBJ2STRIP_FILE}
USED_ROM_ELF_FILE:= ${CHIP_ROM_ELF_FILE_STRIPPED}
else # STRIP_ROM_OBJ2KEEP_FILE or STRIP_ROM_OBJ2STRIP_FILE
# use FULL ROM content
USED_ROM_ELF_FILE := ${CHIP_ROM_ELF_FILE_FULL}
endif # STRIP_ROM_OBJ2KEEP_FILE or STRIP_ROM_OBJ2STRIP_FILE

# There is a ROM in the chip that we fully or partially use, mention it to the linker
LDFLAG_USED_ROM_ELF_FILE := -just-symbols ${USED_ROM_ELF_FILE}

ifeq "${ROM_IN_FLASH_ELF}" "1"
ifneq "${CT_RAMRUN}" "1"
# Put rom contents into flash elf
LDFLAG_USED_ROM_ELF_FILE := ${USED_ROM_ELF_FILE}
endif # CT_RAMRUN
endif # ROM_IN_FLASH_ELF

else # CHIP_ROM_ELF_FILE == ""

# There is no ROM in the chip - No flag to describe the ROM in linker
LDFLAG_USED_ROM_ELF_FILE := 

endif # CHIP_ROM_ELF_FILE == ""

ifeq "${IS_DLL_ENTRY}" "yes"
LDFLAG_USED_ROM_ELF_FILE := 
endif
###################################################################################
# Linker script generation
###################################################################################
# The linker script is generated by preprocessing ${CT_ASIC}_master.ld by default
# An alternate linker source file can be specified using the LINKER_MASTER variable
# This is something more advanced, that takes the preprocessed output and modifies 
# it to generate a custom memory layout, according to env variables.

# Generate sector cleaner lods only
ifneq "${CLEAN_USER_SECTOR}${CLEAN_CALIB_SECTOR}${CLEAN_FACT_SECTOR}" "000"
override LINKER_MASTER := modem2G_sectorcleaner_master.ld
endif

# Select the source of the linker script with LINKER_MASTER
# If not specified we will use the default linker for the CT_ASIC
# If specified use an alternate linker script
ifeq "${LINKER_MASTER}" ""
LD_SRC := ${SOFT_WORKDIR}/env/link/${CT_ASIC}_master.ld
else
LD_SRC := ${SOFT_WORKDIR}/env/link/${LINKER_MASTER}
endif

# Filename of the Linker script generated on the fly
LD_FILE := ${BUILD_ROOT}/${CT_ASIC}.ld

ifneq "${USED_ROM_ELF_FILE}" ""
# Get the end address of the romed global variables in the BBSRAM, if a rom file is used.
# A equal ( = ) is used because BBSRAM_ROMED_GLOBALS_END needs to be evaluated after the stripped rom generation,
# in a rule below.
BBSRAM_ROMED_GLOBALS_END=${NM} ${USED_ROM_ELF_FILE} | grep _bb_sram_romed_globals_end | sed 's/^\([0-9a-f]*\).*/0xFFFFFFFF\1/'
else
BBSRAM_ROMED_GLOBALS_END=
endif

##################################################################################
## Linker flags
##################################################################################
OFORMAT := --oformat=elf32-littlemips
SREC		:= --input-target=elf32-littlemips --output-target=srec

CODE_BASE ?= 0x00000000
EXTRA_CODE_BASE ?= 0x007D0000
EXTRA_CODE_SIZE ?= 0x00000000

# Exports RAM/FLASH parameters to the linker script
# (Specified in memd/$(FLSH_MODEL}/memd.def or 
# overdefined in target/xxx/target.def)
LDPPFLAGS += \
			 -DUSER_DATA_BASE=${FLASH_BASE}+${USER_DATA_BASE} \
			 -DUSER_DATA_SIZE=${USER_DATA_SIZE} \
			 -DFLASH_SIZE=${FLASH_SIZE} \
			 -DRAM_SIZE=${RAM_SIZE} \
			 -DCALIB_BASE=${FLASH_BASE}+${CALIB_BASE} \
			 -DFACT_SETTINGS_BASE=${FLASH_BASE}+${FACT_SETTINGS_BASE} \
			 -DCODE_BASE=${FLASH_BASE}+${CODE_BASE} \
			 -DEXTRA_CODE_BASE=${FLASH_BASE}+${EXTRA_CODE_BASE} \
			 -DEXTRA_CODE_SIZE=${EXTRA_CODE_SIZE} \

# Export all the CHIP_EXPORT_FLAG to the linker, so that dedicated
# features of a chip that need a component of the linker script
# can enable those parts.
#
CHIP_EXPORT_FLAG := $(sort ${CHIP_EXPORT_FLAG})

LDPPFLAGS := ${LDPPFLAGS} ${foreach tmpFlag, ${CHIP_EXPORT_FLAG}, -D${tmpFlag}}

ifeq "${ROM_IN_FLASH_ELF}" "1"
ifneq "${CT_RAMRUN}" "1"
LDPPFLAGS += -DROM_IN_FLASH_ELF
endif # CT_RAMRUN
endif # ROM_IN_FLASH_ELF

# Exports CHIP Memory parameters to the linker script (specified in platform/chip/defs/xxx.def)
LDPPFLAGS += \
	-DFLASH_BASE=${FLASH_BASE} \
	-DEXT_RAM_BASE=${EXT_RAM_BASE} \
	-DSYS_SRAM_BASE=${SYS_SRAM_BASE} \
	-DSYS_SRAM_AVAILABLE_BASE=${SYS_SRAM_AVAILABLE_BASE} \
	-DSYS_SRAM_SIZE=${SYS_SRAM_SIZE} \
	-DRESERVED_MBX_SIZE=${RESERVED_MBX_SIZE} \
	-DSYS_SRAM_END=${SYS_SRAM_END} \
	-DSYS_SRAM_IRQ_STACKWATCH_SIZE=${SYS_SRAM_IRQ_STACKWATCH_SIZE} \
	-DSYS_PROXY_MEM_BASE=${SYS_PROXY_MEM_BASE} \
	-DSYS_PROXY_MEM_SIZE=${SYS_PROXY_MEM_SIZE} \
	-DBB_SRAM_BASE=${BB_SRAM_BASE} \
	-DBB_SRAM_SIZE=${BB_SRAM_SIZE} \
	-DBB_SRAM_GLOBALS_SIZE=${BB_SRAM_GLOBALS_SIZE} \
	-DXCPU_IRQ_STACK_SIZE=${XCPU_IRQ_STACK_SIZE} \
	-DINT_ROM_BASE=${INT_ROM_BASE} \
	-DINT_ROM_END=${INT_ROM_END} \
	-DROM_MISC_BASE=${ROM_MISC_BASE} \
	-DROM_MISC_UBASE=${ROM_MISC_UBASE} \
	-DPS_STACK_SIZE=${PS_STACK_SIZE} \
	-DBOOT_SECTOR_STRUCT_BASE=${BOOT_SECTOR_STRUCT_BASE} \

# Export Misc Parameters to the linker script.
LDPPFLAGS += \
	-DUSE_BINUTILS_2_19=${USE_BINUTILS_2_19} \
	-DDISCARD_UNLISTED_SECTIONS=${DISCARD_UNLISTED_SECTIONS} \

# Sector cleaner flags
ifneq "${CLEAN_USER_SECTOR}${CLEAN_CALIB_SECTOR}${CLEAN_FACT_SECTOR}" "000"
LDPPFLAGS += \
	-DCLEAN_USER_SECTOR=${CLEAN_USER_SECTOR} \
	-DCLEAN_CALIB_SECTOR=${CLEAN_CALIB_SECTOR} \
	-DCLEAN_FACT_SECTOR=${CLEAN_FACT_SECTOR} \
	
endif



ifneq "${CT_RAMRUN}" "1"
# Put Const into ROM
# Code is either in RAM or ROM
ifneq "${WITH_FLASH}" "1" 
LDPPFLAGS += -DCODE_IN_RAM
endif
else # CT_RAMRUN
# In case of RAMRUN, put all in ram : 
LDPPFLAGS += -DCONST_IN_RAM -DCODE_IN_RAM -DPRELOADED_INT_SRAM -DPRELOADED_RAM -DPRELOADED_PROXY_MEMORY -DRAMRUN
CFLAGS    += -DRAMRUN
endif # CT_RAMRUN

ifeq "${USE_BINUTILS_2_19}" "1"
ifeq "${ENABLE_GC_SECTIONS}" "1"
# Should be used along with GCC options -ffunction-sections, -fdata-sections
LDFLAGS += --gc-sections
ifeq "${PRINT_GC_SECTIONS}" "1"
LDFLAGS += --print-gc-sections
endif # PRINT_GC_SECTIONS
endif # ENABLE_GC_SECTIONS
endif # USE_BINUTILS_2_19

ifeq "${CT_USE_JAVA_LIB}" "1"
ifeq "${CT_USE_DEBUG_LIBGCJ}" "1"
GCJ_LIBFILE :=  -lj2me_debug -lgcjgc_debug -lfdlibm_debug
else
GCJ_LIBFILE :=  -lj2me_release -lgcjgc_release -lfdlibm_release
endif
ifneq "${IS_DLL_ENTRY}" "yes"
MYLIBFILES += ${GCJ_LIBFILE}
endif
LDPPFLAGS += -D__NATIVE_JAVA__
endif


ifeq "${CT_USE_LIBSTDC++}" "1"
LDFLAGS		+= ${FULL_LIBRARY_PATH} --start-group ${MYLIBFILES} ${FULL_LIBRARY_EXT} -lgcc -lstdc++ --end-group
LDFLAGS		+= -L${LIBGCC_PATH} 
else
LDFLAGS		+= ${FULL_LIBRARY_PATH} --start-group ${MYLIBFILES} ${FULL_LIBRARY_EXT} --end-group
LDFLAGS		+= -L${LIBGCC_PATH} -lgcc
endif
ifeq "${IS_DLL_ENTRY}" "yes"
LDFLAGS		+= --whole-archive --start-group ${MYLIBFILES} ${FULL_LIBRARY_EXT} --end-group --warn-unresolved-symbols -shared --export-dynamic
else
LDFLAGS		+= --warn-common  --error-unresolved-symbols 
endif

#--warn-section-align



##################################################################################
## Compile flags
# Some of those are exported recursively, other not.
##################################################################################

# Assembly compilation flags
ASFLAGS += -march=xcpu -mtune=xcpu -EL 

# Assembly preprocessing flags
ASCPPFLAGS  += -DCT_ASM
ifeq "${DLL_SUPPORT}" "yes"
ASCPPFLAGS  += -DCT_C_USE_GP
endif

# We disable the feature of assembling mips16 assembly code, 
# as assembling code with the --gstabs debug feature is 
# broken: mips16 code is interpreted as mips32, which leads
# to an exception.
# ie: Don't write mips16 assembly files.
# Since -gstab is removed, this limitation is not anymore...
#ifneq (,${findstring mips16,${CT_MIPS16_CFLAGS}})
# This file is mips16
#ASCPPFLAGS += -DCT_ASM_MIPS16
#endif # CT_MIPS16_CFLAGS

# C/C++ compilation flags

# User flags
# The local export flags are defined in the Makefile.
ALL_EXPORT_FLAG := $(sort ${ALL_EXPORT_FLAG} ${LOCAL_EXPORT_FLAG})

MYCPPFLAGS := ${MYCPPFLAGS} ${foreach tmpFlag, ${ALL_EXPORT_FLAG}, -D${tmpFlag}}



# ------------------------------------------------------------------------
# Depending on the release type, set the corresponding compilation flags
# ------------------------------------------------------------------------
ifeq "${CT_RELEASE}" "release"
MYCPPFLAGS += -DCT_NO_DEBUG
CT_OPT ?= speed
endif

ifeq "${CT_RELEASE}" "debug"
CT_OPT ?= dbg_opt
endif

ifeq "${CT_RELEASE}" "cool_profile"
MYCPPFLAGS += -DCT_PROFILING
CT_OPT ?= dbg_opt
endif

# Only useful for Jade, not for Modem2G.
ifeq "${CT_RELEASE}" "calib"
MYCPPFLAGS += -DPAL_CALIB
CT_OPT ?= dbg_opt
endif

# Old stuff for SA profiling, might not be used anymore.
ifeq "${CT_RELEASE}" "sa_profile"
MYCPPFLAGS += -DSA_PROFILING
CT_OPT ?= dbg_opt
endif



# ------------------------------------------------------------------------
# Warning flags
# ------------------------------------------------------------------------

ifneq "${WITHOUT_WSIGN_COMPARE}" "1"
ifeq "${WITH_WSIGN_COMPARE}" "1"
CFLAGS      += -Wsign-compare
endif
endif

ifneq "${WITHOUT_WERROR}" "1"
CFLAGS      += -Werror
endif

ifneq "${WITHOUT_WSHADOW}" "1"
CFLAGS      += -Wshadow
endif

ifeq "${USE_GCC_4}" "1"
# -finline-small-functions, by default on -O2 and -Os
#  makes code bigger. We disable it explicitly when
#  when we want small code.
#
# Nota: In the ROM we also don't want inlined function !
CFLAGS_EXTRA_SIZE_REDUCE := -fno-inline-small-functions -fno-inline-functions \
							-fno-align-functions  -fno-align-jumps -fno-align-loops -fno-align-labels 

# A good limit value for inlining function size: -finline-limit=6 

else
CFLAGS_EXTRA_SIZE_REDUCE := 
endif



# -----------------------------------
# Debug
# -----------------------------------
ifeq "${CT_OPT}" "dbg"
CFLAGS		+= -g
ASFLAGS		+= --gstabs
#MYCPPFLAGS	+=
else 
ifeq "${CT_OPT}" "dbg_opt"
CFLAGS		+= -O2 -g -falign-functions=16 -falign-jumps=16
#ASFLAGS		+= --gstabs
#MYCPPFLAGS	+=
else
ifeq "${CT_OPT}" "dbg_size"
CFLAGS		+= -Os -g  ${CFLAGS_EXTRA_SIZE_REDUCE}
else
ifeq "${CT_OPT}" "size"
CFLAGS		+= -Os  ${CFLAGS_EXTRA_SIZE_REDUCE}
#ASFLAGS    += 
#MYCPPFLAGS	+=
else #"${CT_OPT}" "speed"
CFLAGS		+= -O2 -falign-functions=16 -falign-jumps=16 
#ASFLAGS    += 
#MYCPPFLAGS	+=
endif # "size"
endif # "dbg_size"
endif # "dbg_opt"
endif # "dbg"

MYCPPFLAGS  += -D__NEW_GCC__

# We don't have a FPU, so we need to use the software implementation (with the
# integers units) of the floating point operations.
# Nota: we SHOULD implement algorithms using integers for perf issues
CFLAGS += -msoft-float
CFLAGS               += -Wunused-function
C_SPECIFIC_CFLAGS    += -std=gnu99

ifeq "${USE_GCC_4}" "1"
CFLAGS               += -Wuninitialized
CFLAGS += -minterlink-mips16 -fno-strict-aliasing
ifeq "${ENABLE_GC_SECTIONS}" "1"
# Should be used along with LD option --gc-sections 
CFLAGS += -ffunction-sections -fdata-sections
endif # ENABLE_GC_SECTIONS
# Create a warning when compiling C++ flags
C_SPECIFIC_CFLAGS += -Wno-pointer-sign 
C++_SPECIFIC_CFLAGS +=  -Wno-write-strings
# Discard conflicting builtin functions reimplemented by the code
CFLAGS += -fno-builtin-iswspace
# Might be useful for wide char stuff: -fno-builtin-iswalnum -fno-builtin-iswalpha -fno-builtin-iswascii -fno-builtin-iswcntrl -fno-builtin-iswdigit -fno-builtin-iswgraph -fno-builtin-iswlower -fno-builtin-iswprint -fno-builtin_iswpunct -fno-builtin-iswspace -fno-builtin-iswupper -fno-builtin-iswxdigit -fno-builtin-towlower -fno-builtin-towupper     and if scanf is detected, enable -fno-builtin-fscanf -fno-builtin-scanf -fno-builtin-sscanf 
MYCPPFLAGS +=  -DUSE_GCC_4=1 -DUSE_BINUTILS_2_19=1
else # USE_GCC_4 != "1"
# We're using GCC 3, with support for march/mtune flags, etc...
# which doesn't natively support interleaving of mips16 and mips32.
CFLAGS += -mfix-abijump
endif # USE_GCC_4 != "1"

# -----------------------------------
# Reduced regs management
# -----------------------------------
#  List of registers not used
REDUCED_REGS_FLAG ?= -ffixed-t3 -ffixed-t4 -ffixed-t5 -ffixed-t6 -ffixed-t7 -ffixed-s2 -ffixed-s3 -ffixed-s4 -ffixed-s5 -ffixed-s6 -ffixed-s7 -ffixed-fp

ifeq  "${WITH_REDUCED_REGS}" "1" 
MYCPPFLAGS +=  -D__REDUCED_REGS__ 
CFLAGS += ${REDUCED_REGS_FLAG}
endif # WITH_REDUCED_REGS

ifeq "${EXTRA_WARNING}" "1"
CFLAGS += -Wextra 
export EXTRA_WARNING
endif

# -----------------------------------
# Final values for compile flags
# -----------------------------------
CPPFLAGS	= ${INCLUDE_PATH} -DEL ${MYCPPFLAGS} -DBUILD_DATE=${BUILD_DATE} -DBUILD_TIME=${BUILD_TIME} -D_T_MMI=
CFLAGS += -G0 -Wall -march=xcpu -mtune=xcpu -Wa,-march=xcpu,-mtune=xcpu -EL -mexplicit-relocs -fweb -frename-registers -mmemcpy -mmips-tfile -nostartfiles -nostdlib -nostdinc -nodefaultlibs -c -pipe 

ifneq "${NO_WIDE_CHARSET}" "1"
CFLAGS += -fwide-exec-charset=UTF-16LE -fshort-wchar
endif

# ------------------------------------------------------------------------
# Export to recursive calls in dependencies
# ------------------------------------------------------------------------
export CT_RELEASE
export CT_TARGET
export CT_ASIC
export CT_ASIC_CAPD
export CT_ASIC_CFG
export CT_OS
export CT_ROMULATOR

# Exported flags and include path are exported.
# The global, target and chip export flags are not exported because
# they are included by all Makefiles, no need to export them.
export LOCAL_EXPORT_FLAG


###############################################################################
# Sources & Objects
###############################################################################
# No need to specify searching paths here, as
# 1. ${LOCAL_SRC_DIR} is the only valid location (a relative path) for
#    source files in implicit/pattern rules;
# 2. header files specified in .d are always with the full path;
# 3. nobody uses ${LOCAL_SUBDIRS};
# 3. vpath for object files will cause trouble in directory removal.
#    See the comment in target "locallclean".
#vpath %.c . ${LOCAL_SRC_DIR}
#vpath %.cpp . ${LOCAL_SRC_DIR}
#vpath %.S . ${LOCAL_SUBDIRS} ${LOCAL_SRC_DIR}
#vpath %.o 	${OBJ_REL_PATH}
#vpath %.h . ${LOCAL_SRC_DIR} ${LOCAL_INC_DIR}

# The list of objects is derived from the source files, 
# *plus* the contents of LOCAL_OBJS, so we can have e.g.
# a binary only main.o...

USED_SRC_IN_ROM ?= ${filter-out ${BYPASSED_SRC_IN_ROM}, ${SRC_IN_ROM} }

# Remove from the list of files to compile the files that are already in ROM
C_SRC_FILTERED := ${filter-out ${USED_SRC_IN_ROM}, ${C_SRC} }
S_SRC_FILTERED := ${filter-out ${USED_SRC_IN_ROM}, ${S_SRC} }
C++_SRC_FILTERED := ${filter-out ${USED_SRC_IN_ROM}, ${C++_SRC} }

ASFILES			:=	${patsubst %.S,%.asm,${S_SRC_FILTERED}}

# Full List of Objects that have to be generated from src (i.e.must be compiled)
FULL_SRC_OBJECTS :=	${LOCAL_OBJS} \
				    ${patsubst %.S,%.o,${S_SRC_FILTERED}}			\
				    ${patsubst %.c,%.o,${C_SRC_FILTERED}}			\
				    ${patsubst %.cpp,%.o,${C++_SRC_FILTERED}}		
FULL_SRC_OBJECTS := ${foreach obj, ${FULL_SRC_OBJECTS},${OBJ_REL_PATH}/${obj}}

# The list of objects that must be built regardless of the modification time
SRC_OBJECTS_FORCE_BUILD :=
ifneq "${LOCAL_SRC_FORCE_BUILD}" ""
ifneq "${filter-out ${USED_SRC_IN_ROM}, ${LOCAL_SRC_FORCE_BUILD}}" "${LOCAL_SRC_FORCE_BUILD}"
$(warning LOCAL_SRC_FORCE_BUILD contains ROM source files: ${LOCAL_SRC_FORCE_BUILD})
endif
ifneq "${filter-out ${C_SRC} ${S_SRC} ${C++_SRC}, ${LOCAL_SRC_FORCE_BUILD}}" ""
$(warning LOCAL_SRC_FORCE_BUILD contains useless files: ${LOCAL_SRC_FORCE_BUILD})
endif
SRC_OBJECTS_FORCE_BUILD := ${LOCAL_SRC_FORCE_BUILD}
SRC_OBJECTS_FORCE_BUILD := ${patsubst %.S,%.o,${SRC_OBJECTS_FORCE_BUILD}}
SRC_OBJECTS_FORCE_BUILD := ${patsubst %.c,%.o,${SRC_OBJECTS_FORCE_BUILD}}
SRC_OBJECTS_FORCE_BUILD := ${patsubst %.cpp,%.o,${SRC_OBJECTS_FORCE_BUILD}}
SRC_OBJECTS_FORCE_BUILD := ${foreach obj,${SRC_OBJECTS_FORCE_BUILD},${OBJ_REL_PATH}/${obj}}
endif # LOCAL_SRC_FORCE_BUILD

ifeq "${CT_COMPRESS_CODE_SECTION}" "yes"
ifeq "${CYGWIN}" "1"
LZMA = ${SOFT_WORKDIR}/application/mmi_customer/ResGenerator/compress/lzma/lzma.exe
else
LZMA = wine ${SOFT_WORKDIR}/application/mmi_customer/ResGenerator/compress/lzma/lzma.exe
endif
LDPPFLAGS += -DCT_COMPRESS_CODE_SECTION
LDPPFLAGS += -DXCPU_OVERLAY_BASE=${EXT_RAM_BASE}+${XCPU_OVERLAY_BASE}
CFLAGS += -D__MMI_COMPRESS_CODE_SECTION__
#LDPPFLAGS += -DXCPU_OVERLAY
ifeq "${LINK_STAGE_2}" "yes"
LDPPFLAGS += -DLINK_STAGE_2
endif # LINK_STAGE_2
endif # CT_COMPRESS_CODE_SECTION

##############################################################################
## General Targets
##############################################################################
LOCAL_LIBRARY_NOTDIR := lib${MODULE_NAME}_${CT_RELEASE}.a
LOCAL_SRCLIBRARY := ${LIB_PATH}/${LOCAL_LIBRARY_NOTDIR}
# This one can be overwritten by packed libraries.
LOCAL_BINLIBRARY ?= ${BINLIB_PATH}/${LOCAL_LIBRARY_NOTDIR}

# Allow name suffixing for all final binary targets, to keep history/variations.
ifneq "$(LOCAL_BATCH_NAME)" ""
NBR := ${LOCAL_BATCH_NAME}_
else 
NBR :=
endif # LOCAL_BATCH_NAME

ifneq "$(CT_PRODUCT)" ""
PRODUCT := ${CT_PRODUCT}_
else
PRODUCT :=
endif # CT_PRODUCT

# Build the base pathname of the generated lodfile.
ifneq "$(CT_APP)" ""
ifeq "$(CT_APP)" "_" #special name for Total_integration behavior
LODBASE_NO_PATH := ${NBR}${CT_TARGET}_${PRODUCT}${CT_RELEASE}
else
LODBASE_NO_PATH := ${CT_APP}_${NBR}${CT_TARGET}_${PRODUCT}${CT_RELEASE}
endif
else
LODBASE_NO_PATH := $(notdir ${LOCAL_NAME})_${NBR}${CT_TARGET}_${PRODUCT}${CT_RELEASE}
endif

# Path for storing all the generated files for one test (elf, dis, lod...).
BINARY_PATH := ${HEX_PATH}/${LODBASE_NO_PATH}


# Change the location where everything is compiled.
BIN_PATH := ${BUILD_ROOT}/${LOCAL_NAME}

BAS := ${BIN_PATH}/${LODBASE_NO_PATH}
BIN := ${BAS}.elf
MAP := ${BAS}.map
HEX := ${BAS}.srec
DIS := ${BAS}.dis
DLL := ${BAS}.dll
BAS_FINAL := ${BINARY_PATH}/${LODBASE_NO_PATH}
BIN_FINAL := ${BAS_FINAL}.elf
MAP_FINAL := ${BAS_FINAL}.map
HEX_FINAL := ${BAS_FINAL}.srec
DIS_FINAL := ${BAS_FINAL}.dis
XMD_FINAL := ${BAS_FINAL}.xmd
XCP_FINAL := ${BAS_FINAL}.xcp
DLL_FINAL := ${BAS_FINAL}.dll
LODBASE   := ${BAS_FINAL}_

BUILD_XCP_PATH := ${BUILD_ROOT}/xcp

EXP := ${LOCAL_DLL_EXPORT}

# The default target is either a library or a srec file, depending on IS_ENTRY_POINT
ifeq "${IS_ENTRY_POINT}" "yes"
	TOP_TARGET := ${HEX}
else
	TOP_TARGET := ${LOCAL_SRCLIBRARY}
endif # IS_ENTRY_POINT

ifeq "${IS_DLL_ENTRY}" "yes"
	TOP_TARGET := ${DLL}
endif # IS_DLL_ENTRY

# List of directories that may need to be created
ifneq "${IS_ENTRY_POINT}" "yes"
MAKE_DIRS := ${OBJ_REL_PATH} ${DEPS_REL_PATH} ${BUILD_XCP_PATH} ${LOCAL_LIB_PATH}
else
MAKE_DIRS := ${OBJ_REL_PATH} ${DEPS_REL_PATH} ${BUILD_XCP_PATH}
endif # IS_ENTRY_POINT

########################## Default Target ###################################
.PHONY: default

ifeq "${IS_DLL_ENTRY}" "yes"
default: dll
else
ifeq "${IS_ENTRY_POINT}" "yes"
default: lod
else
default: all
endif
endif


.PHONY: force
force: ;

# Avoid searching implicit rules for makefiles, as we do not re-generate them
Makefile: ;
%.mk: ;
%.def: ;
%.srcs: ;
%.d: ;

# Avoid searching implicit rules for source files, as we do not generate
# them automatically
%.cpp: ;
%.c: ;
%.S: ;

# Avoid searching implicit rules for header files. Some of them will be generated
# during xmd2h, xcp2h, or updateversion process. However, they are not listed
# as targets in a rule in xmd2h and updateversion; and in xcp2h they are listed
# as targets in an ordinary rule (not an implicit/pattern rule), e.g., ${XCP_OUTPUT_FILE}.
# Since an ordinary rule with commands will take precedence of any implicit/pattern rule,
# we can safely stop searching implicit rules for all these files.
%.h: ;

# Avoid searching implicit rules for XCP files. They are listed as targets in an
# ordinary rule during xcp2h process, e.g., ${XCP_BUILD_FILE} and ${XCP_FINAL}.
# Since an ordinary rule with commands will take precedence of any implicit/pattern rule,
# we can safely stop searching implicit rules for all these files.
%.xcp: ;

#####################################################################
# Error management
#####################################################################
.PHONY: my_err
my_err: 
	$(error Make ERROR)


########################## Directories #######################################

${LOCAL_LIB_PATH}:
	mkdir -p ${LOCAL_LIB_PATH}

ifneq "${BINLIB_PATH}" "${LOCAL_LIB_PATH}"
${BINLIB_PATH}:
	mkdir -p ${BINLIB_PATH}
endif
    
${OBJ_REL_PATH}:
	mkdir -p ${OBJ_REL_PATH}

${BINARY_PATH}:
	mkdir -p ${BINARY_PATH}

${DEPS_REL_PATH}:
	mkdir -p ${DEPS_REL_PATH}

${BIN_PATH}:
	mkdir -p ${BIN_PATH}

${BUILD_XCP_PATH}:
	mkdir -p ${BUILD_XCP_PATH}

########################### Cleaning-up ######################################
.PHONY: locdepclean locclean locallclean hexclean allclean clean depclean realclean

# Remove depfiles for the *current* board only
locdepclean:
	@${ECHO} "DEPCLEAN          ${LOCAL_NAME}"
# With the -f option, rm will return 0 even if there is no .d file
	rm -f ${DEPS_REL_PATH}/*.d

# Remove local library if any, and any objects generated from source
locclean: xcpfinalclean
	@${ECHO} "CLEAN             ${LOCAL_NAME}"
ifneq "${IS_ENTRY_POINT}" "yes"
	${foreach file, ${wildcard ${OBJ_REL_PATH}/*}, rm -fr ${file};}
	rm -f ${LOCAL_SRCLIBRARY}
else
	rm -f ${filter-out ${foreach obj,${LOCAL_OBJS},${OBJ_REL_PATH}/${obj}}, ${FULL_SRC_OBJECTS}}
	# Remove files in the shadow.
	rm -f ${HEX} ${MAP} ${BIN}
	# Remove files in hex.
	rm -f ${HEX_FINAL} ${MAP_FINAL} ${BIN_FINAL} ${LODBASE}*.lod ${XCP_FINAL} ${XMD_FINAL} ${DIS_FINAL}
endif # IS_ENTRY_POINT


# Remove objects and depfiles for *all* targets
locallclean: 
	@${ECHO} "ALLCLEAN         ${LOCAL_NAME}"
# NOTE:
# make will open the file handles for the directories given in vpath.
# In case of a directory removal (allclean), the "vpath %.o ${OBJ_REL_PATH}"
# statement will certainly block the removal operation for the object path:
# the "rm -fr xxx" command results in "Directory not empty".
ifeq "${CLEAN_ALL_RELEASE}" "1"
	rm -fr ${OBJ_PATH} ${DEPS_PATH} ${LIB_PATH}
else
	rm -fr ${OBJ_REL_PATH} ${DEPS_REL_PATH} ${LOCAL_SRCLIBRARY}
endif # CLEAN_ALL_RELEASE

# Remove all .srec, .dis, .map and elf files with the directory prefix, 
# regardless of the suffix (LOCAL_BATCH_NAME)
# In fact, we remove all files with the matching prefix, except those that could be source
# Use with care !
ifeq "" "${notdir ${LOCAL_NAME}}"
hexclean:
	$(error "Bad LOCAL_NAME definition, should not end with '\' Not doing the clean to avoid loosing files.")
else
hexclean:
	rm -f ${wildcard ${BIN_PATH}/${notdir ${LOCAL_NAME}}*}
endif # LOCAL_NAME

# Same targets, but recursive
allclean: locallclean
# The '+' token works like ${MAKE} directly in the line content.
	+@${FULL_DEPENDENCY_ALLCLEAN}

clean: locclean
# The '+' token works like ${MAKE} directly in the line content.
	+@${FULL_DEPENDENCY_CLEAN}

depclean: locdepclean
# The '+' token works like ${MAKE} directly in the line content.
	+@${FULL_DEPENDENCY_DEPCLEAN}

realclean: clean depclean xcprealclean

##################################################################################



########################## Generate the documentation #############################

DOC_MODULE_NAME ?= ${MODULE_NAME}

# Specific Doxygen Configuration Items
DOC_PROJECT_NAME := `perl -e 'print "\\U${DOC_MODULE_NAME}"'`
#DOC_PROJECT_NAME := `ruby -e "puts \\"${DOC_MODULE_NAME}\\".upcase"`

# Doc is generated in this directory
DOC_OUTPUT_DIR ?= ${SOFT_WORKDIR}/${LOCAL_NAME}/doc/
DOCFULL_OUTPUT_DIR ?= ${SOFT_WORKDIR}/${LOCAL_NAME}/docfull/
DOCMODEL_OUTPUT_DIR ?= ${SOFT_WORKDIR}/${LOCAL_NAME}/docmodel/

# The tag containing the doc info will be
DOC_GENERATED_TAG ?= ${SOFT_WORKDIR}/${LOCAL_NAME}/doc/${DOC_MODULE_NAME}.tag
DOCFULL_GENERATED_TAG ?= ${SOFT_WORKDIR}/${LOCAL_NAME}/docfull/${DOC_MODULE_NAME}.tag
DOCMODEL_GENERATED_TAG ?= ${SOFT_WORKDIR}/${LOCAL_NAME}/docmodel/${DOC_MODULE_NAME}.tag

# Input for doxygen (only headers, sources, module dependencies, etc)
ifeq "${DOCUMENT_DEPENDENCIES}" "1"
	# Input files come from $LOCAL_API_DEPENDS and $EXTRA_DOC_INPUTS.
    DOC_INPUT_MODULES := $(sort ${LOCAL_API_DEPENDS} ${EXTRA_DOC_INPUTS})

	# Headers of the dependencies are copied in a tmp dir to edit the paging
	# The local headers still need to be added manually.
    SOFTDOC_INPUT := ${LOCAL_PATH}/include \
					${foreach MODULE_NAME, ${DOC_INPUT_MODULES}, ${SOFT_WORKDIR}/$(MODULE_NAME)/${INC_DIR}}
	SOFTDOCFULL_INPUT := $(SOFTDOC_INPUT) ${foreach MODULE_NAME, ${DOC_INPUT_MODULES},\
                    ${SOFT_WORKDIR}/$(MODULE_NAME)}  
else
	# Document only the current module and any extra the user like.
    SOFTDOC_INPUT := ${LOCAL_PATH}/include \
					${foreach MODULE_NAME, ${EXTRA_DOC_INPUTS}, ${SOFT_WORKDIR}/$(MODULE_NAME)/${INC_DIR}}	
    SOFTDOCFULL_INPUT := ${LOCAL_PATH} \
					${foreach MODULE_NAME, ${EXTRA_DOC_INPUTS},${SOFT_WORKDIR}/$(MODULE_NAME)}	
endif


.PHONY: doc docfull doc_release docfull_release docmodel

doc:
	@${ECHO} "DOXYGEN           $(LOCAL_NAME)"
	mkdir -p ${SOFT_WORKDIR}/doc/files
	# Create specific configuration file holding specific items: current.doxy
	# Ok, that could be better...
	# NOTA: Use XMD_ALL_EXPORT_FLAG as we need to escape the " characters herre too.
	echo -e "\n\
             \t PROJECT_NAME = ${DOC_PROJECT_NAME} \n\
             \t INPUT = ${SOFTDOC_INPUT} \n\
             \t HTML_OUTPUT = files \n\
             \t OUTPUT_DIRECTORY = ${DOC_OUTPUT_DIR} \n\
             \t TAGFILES = ${DOC_TAG_LIST} \n\
             \t PREDEFINED = ${XMD_ALL_EXPORT_FLAG} \n\
             \t GENERATE_TAGFILE = ${DOC_GENERATED_TAG} \n" > ${SOFT_WORKDIR}/env/docgen/current.doxy
	# Create doc.
	doxygen ${SOFT_WORKDIR}/env/docgen/DoxyApi ${STDOUT_NULL}
	# Copy missing items (images...) and create accessor.
	cp $(SOFT_WORKDIR)/env/docgen/style/coolsand.png ./doc/files/
	cp $(SOFT_WORKDIR)/env/docgen/style/template.html ./doc/${DOC_MODULE_NAME}.html
	sed -i"" -e 's/XXX_MODULE_NAME_XXX/files/g' ./doc/${DOC_MODULE_NAME}.html


docfull:
	@${ECHO} "DOXYGEN           $(LOCAL_NAME)"
	mkdir -p ${SOFT_WORKDIR}/docfull/files
	# Create specific configuration file holding specific items: current.doxy
	# Ok, that could be better...
	echo -e "\n\
             \t PROJECT_NAME = ${DOC_PROJECT_NAME} \n\
             \t INPUT = ${SOFTDOCFULL_INPUT} \n\
             \t HTML_OUTPUT = files \n\
             \t OUTPUT_DIRECTORY = ${DOCFULL_OUTPUT_DIR} \n\
             \t TAGFILES = ${DOCFULL_TAG_LIST} \n\
             \t PREDEFINED = ${XMD_ALL_EXPORT_FLAG} \n\
             \t GENERATE_TAGFILE = ${DOCFULL_GENERATED_TAG} \n" > ${SOFT_WORKDIR}/env/docgen/current.doxy
	# Create doc.
	doxygen ${SOFT_WORKDIR}/env/docgen/DoxyFull ${STDOUT_NULL}
	# Copy missing items (images...) and create accessor.
	cp $(SOFT_WORKDIR)/env/docgen/style/coolsand.png ./docfull/files/
	cp $(SOFT_WORKDIR)/env/docgen/style/template.html ./docfull/${DOC_MODULE_NAME}.html
	sed -i"" -e 's/XXX_MODULE_NAME_XXX/files/g' docfull/${DOC_MODULE_NAME}.html


DOC_RELEASE_DIR ?= /n/DOC/Public/Modem2G/APIs
DOC_FULL_RELEASE_DIR ?= /n/projects/Granite/DOC/APIs_full


## ------------------------------------------------------------------------- ##
##          Documentation release rules
## ------------------------------------------------------------------------- ##
doc_release:
	$(MAKE) doc
	@${ECHO} "DOXYGEN           Copy documentation"
	rm -rf ${DOC_RELEASE_DIR}/${DOC_MODULE_NAME}*
	cp -r   ${SOFT_WORKDIR}/${LOCAL_NAME}/doc/files ${DOC_RELEASE_DIR}/${DOC_MODULE_NAME}
	# Create accessor
	cp $(SOFT_WORKDIR)/env/docgen/style/template.html ${DOC_RELEASE_DIR}/${DOC_MODULE_NAME}.html
	sed -i"" -e 's/XXX_MODULE_NAME_XXX/${DOC_MODULE_NAME}/g' ${DOC_RELEASE_DIR}/${DOC_MODULE_NAME}.html
	${ECHO} Done!


docfull_release:
	$(MAKE) docfull
	@${ECHO} "DOXYGEN           Copy full documentation"
	rm -rf ${DOC_FULL_RELEASE_DIR}/${DOC_MODULE_NAME}*
	cp -r   ${SOFT_WORKDIR}/${LOCAL_NAME}/docfull/files ${DOC_FULL_RELEASE_DIR}/${DOC_MODULE_NAME}
	# Create accessor
	cp $(SOFT_WORKDIR)/env/docgen/style/template.html ${DOC_FULL_RELEASE_DIR}/${DOC_MODULE_NAME}.html
	sed -i"" -e 's/XXX_MODULE_NAME_XXX/${DOC_MODULE_NAME}/g' ${DOC_FULL_RELEASE_DIR}/${DOC_MODULE_NAME}.html
	${ECHO} Done!


ifeq ($ISMODELS,YES)
docmodel:
	@${ECHO} "DOXYGEN           $(LOCAL_NAME)"
	# Create specific configuration file holding specific items: current.doxy
	# Ok, it couldn't be more beautifull, mon buddy ...
	echo -e "\n\
	    \t PROJECT_NAME = ${DOC_PROJECT_NAME} \n\
	    \t HTML_OUTPUT = files \n\
	    \t INPUT = ${DOCMODEL_INPUT} \n\
	    \t OUTPUT_DIRECTORY = ${DOCMODEL_OUTPUT_DIR} \n\
	    \t TAGFILES = ${DOCMODEL_TAG_LIST} \n\
	    \t GENERATE_TAGFILE = ${DOCMODEL_GENERATED_TAG} \n" > ${SOFT_WORKDIR}/env/docgen/current.doxy
	mkdir -p ${SOFT_WORKDIR}/${LOCAL_NAME}/docmodel/files
	# Create Doc
	doxygen ${SOFT_WORKDIR}/env/docgen/DoxyModel ${STDOUT_NULL}
	# Copy missing items (images...) and create accessor    
	cp $(SOFT_WORKDIR)/env/docgen/style/coolsand.png ./docmodel/files/
	cp $(SOFT_WORKDIR)/env/docgen/style/template.html ./docmodel/${DOC_MODULE_NAME}.html
	sed -i"" -e 's/XXX_MODULE_NAME_XXX/files/g' ./docmodel/${DOC_MODULE_NAME}.html
else
docmodel:
	echo "There are no models for ${DOC_MODULE_NAME} module."
endif

##################################################################################



##################### Generate the overlay linkers ###############################
OVERLAY_NUMBER_PER_LOCATION := 6

# notes about code bellow
# $(call FUNC, params...) evaluate FUNC which must be defined as =, replace $(1) ... with params
# $(value NAME) -> $(NAME)
# $(eval block) return empty string and to eval of block

# function OVERLAY_SECTION_ITEM parameters
# $(1) = content -> toto.o titi.o
# $(2) = section catch -> .text .text.* .sramtext
OVERLAY_SECTION_ITEM = $(foreach object, $1, "            *$(object)($(2))\n")

OVERLAY_TEXT_SECTIONS := .text .text.* .sramtext
OVERLAY_DATA_SECTIONS := .rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*
OVERLAY_BSS_SECTIONS := .bss .bss.* .srambss COMMON .sbss .sbss.* .sbss2 .sbss2.*

# function OVERLAY_INSTANCE_GEN_FUNCTION parameters
# $(1) = baseName -> sys_sram_overlay
# $(2) = Base of the Makefile variable hodling the symbols to include for 'i' -> INT_SRAM_OVERLAY_CONTENT_
# $(3) = numberOfOverlays -> 2
OVERLAY_INSTANCE_GEN_FUNCTION = $(eval OVERLAY_BUILDING_SIZE := 0)\
	$(eval OVERLAY_MAX_SIZE := 0) \
	$(foreach i,$(shell SimpleSeq.sh 0 `expr $(3) - 1`), \
	$(eval OVERLAY_BUILDING_SIZE += + SIZEOF(.$(1)_$(i)))\
	$(eval OVERLAY_MAX_SIZE = MAX(SIZEOF(.$(1)_$(i)), $(OVERLAY_MAX_SIZE)))\
	"        .$(1)_$(i)\n"\
	"        {\n"\
				$(call OVERLAY_SECTION_ITEM,$(value $(2)$(i)),$(OVERLAY_TEXT_SECTIONS))\
				$(call OVERLAY_SECTION_ITEM,$(value $(2)$(i)),$(OVERLAY_DATA_SECTIONS))\
				$(call OVERLAY_SECTION_ITEM,$(value $(2)$(i)),$(OVERLAY_BSS_SECTIONS))\
	"            \#if (USE_BINUTILS_2_19 == 1) \n" \
	"            /* FIXME: LD 2.19 doesn't like empty overlays... */\n" \
	"            LONG(0)\n" \
	"            \#endif \n" \
	"            . = ALIGN(0x10);\n"\
	"        }\n\n")\
	"        \#define FLASH_SIZE_OF_$(shell perl -e 'print "\U$(1)"') $(OVERLAY_BUILDING_SIZE)\n"\
	"        \#define MAX_SIZE_OF_$(shell perl -e 'print "\U$(1)"') $(OVERLAY_MAX_SIZE)\n"

 

# search path for preprocessor to include genrated parts
LDPPFLAGS += -I${BUILD_ROOT}

.PHONY: overlayLinkerIntSram overlayLinkerFastCpuMemory overlaylinkers

# build INT SRAM overlay linker file
overlayLinkerIntSram:
	echo -e $(call OVERLAY_INSTANCE_GEN_FUNCTION,sys_sram_overlay,INT_SRAM_OVERLAY_CONTENT_,$(OVERLAY_NUMBER_PER_LOCATION)) > ${BUILD_ROOT}/int_sram_overlay.ld

# build Fast Cpu Memory overlay linker file
overlayLinkerFastCpuMemory:
	echo -e $(call OVERLAY_INSTANCE_GEN_FUNCTION,fast_cpu_memory_overlay,FAST_CPU_MEMORY_OVERLAY_CONTENT_,$(OVERLAY_NUMBER_PER_LOCATION)) > ${BUILD_ROOT}/fast_cpu_memory_overlay.ld



# all overlays
overlaylinkers: overlayLinkerIntSram overlayLinkerFastCpuMemory

##################################################################################
## Clean the disassembly rule. (Part of the lod creation process, as a new lod
## implies that any previous disassembly becomes false and irrelevant.
##################################################################################
.PHONY: clean_disassembly
clean_disassembly:
	rm -f ${DIS_FINAL}

##################################################################################

##################################################################################


########################## Generate the CoolProfile headers ######################

XCP_INPUT_NAME     := *_profile_codes.xcp
XCP_INPUT_DIR      := ${LOCAL_INC_DIR}
XCP_INPUT_FILE     := $(wildcard ${XCP_INPUT_DIR}/${XCP_INPUT_NAME})
XCP_INPUT_ABS_PATH := ${WIN_SOFT_WORKDIR}/${LOCAL_NAME}/${XCP_INPUT_FILE}

# Create the output file by changing the extension of the input file.
XCP_OUTPUT_DIR     := ${LOCAL_INC_DIR}
XCP_OUTPUT_FILE    := $(patsubst %.xcp,%.h,${XCP_INPUT_FILE})
XCP_OUTPUT_ABS_DIR := ${WIN_SOFT_WORKDIR}/${LOCAL_NAME}/${XCP_OUTPUT_DIR}

XCP_BUILD_NAME     := $(notdir ${XCP_INPUT_FILE})
XCP_BUILD_FILE     := ${BUILD_XCP_PATH}/${XCP_BUILD_NAME}

XCP_DEPENDS_INPUTS := $(wildcard ${BUILD_XCP_PATH}/*.xcp)

XCP_PROCESS := @${ECHO} "XCP2H             $(notdir $(XCP_INPUT_FILE))"; \
		xcp2h -i ${XCP_INPUT_ABS_PATH} \
		-o ${XCP_OUTPUT_ABS_DIR} \
        -u ${STDOUT_NULL}

.PHONY: xcp2h xcprealclean xcpfinalclean xcpclean

# Generate the CoolProfile header from the XCP file, when asked.
xcp2h: ${XCP_OUTPUT_FILE}

ifneq (${XCP_INPUT_FILE},)
${XCP_OUTPUT_FILE}: ${XCP_INPUT_FILE}
#	@${ECHO} "XCP OUTPUT    $(XCP_OUTPUT_FILE)"
	$(XCP_PROCESS)
endif

xcprealclean:
	@${ECHO} "XCPREALCLEAN"
	rm -rf ${BUILD_XCP_PATH}

# Generate automatically the CoolProfile header from the XCP file for a given
# software module. This is define in the module's Makefile.
# Default value is no.
ifeq "${AUTO_XCP2H}" "yes"
XCP2H := xcp2h

ifneq (${XCP_INPUT_FILE},)
# Also generate XCP_BUILD_FILE during XCP process
xcp2h: ${XCP_BUILD_FILE}
${XCP_BUILD_FILE}: ${XCP_INPUT_FILE} | ${BUILD_XCP_PATH}
#	@${ECHO} "XCP BUILD     $(XCP_BUILD_FILE)"
	cp ${XCP_INPUT_FILE} $@
endif

xcpfinalclean:
ifneq (${XCP_INPUT_FILE},)
#	@${ECHO} "XCPFINALCLEAN     $(XCP_BUILD_FILE)"
	rm -f ${XCP_BUILD_FILE}
endif

# CoolProfile header files from XCP are also committed in SVN database,
# and there is no need to remove them when cleaning objects.
xcpclean:
#ifneq (${XCP_INPUT_FILE},)
##	@${ECHO} "XCPCLEAN     $(XCP_OUTPUT_FILE)"
#	rm -f ${XCP_OUTPUT_FILE}
#endif

else  # AUTO_XCP2H = no
XCP2H :=

xcpfinalclean:

xcpclean:

endif # AUTO_XCP2H = no

# Generate the CoolProfile packed file from the XCP file.
${XCP_FINAL}: $(XCP_DEPENDS_INPUTS) ${BINARY_PATH} force
ifneq ($(strip $(XCP_DEPENDS_INPUTS)),)
	@${ECHO}
	@${ECHO} "XCPPACK           $(notdir $(XCP_FINAL))"
	rm -f $@
	echo "<cpconfig>" >> $@
	cat ${XCP_DEPENDS_INPUTS} >> $@
	echo "</cpconfig>" >> $@
endif # XCP_DEPENDS_INPUTS

##################################################################################



########################## Create a .h file with the SVN revision number 

MOD_LOWCASE := ${MODULE_NAME}
MOD_CMD := perl -e 'print "\U${MOD_LOWCASE}"'
ifeq "${WITH_SVN}" "0"
LOCAL_REVISION_CMD := echo -1
LOCAL_BRANCH_CMD := echo "none"
else
LOCAL_REVISION_CMD := svn info ${LOCAL_PATH} | grep "Last Changed Rev" | sed 's/.*: //'
LOCAL_BRANCH_CMD := svn info ${LOCAL_PATH} | grep "^URL" | cut -d '/' -f 6-
endif

# Default value is no.
ifeq "${AUTO_GEN_REVISION_HEADER}" "yes"
UPDATE_VERSION_HEADER := updateversion
ifeq "${IS_ENTRY_POINT}" "yes"
REV_FILE := ${MOD_LOWCASE}p_version.h
else # ! IS_ENTRY_POINT
IS_ENTRY_POINT := no
REV_FILE := ${SRC_PATH}/${MOD_LOWCASE}p_version.h
endif # ! IS_ENTRY_POINT
REV_SKEL_FILE := ${SOFT_WORKDIR}/env/compilation/mod_version_skel.h
REGISTERED_REV_CMD := sed -e 's/.*VERSION_REVISION *(\([[:alnum:]]*\)).*/\1/p' -e 'd' ${REV_FILE}
endif # "${AUTO_GEN_REVISION_HEADER}" "yes"

.PHONY: updateversion
updateversion:
# REGISTERED_REV is defined only if the file already exists.
# If it doesn't exist, or the value is different, the file is
# created, which will lead to the update of the revision file.
	if [ -d ${INC_PATH} ]; then														\
	    if [ "${IS_ENTRY_POINT}" != "yes" -a ! -d ${SRC_PATH} ]; then				\
            mkdir ${SRC_PATH};														\
        fi;																			\
	    if [ -f ${REV_FILE} ]; then													\
	    	REGISTERED_REV=`$(REGISTERED_REV_CMD)`;									\
	    fi;																			\
	    LOCAL_REVISION=`$(LOCAL_REVISION_CMD)`;										\
	    LOCAL_BRANCH=`$(LOCAL_BRANCH_CMD) | sed -e 's/\\//\\\\\\//g'`;				\
	    MOD=`$(MOD_CMD)`;															\
	    if [[ "$$REGISTERED_REV" != "$$LOCAL_REVISION" ]]; then						\
	       	${ECHO} "REVGEN            `basename ${REV_FILE}` @ r$$LOCAL_REVISION";	\
	    	sed -e "s/@{MOD}/$$MOD/g"												\
	    	    -e "s/@{LOCAL_REVISION}/$$LOCAL_REVISION/g"							\
	    	    -e "s/@{LOCAL_BRANCH}/\"$$LOCAL_BRANCH\"/g" ${REV_SKEL_FILE} > ${REV_FILE};			\
	    fi;																			\
	fi;																			

##################################################################################



############### Generate the CoolWatcher memory description headers ##############

# Only keeps the flags containing an "=" character.
XMD_ALL_EXPORT_FLAG := ${subst ",\", ${ALL_EXPORT_FLAG}}
# "} # just to fix vim's coloring that beleave we are in a string there...
XMD_PROCESS := xmd2h.sh "${VERBOSE}" "${XMD_ALL_EXPORT_FLAG}"
XMD_MAP_DIR := toolpool/map/${CT_ASIC_CAPD}
XMD_ROOT_ABS := ${WIN_SOFT_WORKDIR}/${XMD_MAP_DIR}/${CT_ASIC}_soft_root.xmd
XMD_ROOT_WITH_VOC_ABS := ${WIN_SOFT_WORKDIR}/${XMD_MAP_DIR}/${CT_ASIC}_soft_root_with_voc.xmd
XMD_PKG_ABS := ${WIN_SOFT_WORKDIR}/${XMD_MAP_DIR}/${CT_ASIC}_soft_pkg.xmd
XMD_PKG_WITH_VOC_ABS := ${WIN_SOFT_WORKDIR}/${XMD_MAP_DIR}/${CT_ASIC}_soft_pkg_with_voc.xmd
XMD_FINAL_ABS := ${WIN_HEX_PATH}/${LODBASE_NO_PATH}/${LODBASE_NO_PATH}.xmd

.PHONY: xmd2h xmdpack

# Generate the CoolWatcher memory description headers from the XMD files,
# only if it is needed by the modification dates.
# Do this once for all the XMDs of all the modules.
xmd2h: ${UPDATE_VERSION}
	@${ECHO}
ifeq ($(VERBOSE),1)
	@${ECHO} "XMD2H"
	$(XMD_PROCESS)
else
	@${ECHO} "XMD2H"
	# Filter out the redeclaration of variables warnings.
	$(XMD_PROCESS)
endif

# All XMD header files are generated on the first run to make ${HEX}
ifeq "${AUTO_XMD2H}" "yes"
XMD2H := xmd2h
else
XMD2H := 
endif # AUTO_XMD2H != yes

# Generate the CoolXml packed file from the XMD file.
${XMD_FINAL}: ${BINARY_PATH} force
ifneq "${AUTO_XMD2H}" "no"
	@${ECHO}
	@${ECHO} "XMDPACK           `basename $(XMD_FINAL)`"
	rm -f $@
ifeq ($(VERBOSE),1)
	# Pass the SPC_IF_VERsion number to coolpkg to generate a coherent .xmd for
	# norom targets using the standard (latest) version for BCPU code instead
	# of the romed one.
	coolpkg -i ${XMD_ROOT_ABS} -o ${XMD_FINAL_ABS} ${XMD_FLAGS} -D SPC_IF_VER=${SPC_IF_VER} \
		--keepcomments ${STDOUT_NULL}
else
	# Filter out the redeclaration of variables warnings.
	coolpkg -i ${XMD_ROOT_ABS} -o ${XMD_FINAL_ABS} ${XMD_FLAGS} -D SPC_IF_VER=${SPC_IF_VER} \
		--keepcomments ${STDOUT_NULL} ${STDERR_NULL}
endif # VERBOSE
endif # AUTO_XMD2H != no

# Special target to just create an XMD pack and install it.
xmdpack: ${XMD_FINAL}
	@${ECHO}
	@${ECHO} "INSTALL           ${XMD_MAP_DIR}/`basename $(XMD_PKG_ABS)`"
	rm -f ${XMD_PKG_ABS}
	cp ${XMD_FINAL_ABS} ${XMD_PKG_ABS}
ifeq ($(VERBOSE),1)
	coolpkg -i ${XMD_ROOT_WITH_VOC_ABS} -o ${XMD_PKG_WITH_VOC_ABS} ${XMD_FLAGS} \
		--keepcomments ${STDOUT_NULL} 
else
	# Filter out the redeclaration of variables warnings.
	coolpkg -i ${XMD_ROOT_WITH_VOC_ABS} -o ${XMD_PKG_WITH_VOC_ABS} ${XMD_FLAGS} \
		--keepcomments ${STDOUT_NULL}  ${STDERR_NULL}
endif # VERBOSE

##################################################################################



##################################################################################
## Compilation rules
##################################################################################

ifeq "${WITH_FULL_PATH}" "1"
REALPATH=$(abspath $<)
else
REALPATH=$<
endif # WITH_FULL_PATH := 1

############################ Implicit rules ########################################

# We generate one dependency file on the fly for each source file, 
# either when preprocessing for asm files, or when compiling for C files.
# Those depfiles are also board depend (to deal with conditional includes)
# Empty rules are generated for all header files, to avoid issues in case one header is deleted (-MP)
${OBJ_REL_PATH}/%.o: ${LOCAL_SRC_DIR}/%.S
	@${ECHO} "CPP               $*.S"
	$(CPP) $(CPPFLAGS) $(ASCPPFLAGS)  -MT ${OBJ_REL_PATH}/$*.o -MD -MP -MF ${DEPS_REL_PATH}/$*.d -o ${OBJ_REL_PATH}/$*.asm $(REALPATH)
	@${ECHO} "AS                $*.asm"
	$(AS) $(ASFLAGS) -o ${OBJ_REL_PATH}/$*.o ${OBJ_REL_PATH}/$*.asm

${OBJ_REL_PATH}/%.o: ${LOCAL_SRC_DIR}/%.c
	@${ECHO} "CC                $*.c"
	$(CC) -MT ${OBJ_REL_PATH}/$*.o -MD -MP -MF ${DEPS_REL_PATH}/$*.d $(C_SPECIFIC_CFLAGS) $(CFLAGS) $(CT_MIPS16_CFLAGS) $(MYCFLAGS) $(CPPFLAGS)  -o ${OBJ_REL_PATH}/$*.o $(REALPATH)

${OBJ_REL_PATH}/%.o: ${LOCAL_SRC_DIR}/%.cpp
	@${ECHO} "C++               $*.cpp"
	$(C++) -MT ${OBJ_REL_PATH}/$*.o -MD -MP -MF ${DEPS_REL_PATH}/$*.d $(C++_SPECIFIC_CFLAGS) $(CFLAGS) $(CT_MIPS16_CFLAGS) $(MYCFLAGS) $(CPPFLAGS)  -o ${OBJ_REL_PATH}/$*.o $(REALPATH)


######################### library stuff #########################################
.PHONY: makedirs headergen dependencies

makedirs: ${MAKE_DIRS}
headergen: ${XCP2H} ${UPDATE_VERSION_HEADER}

# The submodules depend on:
# 	The automatically generated header files for this module
dependencies: force headergen
# The '+' token works like ${MAKE} directly in the line content.
	+@${FULL_DEPENDENCY_COMPILE}

ifneq "${strip ${FULL_SRC_OBJECTS}}" ""
# Prerequisites are order-only as we do not want to re-build the
# source objects in every make run (depending on phony targets)
${FULL_SRC_OBJECTS}: | makedirs headergen
# The local source objects that must be built
ifneq "${strip ${SRC_OBJECTS_FORCE_BUILD}}" ""
${SRC_OBJECTS_FORCE_BUILD}: force
endif # SRC_OBJECTS_FORCE_BUILD
endif # FULL_SRC_OBJECTS

# Explode sub-modules libraries
LOCAL_SUBMODULE_LIBRARY_EXPLODE_CMD = cd ${OBJ_REL_PATH} && $(AR) x ${libfile} && ${ECHO} "        (added ${notdir $(libfile)} objects)"
LOCAL_SUBMODULE_LIBRARY_EXPLODE := ${foreach libfile, $(FULL_LIBRARY_FILES) , $(LOCAL_SUBMODULE_LIBRARY_EXPLODE_CMD) &&}

# The local library is different in a module that depends on submodules, 
# since we need to depend on the submodules, and add them to the archive...
# No lib is generated for ENTRY_POINT dirs
ifneq "${IS_ENTRY_POINT}" "yes"
ifeq "${IS_TOP_LEVEL}" "yes"

# We are building a module with submodules
# This module depends on:
# 	The directories which save the built objects or files
# 	The submodules that need to be compiled (listed in LOCAL_MODULE_DEPENDS, target "dependencies")
# 	The local sources that go in the library
# 	The binary library files
# 	The local library files
# We need to explode the binary sub library into objects
# We need to copy the all objects from the submodules in the obj directory of this module
$(LOCAL_SRCLIBRARY): dependencies $(FULL_SRC_OBJECTS) ${BINARY_LIBRARY_FILES} ${LOCAL_ADD_LIBRARY_FILES} | makedirs
	@${ECHO} "PREPARING         ${notdir ${LOCAL_SRCLIBRARY}}"
	${LOCAL_SUBMODULE_LIBRARY_EXPLODE} ${ECHO} "        (All submodules objects added)"
	@${ECHO} "AR                ${notdir ${LOCAL_SRCLIBRARY}}"
# The wildcard function might generate a huge object-path list. Will it
# lead to overflow in the shell command line? Anyway, letting the shell
# expand *.o will bypass this risk.
# Only specify *.o to archive if they exist to prevent an error from
# blocking the archive generation, else create quickly an empty archive.
	if ls ${OBJ_REL_PATH}/*.o &>/dev/null;\
		then $(AR) cru 	${LOCAL_SRCLIBRARY} ${OBJ_REL_PATH}/*.o ${STDERR_NULL} || ${ECHO} "	Error in AR"; \
		else $(AR) cq 	${LOCAL_SRCLIBRARY}  					${STDERR_NULL} || ${ECHO} "	Error in AR"; \
	fi;
ifneq "$(DEPS_NOT_IN_SUBDIR)" ""
	@${ECHO} "--- DEPS_NOT_IN_SUBDIR ---"
	@${ECHO} " $(DEPS_NOT_IN_SUBDIR)"
endif # DEPS_NOT_IN_SUBDIR

else # !IS_TOP_LEVEL

# This type of module depends on:
# 	The directories which save the built objects or files
# 	The automatically generated header files
# 	The local sources that go in the library
# (The prerequisites "makedirs" and "headergen" are necessary as FULL_SRC_OBJECTS might be empty)
$(LOCAL_SRCLIBRARY): ${FULL_SRC_OBJECTS} | makedirs headergen
	@${ECHO} "AR                ${notdir ${LOCAL_SRCLIBRARY}}"
	$(AR) cru ${LOCAL_SRCLIBRARY} ${FULL_SRC_OBJECTS} ${STDERR_NULL} || ${ECHO} "	Error in AR"

endif # IS_TOP_LEVEL
endif # IS_ENTRY_POINT

############################  res_gen ############################ 
EMPTY:=
RMMACRO=-DMMI_ON_HARDWARE_P
RESCFLAGS = $(subst $(RMMACRO), $(EMPTY), $(foreach Aflag, $(MMI_EXPORT_FLAG), -D$(Aflag)))
RESCFLAGS += $(subst $(RMMACRO), $(EMPTY), $(foreach Aflag, $(GLOBAL_EXPORT_FLAG), -D$(Aflag)))

ifeq "${CYGWIN}" "1"
STRCMPEX = ${SOFT_WORKDIR}/$(MMI_CUSTOMER)/ResGenerator/strcmpex.exe
else
STRCMPEX = wine ${SOFT_WORKDIR}/$(MMI_CUSTOMER)/ResGenerator/strcmpex.exe
endif

.PHONY : resgen
resgen : 
	#ResGenerator_HW.bat
	@${ECHO} MMI Res used is ${MMI_CUSTOMER}
	@${ECHO}
	mkdir -p ${SOFT_WORKDIR}/$(MMI_CUSTOMER)/ResGenerator
ifneq "${MMI_CUSTOMER}" "application/mmi_customer"
	cp ${MMI_DEFAULT_CUSTOMER}/ResGenerator/strcmpex.exe ${SOFT_WORKDIR}/$(MMI_CUSTOMER)/ResGenerator/strcmpex.exe
endif
	chmod 777 ${SOFT_WORKDIR}/$(MMI_CUSTOMER)/ResGenerator/strcmpex.exe
	${STRCMPEX} TRUE TRUE e ~cus_opt.tmp $(foreach def,$(RESCFLAGS),$(def))
	echo "CUSTOM_OPTION = \\" > ${SOFT_WORKDIR}/$(MMI_CUSTOMER)/ResGenerator/custom_option.txt
	cat ~cus_opt.tmp >> ${SOFT_WORKDIR}/$(MMI_CUSTOMER)/ResGenerator/custom_option.txt
	@sed -i 's/-DCOMPRESS_OPTIMIZE_FLASH_SIZE/\ /g' ~cus_opt.tmp
	@sed -i 's/-D__MMI_ANALOGTV__/\ /g' ~cus_opt.tmp
	@sed -i 's/-D\([^ ]*\)/\ \/D\ \"\1\"/g' ~cus_opt.tmp
	@cp ~cus_opt.tmp ${SOFT_WORKDIR}/application/CoolSimulator/TargetOption.txt
	rm ~cus_opt.tmp	
ifneq "${MMI_CUSTOMER}" "application/mmi_customer"
# The resource generator needs make's built-in implicit rules
	export MAKEFLAGS= && ${SOFT_WORKDIR}/$(MMI_CUSTOMER)/resgen.sh ${SOFT_WORKDIR}/$(MMI_CUSTOMER) ${CT_ERES}
else	
	export MAKEFLAGS= && ${SOFT_WORKDIR}/$(MMI_CUSTOMER)/ResGenerator/resgen.sh ${SOFT_WORKDIR}/$(MMI_CUSTOMER)		
endif

############################ bin files #######################################
# Generate elf binary and map file
.PHONY: targetgen

# Allow LINK=1 or FORCE=1 to force relinking
LINK ?= FORCE
ifeq "${LINK}" "1"
FORCELINKDEP := force
endif # FORCE=1

TARGET_FILE := ${BUILD_ROOT}/targetgen

targetgen: force
	@${ECHO} "#############################################"     > $(TARGET_FILE)
	@${ECHO} "# Target component definitions"                   >> $(TARGET_FILE)
	@${ECHO} "#############################################"    >> $(TARGET_FILE)
	@${ECHO} "XCV_MODEL:=            $(XCV_MODEL)"              >> $(TARGET_FILE)
	@${ECHO} "PA_MODEL:=             $(PA_MODEL)"               >> $(TARGET_FILE)
	@${ECHO} "SW_MODEL:=             $(SW_MODEL)"               >> $(TARGET_FILE)
	@${ECHO} "FLSH_MODEL:=           $(FLSH_MODEL)"             >> $(TARGET_FILE)
	@${ECHO} "BOARD_CFG_FILE:=       $(BOARD_CFG_FILE)"         >> $(TARGET_FILE)
	@${ECHO} "#############################################"    >> $(TARGET_FILE)
	@${ECHO} "# Flash Mapping"                                  >> $(TARGET_FILE)
	@${ECHO} "#############################################"    >> $(TARGET_FILE)
	@${ECHO} "FLASH_SIZE:=           $(FLASH_SIZE)"             >> $(TARGET_FILE)
	@${ECHO} "RAM_SIZE:=             $(RAM_SIZE)"               >> $(TARGET_FILE)
	@${ECHO} "CALIB_BASE:=           $(CALIB_BASE)"             >> $(TARGET_FILE)
	@${ECHO} "FACT_SETTINGS_BASE:=   $(FACT_SETTINGS_BASE)"     >> $(TARGET_FILE)
	@${ECHO} "CODE_BASE:=            $(CODE_BASE)"              >> $(TARGET_FILE)
	@${ECHO} "USER_DATA_BASE:=       $(USER_DATA_BASE)"         >> $(TARGET_FILE)
	@${ECHO} "USER_DATA_SIZE:=       $(USER_DATA_SIZE)"         >> $(TARGET_FILE)
	@${ECHO} "ROMU_FLASH_LAYOUT:=    $(ROMU_FLASH_LAYOUT)"      >> $(TARGET_FILE)
	@${ECHO} ""                                                 >> $(TARGET_FILE)
	@${ECHO} "#############################################"    >> $(TARGET_FILE)
	@${ECHO} "# others"                                  	    >> $(TARGET_FILE)
	@${ECHO} "#############################################"    >> $(TARGET_FILE)
	@${ECHO} -e ${foreach flag, ${EXPORT_BOARD_FLAGS}, "${flag} :=\t ${${flag}}\n"} >> $(TARGET_FILE)

############################ Rule to generate the binary ##################################
# Workaround:
# Move overlaylinkers to the front of ${FULL_SRC_OBJECTS}, so as to avoid jobserver internal
# error during parallel execution when making platform_test applications.
# When make has already started a lot of jobs, e.g., ${FULL_SRC_OBJECTS}, and it wants to
# start one or two more jobs, e.g., overlaylinkers, it is error-prone if the previously
# started jobs are exiting at the same time. There are 2 kinds of symptoms:
# 1. Debug logs show "Reaping losing child ..." and make exits with
#    error "INTERNAL: Exiting with N jobserver tokens available; should be M!"
# 2. make crashes with segmentation fault
#
${BIN}: dependencies overlaylinkers clean_disassembly targetgen ${FULL_SRC_OBJECTS} ${FULL_LIBRARY_FILES} ${FORCELINKDEP} | ${BIN_PATH} ${BINARY_PATH}
	@${ECHO}
# Keeping being done after, it has the priority on the stripping.
# ie: when STRIP_ROM_OBJ2STRIP_FILE and STRIP_ROM_OBJ2KEEP_FILE are
# both defined, only STRIP_ROM_OBJ2KEEP_FILE is taken into account.
ifneq "${STRIP_ROM_OBJ2STRIP_FILE}" ""
	# Strip and get from the standard ROM a subset of the symbols which
	# are needed anyway.
	${ECHO} "GEN               stripped (remove objs) elf file ${notdir $(CHIP_ROM_ELF_FILE_STRIPPED)}"
	${OBJCOPY} --strip-symbols=${STRIP_ROM_OBJ2STRIP_FILE_PATHED} \
							   ${CHIP_ROM_ELF_FILE_FULL} \
							   ${CHIP_ROM_ELF_FILE_STRIPPED}
endif # STRIP_ROM_OBJ2STRIP_FILE
	#
ifneq "${STRIP_ROM_OBJ2KEEP_FILE}" ""
	# Strip and get from the standard ROM a subset of the symbols which
	# are needed anyway.
	${ECHO} "GEN               stripped (keep objs) elf file ${notdir $(CHIP_ROM_ELF_FILE_STRIPPED)}"
	${OBJCOPY} -S --keep-symbols=${STRIP_ROM_OBJ2KEEP_FILE_PATHED} \
								 ${CHIP_ROM_ELF_FILE_FULL} \
								 ${CHIP_ROM_ELF_FILE_STRIPPED}
endif # STRIP_ROM_OBJ2KEEP_FILE
	#
	@${ECHO} "LDGEN             ${notdir ${LD_FILE}}"
	test -f ${LD_FILE} && chmod +w ${LD_FILE} || echo ""
	# BBSRAM_ROMED_GLOBALS_END is separated from the other flags as it needs to be evaluated
	# after the stripped rom generation, here.
	${LDPP} ${LDPPFLAGS} -DBBSRAM_ROMED_GLOBALS_END=`${BBSRAM_ROMED_GLOBALS_END}` ${LD_SRC} > ${LD_FILE}
	#
	@${ECHO} "LD                ${notdir ${BIN}}"
	#
ifneq "${CLEAN_USER_SECTOR}${CLEAN_CALIB_SECTOR}${CLEAN_FACT_SECTOR}" "000"
    #
	# Generate sector cleaner lods only.
	# We do not care about the non-existed prerequisite libs here, as
	# the implicit rule %.a has suppress the issue.
	${LD} -nostdlib -o ${BIN} ${OFORMAT}    \
		${LDFLAG_USED_ROM_ELF_FILE}	\
		--script ${LD_FILE} \
		-Map ${MAP}
	#
else # CLEAN_USER_SECTOR
	#
	${LD} -nostdlib -o ${BIN} ${OFORMAT} ${FULL_SRC_OBJECTS}    \
		${LDFLAG_USED_ROM_ELF_FILE}	\
		--script ${LD_FILE} \
		$(LDFLAGS) \
		-Map ${MAP}
ifeq "${CT_COMPRESS_CODE_SECTION}" "yes"
ifneq "${LINK_STAGE_2}" "yes"
	${ECHO} "start to compress code section,please wait ... ..."
	${OBJCOPY} -j .xcpu_overlay_init_func ${BIN} ${BUILD_ROOT}/init_func.elf ${STDERR_NULL}
	${OBJCOPY} -O binary ${BUILD_ROOT}/init_func.elf ${BUILD_ROOT}/init_func.bin ${STDERR_NULL}
	${LZMA} -f -v -9 ${BUILD_ROOT}/init_func.bin
	${OBJCOPY} -j .xcpu_overlay_csdapp ${BIN} ${BUILD_ROOT}/csdapp.elf ${STDERR_NULL}
	${OBJCOPY} -O binary ${BUILD_ROOT}/csdapp.elf ${BUILD_ROOT}/csdapp.bin ${STDERR_NULL}
	${LZMA} -f -v -9 ${BUILD_ROOT}/csdapp.bin
	${OBJCOPY} -j .xcpu_overlay_wap ${BIN} ${BUILD_ROOT}/wap.elf ${STDERR_NULL}
	${OBJCOPY} -O binary ${BUILD_ROOT}/wap.elf ${BUILD_ROOT}/wap.bin ${STDERR_NULL}
	${LZMA} -f -v -9 ${BUILD_ROOT}/wap.bin
	${OBJCOPY} -j .xcpu_overlay_jmetoc ${BIN} ${BUILD_ROOT}/jmetoc.elf ${STDERR_NULL}
	${OBJCOPY} -O binary ${BUILD_ROOT}/jmetoc.elf ${BUILD_ROOT}/jmetoc.bin ${STDERR_NULL}
	${LZMA} -f -v -9 ${BUILD_ROOT}/jmetoc.bin
	${OBJCOPY} -j .xcpu_overlay_other ${BIN} ${BUILD_ROOT}/other.elf ${STDERR_NULL}
	${OBJCOPY} -O binary ${BUILD_ROOT}/other.elf ${BUILD_ROOT}/other.bin ${STDERR_NULL}
	${LZMA} -f -v -9 ${BUILD_ROOT}/other.bin
	cp -f ${BUILD_ROOT}/*.bin.lzma ${SOFT_WORKDIR}/application/coolmmi/mmi/cdflashus/src/
	cp -f ${BIN} ${BIN_FINAL}_stage1
else # LINK_STAGE_2
	${OBJCOPY} --remove-section .xcpu_overlay_init_func --remove-section .xcpu_overlay_jmetoc --remove-section .xcpu_overlay_other --remove-section .xcpu_overlay_csdapp --remove-section .xcpu_overlay_wap ${BIN} ${BIN}_tmp ${STDERR_NULL}
	$(shell sync)
	${ECHO} "offset is     $(shell cat ${MAP}|awk '$$2=="_compressed_code_start" {print $$1}')"
	${OBJCOPY} --change-section-lma .compressed=$(shell cat ${MAP}|awk '$$2=="_compressed_code_start" {print $$1}') ${BIN}_tmp ${BIN} ${STDERR_NULL}
	${ECHO} "offset is     $(shell cat ${MAP}|awk '$$2=="_compressed_code_start" {print $$1}')"
	rm -f ${BIN}_tmp
endif # LINK_STAGE_2
	touch ${SOFT_WORKDIR}/application/coolmmi/mmi/cdflashus/src/*.*
endif # CT_COMPRESS_CODE_SECTION
	#
endif # CLEAN_USER_SECTOR
	#
ifneq "${IS_DLL_ENTRY}" "yes"
	@${ECHO} "CP                Elf binary & map file"
	cp -f ${BIN} ${BIN_FINAL}
	cp -f ${MAP} ${MAP_FINAL}
endif



############################ hex files #######################################
# Generate srec and disassembly files, same logic as bin
# Always build ${HEX}
# First generate all the XMD header files if requested, second make ${BIN}
${HEX}: ${XMD2H} force
	${MAKE} ${BIN}
	@${ECHO}
	$(OBJCOPY)  ${OBJCPY_OPTS} ${SREC} ${BIN} ${HEX}

.PHONY: dump

dump:
	@${ECHO}
	@${ECHO} "DUMP              ${notdir ${DIS}}"
	$(OBJDUMP) --demangle=auto ${OBJDUMP_OPTS} ${BIN} ${USED_ROM_ELF_FILE} > ${DIS}
	@${ECHO} "CP                Disassembly file"
	cp -f ${DIS} ${DIS_FINAL}

############################# dll file ##########################################
${DLL}: ${BIN} ${BINARY_PATH}
	@${ECHO}
ifneq "${EXP}" ""
	elf2dll  -i ${BIN} -x ${EXP} -o ${DLL}
else
	elf2dll  -i ${BIN} -o ${DLL}
endif
	@${ECHO}
	@${ECHO} "CP                Elf binary & map file"
	@${ECHO}
	@${ECHO} "${notdir ${DLL}} generated"
	cp -f ${BIN} ${BIN_FINAL}
	cp -f ${MAP} ${MAP_FINAL}
	cp -f ${DLL} ${DLL_FINAL}


dlldump:
	@${ECHO}
	@${ECHO} "DUMP              ${notdir ${DIS}}"
	echo OBJDUMP=$(OBJDUMP)
	$(OBJDUMP) -D -T -s -t ${BIN} > ${DIS}
	@${ECHO} "CP                Disassembly file"
	cp -f ${DIS} ${DIS_FINAL}

############################ Lod files ##########################################
## bin is the generic .lod generating rule.
# It creates the lod file for the current board, whatever that is.
.PHONY: bin

ifeq "${CT_ROMULATOR}" "yes"
LDPPFLAGS   += -DROMULATOR=1
endif #"${CT_ROMULATOR}" "yes"

SKIP_LOD_GEN := 0
ifeq "${CT_COMPRESS_CODE_SECTION}" "yes"
ifneq "${LINK_STAGE_2}" "yes"
SKIP_LOD_GEN := 1
endif # !LINK_STAGE_2
endif # CT_COMPRESS_CODE_SECTION

ifneq "${IS_ENTRY_POINT}" "yes"
bin: force
	@echo "***************************************"
	@echo "* Need to be in an entry point module *"
	@echo "* to generate lod files ! Aborted.    *"
	@echo "***************************************"
else # IS_ENTRY_POINT

bin: all
	@${ECHO}
	#
ifeq "${CT_ASICSIMU}" "1"
		@${ECHO} "SREC		  ${LODBASE_NO_PATH}.srec for simulation"
		srecmap -c ${MAP_FILE} -m simu ${HEX} ${LODBASE} ${STDOUT_NULL}
	#
else #"${CT_ASICSIMU}" "1"
	#
ifeq "${BUILD_ROM}" "1"
	#
			@${ECHO} "SREC		  ${LODBASE_NO_PATH}.srec for rom"
			srecmap -c ${MAP_FILE} -m rom ${HEX} ${LODBASE} ${STDOUT_NULL}
			@${ECHO} "${LODBASE_NO_PATH}_introm.lod Generated"
			@${ECHO}
			@${ECHO} "---------------------------------------------------"
	#
else #"${BUILD_ROM}" "1"
ifneq "${SKIP_LOD_GEN}" "1"
	#
ifeq "${CT_RAMRUN}" "1"
				@${ECHO} "SREC		  ${LODBASE_NO_PATH}.srec for ramrun"
				srecmap -c ${MAP_FILE} -m preload ${HEX} ${LODBASE} ${STDOUT_NULL}
				@${ECHO} "CAT               ${LODBASE_NO_PATH}_ramrun.lod"
				cat ${LODBASE}*ram.lod > ${LODBASE}ramrun.lod
				rm -f ${LODBASE}*ram.lod 
				rm -f ${LODBASE}*flash.lod 
				@${ECHO}
				@${ECHO} "---------------------------------------------------"
				@${ECHO} "${LODBASE_NO_PATH}_ramrun.lod Generated"
				@${ECHO} "---------------------------------------------------"
else # CT_RAMRUN
				@${ECHO} "SREC		  ${LODBASE_NO_PATH}.srec for flash/romulator"
ifneq ($(findstring yes,${EXPORT_BOARD_INFO}),)
				srecmap -c ${MAP_FILE} -m ${FLSH_MODEL} -b ${TARGET_FILE} ${HEX} ${LODBASE} ${STDOUT_NULL}
else	
				srecmap -c ${MAP_FILE} -m ${FLSH_MODEL} ${HEX} ${LODBASE} ${STDOUT_NULL}
endif
				@${ECHO}
				@${ECHO} "---------------------------------------------------"
ifeq "${CT_ROMULATOR}" "yes"
					# In case of romulator - mv the file to a romulator file
					mv ${LODBASE}flash.lod ${LODBASE}romulator.lod
					@${ECHO} "${LODBASE_NO_PATH}_romulator.lod Generated"
else #"${CT_ROMULATOR}" "yes"
					@${ECHO} "${LODBASE_NO_PATH}_flash.lod Generated"
endif #"${CT_ROMULATOR}" "yes"
				@${ECHO} "---------------------------------------------------"
endif # CT_RAMRUN
	#
endif # !SKIP_LOD_GEN
endif #"${BUILD_ROM}" "1"
	#
ifeq "${CT_COMPRESS_CODE_SECTION}" "yes"
ifeq "${LINK_STAGE_2}" "yes"
	@${ECHO}
	@${ECHO} "---------------------------------------------------"
	@${ECHO} "The second stage of code compression is finished"
	@${ECHO} "---------------------------------------------------"
	@${ECHO}
else # !LINK_STAGE_2
	@${ECHO}
	@${ECHO} "---------------------------------------------------"
	@${ECHO} "The first stage of code compression is finished"
	@${ECHO} "Starting the second stage ..."
	@${ECHO} "---------------------------------------------------"
	@${ECHO}
	${MAKE} LINK_STAGE_2=yes
	@${ECHO}
endif # !LINK_STAGE_2
endif # CT_COMPRESS_CODE_SECTION
	#
endif # "${CT_ASICSIMU}" "1"
	#
ifneq "${SKIP_LOD_GEN}" "1"
	${MAKE} ${XCP_FINAL}
	${MAKE} ${XMD_FINAL}
endif # !SKIP_LOD_GEN
	#
endif # !IS_ENTRY_POINT

######################## target lod build #################################################
.PHONY: requestclean save_target lod

requestclean:
	@${ECHO} "REQ CLEAN         ${TARGET_TAG}"
	rm -f ${LAST_TARGET_FILE}

save_target: makedirs force
	@${ECHO} "SAVE TGT          ${TARGET_TAG}"
	echo "${TARGET_TAG}" > ${LAST_TARGET_FILE}

lod:
# Respect the order clean->save_target->resgen->bin
ifneq '${LAST_TARGET}' ''
ifneq '${TARGET_TAG}' '${LAST_TARGET}'
	${MAKE} clean
	${MAKE} save_target
endif
else
	${MAKE} save_target
endif
ifeq "${RESGEN}" "yes"
	${MAKE} resgen
endif
	${MAKE} bin
	
dll: all

#--------------------------------------------------------------------------
# Commands for 'lod' that are executed once only (for the first-level make)
# to avoid multiple execution when CT_COMPRESS_CODE_SECTION is enabled

# Target-saving
export LAST_TARGET := ${TARGET_TAG}

# Resource generation
export RESGEN := no

################### "all" target #############################################
# Needs to come after everything else has been defined...
.PHONY: all

all: $(TOP_TARGET)

##############################################################################
## Special targets
##############################################################################
################## binary lib ################################################
# copy from BUILD_ROOT to SOFT_WORKDIR if needed
# so source library apears as binary lib after make binlib
# while still using BUILD_ROOT for compilation

ifneq "${IS_ENTRY_POINT}" "yes"

.PHONY: binlib

binlib: all ${BINLIB_PATH} force
	@${ECHO} "CP                Library ${notdir ${LOCAL_SRCLIBRARY}}"
	cp -f ${LOCAL_SRCLIBRARY} ${LOCAL_BINLIBRARY}

endif # !IS_ENTRY_POINT


################## VOC BUILD #################################################
# Rules to build voc .tab from voc sources, only valid when voc tools are available
ifeq (${WITH_VOC_TOOLS},1)
ifneq (${LOCAL_VOCPROJ_NAME},)

# reset V_OBJ
V_OBJ :=

# reset V_SRC to force build when *.vocsrcs does not exist
V_SRC := force

# set V_OBJ & V_SRC if the deps file is available
-include ${LOCAL_SRC_DIR}/${LOCAL_VOCPROJ_NAME}.vocsrcs

# all mips objects depend on the voc objects (through the marker, the voc makefile being used as marker)
${FULL_SRC_OBJECTS} : ${LOCAL_SRC_DIR}/${LOCAL_VOCPROJ_NAME}.mx

# voc marker make rule (compilation is either forced or the dependency list is not empty)
${LOCAL_SRC_DIR}/${LOCAL_VOCPROJ_NAME}.mx : ${V_SRC}
	@${ECHO} "VOC               ${LOCAL_VOCPROJ_NAME}"
	voc_make -v -p ${LOCAL_VOCPROJ_NAME} -s ${LOCAL_SRC_DIR} -d ${LOCAL_SHADOW_PATH}/voccompiletmp -h ${SOFT_WORKDIR}/platform/chip/regs/${CT_ASIC}/include/voc_ram.h
	touch ${LOCAL_SRC_DIR}/${LOCAL_VOCPROJ_NAME}.mx

# voc clean rule
vocclean:
	@${ECHO} "VOCCLEAN          ${LOCAL_VOCPROJ_NAME}"
	rm -f ${V_OBJ} ${LOCAL_SRC_DIR}/*.vocsrcs

# when cleaning locally also clean voc
locclean: vocclean

endif
endif

##############################################################################
## Dependencies
##############################################################################
# Generated on-the-fly, one dependency file per source file, on each build, 
# and for use on next build


DEPFILES := $(foreach file, $(basename ${C_SRC_FILTERED} ${C++_SRC_FILTERED} ${S_SRC_FILTERED}), ${DEPS_REL_PATH}/${file}.d)
ifneq "$(DEPFILES)" ""
# allow fail on include, if depfiles haven't been generated yet (first build, or after depclean)
-include ${DEPFILES}
endif # DEPFILES

# The libraries are always generated before. This rule respond allways true.
%.a:
	echo -n ""


######################## info #################################
# Misc rules usable to debug or get information about the
# compilation environment
############################################################### 
.PHONY: showdepends dumpinfo dumpexportflags
showdepends:
	@echo -------------------------------------------------
	@echo Local built binary libs:
	@echo        ${LOCAL_BUILT_BINARY_LIBS}
	@echo Binary libs:
	@echo        $(BINARY_LIBS)
	@echo Src libs:
	@echo        $(SRC_LIBS)
	@echo Local additions:
	@echo "  libs    : ${LOCAL_LIBS}"
	@echo "  objects : ${LOCAL_OBJS}"
	@echo Objects from local source:
	@echo        ${FULL_SRC_OBJECTS}
	@echo -------------------------------------------------
	@echo Full Library files:
	@echo       $(FULL_LIBRARY_FILES)
	@echo -------------------------------------------------
	@echo Dependencies compile command: 
	@echo 		'${FULL_DEPENDENCY_COMPILE}' 
	@echo -------------------------------------------------


dumpexportflags:
	@echo -----------------------------------------------------
	@echo All export flags :
	@echo ${ALL_EXPORT_FLAG}
	@echo Global flags :
	@echo ${GLOBAL_EXPORT_FLAG}
	@echo Target flags :
	@echo ${TARGET_EXPORT_FLAG}
	@echo Chip flags :
	@echo ${CHIP_EXPORT_FLAG}
	@echo Local flags :
	@echo ${LOCAL_EXPORT_FLAG}
	@echo -----------------------------------------------------

dumpinfo:
	@echo -----------------------------------------------------
	@echo GCC version :
	$(CC) -dumpversion
	@echo SOFT_WORKDIR :
	@echo ${SOFT_WORKDIR}
	@echo BUILD_ROOT :
	@echo ${BUILD_ROOT}
	@echo CT_USER :
	@echo ${CT_USER}
	@echo CT_ASIC :
	@echo ${CT_ASIC}
	@echo CT_TARGET :
	@echo ${CT_TARGET}
	@echo CT_RELEASE :
	@echo ${CT_RELEASE}
	@echo CT_ROMULATOR :
	@echo ${CT_ROMULATOR}
	@echo CT_RAMRUN :
	@echo ${CT_RAMRUN}
	@echo "#################"
	@echo ASFLAGS :
	@echo ${ASFLAGS}
	@echo LDFLAGS :
	@echo ${LDFLAGS}
	@echo CFLAGS :
	@echo ${CFLAGS}
	@echo REDUCED_REGS_FLAG:
	@echo ${REDUCED_REGS_FLAG}
	@echo "#################"
	@echo Include Path :
	@echo ${INCLUDE_PATH}
	@echo LDPPFLAGS :
	@echo ${LDPPFLAGS}
	@echo Extra CPP flags :
	@echo ${MYCPPFLAGS}
	@echo "#################"
	@echo Top target :
	@echo ${TOP_TARGET}
	@echo Local objs :
	@echo ${FULL_SRC_OBJECTS}
	@echo Linked libs :
	@echo ${FULL_LIBRARY_EXT}
	@echo SRC_IN_ROM :
	@echo ${SRC_IN_ROM}
	@echo LINKER_MASTER :
	@echo ${LINKER_MASTER}
	@echo XCP_DEPENDS_INPUTS :
	@echo ${XCP_DEPENDS_INPUTS}
	@echo DOC_INPUT_MODULES :
	@echo ${DOC_INPUT_MODULES}
	@echo -----------------------------------------------------
	@echo C_SRC:
	@echo ${C_SRC}
	@echo C++_SRC:
	@echo ${C++_SRC}
	@echo TARGET_TAG
	@echo ${TARGET_TAG}
	@echo LAST_TARGET
	@echo ${LAST_TARGET}

##############################################################################

# To connect to CoolGdb in localhost.
COOLGDB_IP ?= 127.0.0.1
# For the BCPU:
#COOLGDB_PORT ?= 26332
# For the XCPU:
COOLGDB_PORT ?= 26331

gdb:
	@echo Launching GDB on remote CoolGDB @ ${COOLGDB_IP}:${COOLGDB_PORT}...
	$(GDB) ${BIN_FINAL} -ex "target remote ${COOLGDB_IP}:${COOLGDB_PORT}"

clewn:
	@echo Launching Clewn on remote CoolGDB @ ${COOLGDB_IP}:${COOLGDB_PORT}...
	echo "file ${BIN_FINAL}" > proj
	echo "target remote ${COOLGDB_IP}:${COOLGDB_PORT}" >> proj
	clewn -gc mips-elf-gdb -p proj 
	rm proj

##############################################################################
# Rules to build tags
##############################################################################
.PHONY: tags etags

TAGS_FILE 		?= ${SOFT_WORKDIR}/tags
TAGS_FILE_EMACS ?= ${SOFT_WORKDIR}/TAGS

# tag for vim users
tags: build_tags

build_tags: clean_tags
	ctags -f ${TAGS_FILE} -h +hp -R ${SOFT_WORKDIR}/platform/base
	ctags -f ${TAGS_FILE} -h +hp --exclude='${SOFT_WORKDIR}/platform/chip/bcpu/spal/models/*' -Ra ${SOFT_WORKDIR}/platform/chip/bcpu
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/chip/boot/$(CT_ASIC)
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/chip/boot/include
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/chip/boot/src
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/chip/defs
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/chip/hal
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/chip/patch
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/chip/regs/$(CT_ASIC)
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/chip/rom/$(CT_ASIC)
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/chip/pal
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/csw
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/edrv
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/include
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/stack
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/svc
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/vpp
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform_test
	ctags -f ${TAGS_FILE} -h +hp --exclude='${SOFT_WORKDIR}/application/coolmmi/firmware/*' -Ra ${SOFT_WORKDIR}/application
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/target
	ctags -f ${TAGS_FILE} -h +hp -Ra ${SOFT_WORKDIR}/platform/mdi


clean_tags:
	rm -f ${TAGS_FILE}

# Tag for emacs users
etags:
	rm ${TAGS_FILE_EMACS}; for i in `find -L ${SOFT_WORKDIR} -name "*.[hc]"`;  do etags -o ${TAGS_FILE_EMACS} -a $$i; done

















##############################################################################

# Rules that is useful to get compilation
# environment variables into a shell script
# (Try make get_var_CT_ASIC.)
get_var_%:
# Single quotes are necessary to avoid variable expansion in the shell
# Return the unexpanded value
	@echo '$(value $*)'

get_expanded_var_%:
# Single quotes are necessary to avoid variable expansion in the shell
# Return the expanded value
# (eval always returns an empty string)
	@echo '$(eval EXP_VAR := $$($*))$(EXP_VAR)'


# Rules that is useful to get compilation
# environment variable origins.
# (Try make get_var_origin_CT_ASIC.)
get_origin_var_%:
# Single quotes are necessary to avoid variable expansion in the shell
	@echo '$(origin $*)'











# Finally include custom rules if present
CUSTOMRULES_EXIST := $(wildcard ${SOFT_WORKDIR}/env/compilation/customrules.mk)
ifneq ($(strip $(CUSTOMRULES_EXIST)),)
include ${SOFT_WORKDIR}/env/compilation/customrules.mk
endif




