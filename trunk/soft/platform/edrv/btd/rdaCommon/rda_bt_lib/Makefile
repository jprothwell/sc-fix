## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/btd/rdaCommon/rda_bt_lib

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform \
					 platform/base/common \
					 target  \
                     platform/base/sx \
                     platform/chip/hal \
                     platform/chip/defs\
                     platform/stack \
					 platform/edrv/btd \
                     platform/edrv/pmd \
                     platform/csw \
                     platform/edrv/mcd \
                     platform/csw/base \
                    platform/edrv/i2cgpio \
	              
# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := ${LOCAL_NAME}/bt_host \


IS_TOP_LEVEL := yes


## ------------------------------------- ##
##      List all your sources here       ##
## ------------------------------------- ##
include src/make.srcs
##################################

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

