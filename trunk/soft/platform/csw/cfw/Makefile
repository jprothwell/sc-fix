## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/csw/cfw

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := \
					platform \
					platform/chip/hal \
					platform/base/common \
					platform/chip/defs \
					platform/stack \
					platform/csw \

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := \
						platform/csw/cfw/cm \
						platform/csw/cfw/sim \
						platform/csw/cfw/aom \
						platform/csw/cfw/shell \
						platform/csw/cfw/bal \
						platform/csw/cfw/sms \
						platform/csw/cfw/cfg \
						platform/csw/cfw/nw \
						platform/csw/cfw/aps \
						platform/csw/cfw/emod \
                                               platform/csw/cfw/tcpip\
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
		MF32_CFW_MMI_TWO_TASKS

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
