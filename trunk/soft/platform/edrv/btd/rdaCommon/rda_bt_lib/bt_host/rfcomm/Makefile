layer = rfcomm

local_srcs = rfcomm_core.c rfcomm_ctrl_chan.c rfcomm_dlc_state.c rfcomm_frames.c rfcomm_l2cap.c rfcomm_ue_api.c rfcomm_ctrl_frames.c rfcomm_pe.c port_ent_api.c rfcomm_credit.c rfcomm_entity_core.c stack_ent.c stack_ent_api.c rfcomm_l2auto_state.c rfcomm_bufalloc.c stack_ent_pump.c
#
#LOCAL_INC = -I$(PWD)/include -I../l2cap/include
#
#include $(TARGET_RULES)
#
## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/edrv/btd/rdaCommon/rda_bt_lib/bt_host/${layer}

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS :=  \
			 platform \
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
			${LOCAL_NAME}/../../bt_host \
			${LOCAL_NAME}/../../bt_host/l2cap \
			${LOCAL_NAME}/../../bt_host/a2dp \
			${LOCAL_NAME}/../../bt_host/sdp \
			${LOCAL_NAME}/../../bt_host/arch \
			${LOCAL_NAME}/../../bt_host/hci \
			${LOCAL_NAME}/../../bt_host/manager \
			${LOCAL_NAME}/../../bt_host/rfcomm \
			
 
# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG += 

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
#S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
#C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files

C_SRC := ${local_srcs}

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

