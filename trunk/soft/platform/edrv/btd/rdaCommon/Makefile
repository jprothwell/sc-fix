## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/btd/rdaCommon

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := 
                     
# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := \
            platform/edrv/btd/rdaCommon/BluetoothAdapter \
            platform/edrv/btd/rdaCommon/rda_bt_lib       \

## ------------------------------------- ##
##      List all your sources here       ##
## ------------------------------------- ##
# Assembly / C code
S_SRC :=
C_SRC :=

# This is a top-level module
IS_TOP_LEVEL := yes

export WITHOUT_WERROR := 1


# Macros for different CT_RELEASE. Should be in env, put here temporarily
ifeq "${CT_RELEASE}" "release"
LOCAL_EXPORT_FLAG += 

else
ifeq "${CT_RELEASE}" "debug"
LOCAL_EXPORT_FLAG += RDA_BT_DEBUG
                                              
else
ifeq "${CT_RELEASE}" "cool_profile"

LOCAL_EXPORT_FLAG += RDA_BT_DEBUG
	                                       
endif
endif
endif

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


