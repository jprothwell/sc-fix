## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/svc/umss

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform                           \
                     platform/svc                       \
                     platform/svc/uctls                 \
					 platform/svc/uvideos                 \
                     platform/chip/hal                  \
                     platform/chip/defs                 \
                     platform/base/sx

# list all the modules that need to be compiled prior to using this module
ifeq "$(strip ${UCTLS_SERVICE})" "umss_storage_flashmc2"
LOCAL_MODULE_DEPENDS :=                                 \
                     platform/svc/umss/storage/ram      \
                     platform/svc/umss/transport/boscsi \
		     						 platform/svc/umss/storage/mc0				\
		     						 platform/svc/umss/storage/mc1				\
		     						 platform/svc/umss/storage/flash
else
LOCAL_MODULE_DEPENDS :=                                 \
                     platform/svc/umss/storage/ram      \
                     platform/svc/umss/transport/boscsi \
		     						 platform/svc/umss/storage/mc				\
		     						 platform/svc/umss/storage/flash
endif	
					
# This is a top-level module
IS_TOP_LEVEL := yes

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##

LOCAL_EXPORT_FLAG +=

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
