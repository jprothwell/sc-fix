## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/base

# Compile the sub-modules, except when the "base" must be used as a library.
ifeq ($(findstring platform/base/packed,${GLOBAL_BINARY_LIBS}),)

# List of submodules which contain code we need to include in the final lib
LOCAL_MODULE_DEPENDS := \
					    platform/base/dbg \
					    platform/base/sap \
					    platform/base/std \
					    platform/base/sx \

else # GLOBAL_BINARY_LIBS

ifeq "${DUAL_SIM_SUPPORT}" "1"

ifeq "${WAPMMS_SUPPORT}" "1"
LOCAL_BINARY_LIBS    += platform/base/packed/base_dualsim_gprs
LOCAL_MODULE_DEPENDS := platform/base/packed/base_dualsim_gprs
else # !WAPMMS_SUPPORT
LOCAL_BINARY_LIBS    += platform/base/packed/base_dualsim
LOCAL_MODULE_DEPENDS := platform/base/packed/base_dualsim
endif # WAPMMS_SUPPORT

else # !DUAL_SIM_SUPPORT

ifeq "${WAPMMS_SUPPORT}" "1"
LOCAL_BINARY_LIBS    += platform/base/packed/base_singlesim_gprs
LOCAL_MODULE_DEPENDS := platform/base/packed/base_singlesim_gprs
else # !WAPMMS_SUPPORT
LOCAL_BINARY_LIBS    += platform/base/packed/base_singlesim
LOCAL_MODULE_DEPENDS := platform/base/packed/base_singlesim
endif # WAPMMS_SUPPORT

endif # DUAL_SIM_SUPPORT

endif # GLOBAL_BINARY_LIBS

# This is a top-level module
IS_TOP_LEVEL := yes

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
