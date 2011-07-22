layer = arch

local_srcs = hcit_csd_uart.c papi_csd.c

CT_APP := coolmmi
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
					application/${CT_APP} \
					application/${CT_APP}/adaptation \
					application/${CT_APP}/custom/app \
					application/${CT_APP}/custom/audio/PSI \
					application/${CT_APP}/custom/common \
					application/${CT_APP}/custom/drv \
					application/${CT_APP}/custom/system \
					application/${CT_APP}/debug \
					application/${CT_APP}/drv \
					application/${CT_APP}/mmi \
					application/systeminc/interface/adaptation \
					application/systeminc/interface/hwdrv \
					application/systeminc/interface/l1audio \
					application/systeminc/interface/media \
					application/systeminc/interface/mmi \
					application/systeminc/interface/os \
					application/systeminc/interface/ps \
					application/systeminc/interface/config \
					application/${CT_APP}/interface/l1interface \
					application/${CT_APP}/main \
					application/${CT_APP}/mmi/GUI \
					application/${CT_APP}/mmi/IdleScreen \
					application/${CT_APP}/mmi/Framework \
					application/${CT_APP}/mmi/DateTime \
					application/${CT_APP}/mmi/Alarm \
					application/${CT_APP}/mmi/AsyncEvents \
					application/${CT_APP}/mmi/Audio \
					application/${CT_APP}/mmi/CPHS \
					application/${CT_APP}/mmi/CallManagement \
					application/${CT_APP}/mmi/Calls \
					application/${CT_APP}/mmi/CommonScreens \
					application/${CT_APP}/mmi/DI \
					application/${CT_APP}/mmi/DebugLevels \
					application/${CT_APP}/mmi/FileSystem \
					application/${CT_APP}/mmi/Framework \
					application/${CT_APP}/mmi/FunAndGames \
					application/${CT_APP}/mmi/Messages \
					application/${CT_APP}/mmi/MiscFramework \
					application/${CT_APP}/mmi/Organizer \
					application/${CT_APP}/mmi/PROFILES \
					application/${CT_APP}/mmi/PhoneBook \
					application/${CT_APP}/mmi/SAT \
					application/${CT_APP}/mmi/Setting \
					application/${CT_APP}/mmi/Shortcuts \
					application/${CT_APP}/mmi/StopWatch \
					application/${CT_APP}/mmi/ToDoList \
					application/${CT_APP}/mmi/TouchPanel \
					application/${CT_APP}/mmi/gpio \
					application/${CT_APP}/mmi/bt_test \
					application/${CT_APP}/mmi_csdapp/EngineerMode \
					application/${CT_APP}/mmi_csdapp/FileMgr \
					application/${CT_APP}/mmi_csdapp/MDI \
					application/${CT_APP}/mmi_csdapp/fmt \
					application/${CT_APP}/mmi_csdapp/GDI \
					application/${CT_APP}/mmi_csdapp/mmiapi \
					application/${CT_APP}/mmi_customer \
					application/${CT_APP}/mmi_csdapp \
					application/${CT_APP}/ps/l4 \
					application/${CT_APP}/ps/interfaces \
					application/${CT_APP}/sst \
			   platform \
                        platform/base \
                        platform/base/sx \
                        platform/chip \
                        platform/chip/defs \
                        platform/chip/hal \
                        platform/csw \
                        platform/edrv \
                        platform/pal \
                        platform/stack \
                        platform/svc \
                        platform/vpp \
                        target \
                     ${LOCAL_NAME}/../../bt_host \
			${LOCAL_NAME}/../../bt_host/l2cap \
			${LOCAL_NAME}/../../bt_host/manager \
			${LOCAL_NAME}/../../bt_host/hci \

 
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



