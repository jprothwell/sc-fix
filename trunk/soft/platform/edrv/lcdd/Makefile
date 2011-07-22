## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/edrv/lcdd

# Include the target definition here to know whic libs to link to
LOCAL_MODULE_DEPENDS :=${foreach LCD, ${LCD_MODEL}, platform/edrv/lcdd/${LCD}}

# List all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform \
                     platform/base/common \
                     platform/base/sx \
                     platform/chip \
                     platform/chip/hal \
                     platform/chip/defs\
                     platform/edrv/pmd	\
                     platform/edrv	\
                     target 

# This is a top-level module
IS_TOP_LEVEL := yes

ifeq "${RELEASE_MODULE}" "1"
# in case of LIB_RELEASE - list all the lcd modules 
LOCAL_DEPEND_LIBS := \
        platform/edrv/lcdd/au \
        platform/edrv/lcdd/optrex \
        platform/edrv/lcdd/s6d0144 \
        platform/edrv/lcdd/s6b33bf \
        platform/edrv/lcdd/st7669v \
		platform/edrv/lcdd/ili9320 \
		platform/edrv/lcdd/hx8345a \
		platform/edrv/lcdd/ili9163 \
		platform/edrv/lcdd/kc20a	\
		platform/edrv/lcdd/lgdp4532

endif #RELEASE_MODULE

#for doc to include each model's target structure
EXTRA_DOC_INPUTS := \
		platform/edrv/lcdd/au/ebc/ \
		platform/edrv/lcdd/au/lcdc/ \
		platform/edrv/lcdd/s6b33bf/ebc/ \
		platform/edrv/lcdd/s6b33bf/lcdc/ \
		platform/edrv/lcdd/ili9320/ebc/ \
		platform/edrv/lcdd/ili9320/lcdc/ \

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= yes

# Generates the CoolProfile headers automatically.
AUTO_XCP2H ?= yes

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := yes

# Compilation mode for the version string.
LOCAL_EXPORT_FLAG += LCDD_LCD_MODEL=${LCD_MODEL}
comma :=,
empty :=#
space := $(empty) $(empty)#
TGT_LCD_REGINIT := ${foreach LCD, ${LCD_MODEL},lcdd_${LCD}_RegInit$(comma)}
LCDD_REGINIT_LIST:=$(subst $(space),$(empty),$(TGT_LCD_REGINIT))
	
LOCAL_EXPORT_FLAG += LCDD_REGINIT_LIST=$(LCDD_REGINIT_LIST)
#$(info TGT_LCD_new= ${LCDD_REGINIT_LIST})

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
S_SRC :=
C_SRC := lcdd.c lcdd_map.c  
INPUT_FILES := 

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
