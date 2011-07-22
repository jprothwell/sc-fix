## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

######################################
# Name of the module
######################################
LOCAL_NAME := platform/chip/bcpu

# List of submodules which contain code we need to include in the final lib
LOCAL_MODULE_DEPENDS := \
					platform/chip/bcpu/spal \
					platform/chip/bcpu/spp \
					platform/chip/bcpu/spc \

LOCAL_API_DEPENDS := platform \
					 platform/chip/hal \

# This is a top-level module
IS_TOP_LEVEL := yes

# Overwrite the normal optimisation to fit in the internal SRAM
# and to avoid inlining.
# (Inlining is the patching hell.)
export MYCFLAGS = -O2 -fno-inline-functions 

## ------------------------------------------------------------------------- ##
##	                       Source files                                      ##
## ------------------------------------------------------------------------- ##
S_SRC := 
C_SRC := bcpu_registermap.c
INPUT_FILES :=  

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
