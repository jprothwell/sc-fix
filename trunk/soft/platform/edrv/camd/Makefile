## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/edrv/camd

ifneq (${CAMERA_MODEL},)
LOCAL_MODULE_DEPENDS := platform/edrv/camd/${CAMERA_MODEL}
endif # (${CAMERA_MODEL},)

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform \
                     platform/base/common \
                     platform/base/sx \
                     platform/chip \
                     platform/chip/hal \
                     target 

# This is a top-level module
IS_TOP_LEVEL := yes

ifeq "${RELEASE_MODULE}" "1"
# in case of LIB_RELEASE - list all the camera modules 
LOCAL_DEPEND_LIBS := \
        platform/edrv/camd/ov7670 \

endif #RELEASE_MODULE

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##
AUTO_XCP2H ?= yes

LOCAL_EXPORT_FLAG += 

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# No source files in the top-level itself...
S_SRC :=
C_SRC := 
INPUT_FILES := 
## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
