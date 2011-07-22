## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/vpp/speech/soft

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := platform/vpp/speech/soft/efr\
                        platform/vpp/speech/soft/fr\
                        platform/vpp/speech/soft/hr 

IS_TOP_LEVEL := yes

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= yes

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
