## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/svc

# Compile the sub-modules, except when the "platform/svc" must be used as a library.
ifeq ($(findstring platform/svc/packed,${GLOBAL_BINARY_LIBS}),)

# List of submodules which contain code we need to include in the final lib
LOCAL_MODULE_DEPENDS := \
						platform/svc/fmg \
						platform/svc/uctls \
						platform/svc/umss \
						platform/svc/utraces \
						platform/svc/uvideos \
						platform/svc/vois \
						platform/svc/fstraces \

# TODO: Do we really need to make this inclusion depend on the CSW_EXTENDED_API flag?
ifeq "${CSW_EXTENDED_API_CAMERA}" "1"
LOCAL_MODULE_DEPENDS += platform/svc/cams
LOCAL_MODULE_DEPENDS += platform/svc/imgs
endif

ifeq "${CSW_EXTENDED_API_AUDIO_VIDEO}" "1"
LOCAL_MODULE_DEPENDS += platform/svc/avps
LOCAL_MODULE_DEPENDS += platform/svc/mps
else
LOCAL_MODULE_DEPENDS += platform/svc/aps
endif

ifeq "${CSW_EXTENDED_API_IMAGE}" "1"
LOCAL_MODULE_DEPENDS += platform/svc/imgs
endif

ifeq "${CSW_EXTENDED_API_MRS}" "1"
LOCAL_MODULE_DEPENDS += platform/svc/mrs
LOCAL_MODULE_DEPENDS += platform/svc/avrs
else
LOCAL_MODULE_DEPENDS += platform/svc/ars
endif

else # GLOBAL_BINARY_LIBS

LOCAL_BINARY_LIBS    += platform/svc/packed/svc_${CT_ASIC}
LOCAL_MODULE_DEPENDS := platform/svc/packed/svc_${CT_ASIC}

endif # GLOBAL_BINARY_LIBS

# This is a top-level module
IS_TOP_LEVEL := yes

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
