## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/svc/uctls

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform                      \
                     platform/svc                  \
                     platform/svc/umss             \
					 platform/svc/uvideos             \
                     platform/svc/utraces          \
                     platform/chip/hal             \
                     platform/base/sx              \
                     platform/chip/defs            \
                     platform/stack                \
                     target

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS :=

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
