## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/btd/rdaCommon/rda_bt_lib/bt_host

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := 


	              
# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := ${LOCAL_NAME}/rfcomm \
						${LOCAL_NAME}/hci \
						${LOCAL_NAME}/l2cap \
						${LOCAL_NAME}/sdp \
						${LOCAL_NAME}/generic \
						${LOCAL_NAME}/manager \
						${LOCAL_NAME}/hfp \
						${LOCAL_NAME}/a2dp \
						${LOCAL_NAME}/obex \
						${LOCAL_NAME}/tcs \
						${LOCAL_NAME}/headset \
						${LOCAL_NAME}/arch \


IS_TOP_LEVEL := yes


ifneq ("$(CT_RELEASE)", "debug")
USER_DEF := DEBUG=0
else
USER_DEF := DEBUG=1
endif

LOCAL_EXPORT_FLAG += ${USER_DEF} COMBINED_HOST=0 _REENTRANT COOLSAND

## ------------------------------------- ##
##      List all your sources here       ##
## ------------------------------------- ##
C_SRC := 
##################################

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

