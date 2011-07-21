#**************************************************************#
# COPY THIS FILE AS "Makefile" IN THE "src" DIR OF YOUR MODULE #
# AND CUSTOMIZE IT TO FIT YOUR NEEDS.                          #
#**************************************************************#


## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/applib/misc

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                    ${MMI_CONFIG} \
					application/adaptation/custom/app \
					application/adaptation/custom/audio/PSI \
					application/adaptation/custom/common \
					application/adaptation/custom/system \
					application/adaptation/device \
					application/systeminc/interface/adaptation \
					application/systeminc/interface/config \
					application/systeminc/interface/hwdrv \
					application/systeminc/interface/l1audio \
					application/systeminc/interface/l1interface \
					application/systeminc/interface/media \
					application/systeminc/interface/mmi \
					application/systeminc/interface/os \
					application/systeminc/interface/ps \
					application/systeminc/mmi \
					application/systeminc/ps/interfaces \
					application/systeminc/ps/l4 \
					application/systeminc/sst \
					application/coolmmi \
					application/coolmmi/adaptation \
					application/coolmmi/custom/app \
					application/coolmmi/custom/audio/PSI \
					application/coolmmi/custom/common \
					application/coolmmi/custom/drv \
					application/coolmmi/custom/system \
					application/coolmmi/debug \
					application/coolmmi/drv \
					application/coolmmi/mmi \
					application/coolmmi/interface/adaptation \
					application/coolmmi/interface/hwdrv \
					application/coolmmi/interface/l1audio \
					application/coolmmi/interface/media \
					application/coolmmi/interface/mmi \
					application/coolmmi/interface/os \
					application/coolmmi/interface/ps \
					application/coolmmi/interface/config \
					application/coolmmi/interface/l1interface \
					application/coolmmi/interface/bt \
					application/coolmmi/main \
					application/coolmmi/mmi/GUI \
					application/coolmmi/mmi/IdleScreen \
					application/coolmmi/mmi/Framework \
					application/coolmmi/mmi/DateTime \
					application/coolmmi/mmi/Alarm \
					application/coolmmi/mmi/AsyncEvents \
					application/coolmmi/mmi/Audio \
					application/coolmmi/mmi/CPHS \
					application/coolmmi/mmi/CallManagement \
					application/coolmmi/mmi/Calls \
					application/coolmmi/mmi/CommonScreens \
					application/coolmmi/mmi/DI \
					application/coolmmi/mmi/DebugLevels \
					application/coolmmi/mmi/FileSystem \
					application/coolmmi/mmi/Framework \
					application/coolmmi/mmi/FunAndGames \
					application/coolmmi/mmi/GUI \
					application/coolmmi/mmi/Messages \
					application/coolmmi/mmi/MiscFramework \
					application/coolmmi/mmi/Organizer \
					application/coolmmi/mmi/PROFILES \
					application/coolmmi/mmi/PhoneBook \
					application/coolmmi/mmi/PROFILES \
					application/coolmmi/mmi/SAT \
					application/coolmmi/mmi/Setting \
					application/coolmmi/mmi/Shortcuts \
					application/coolmmi/mmi/StopWatch \
					application/coolmmi/mmi/ToDoList \
					application/coolmmi/mmi/TouchPanel \
					application/coolmmi/mmi/gpio \
					application/coolmmi/mmi/bt_test \
					${MMI_CUSTOMER} \
					application/coolmmi/mmi_csdapp/EngineerMode \
					application/coolmmi/mmi_csdapp/FileMgr \
					application/coolmmi/mmi_csdapp/MDI \
					application/coolmmi/mmi_csdapp/fmt \
					application/coolmmi/mmi_csdapp/GDI \
					application/coolmmi/mmi_csdapp/mmiapi \
					application/coolmmi/mmi_customer \
					application/coolmmi/mmi_csdapp \
			                application/coolmmi/mmi/Bluetooth/BtComon \
        			        application/coolmmi/mmi/Bluetooth/BtApp \
					application/coolmmi/ps/l4 \
					application/coolmmi/ps/interfaces \
					application/coolmmi/sst \
					platform \
					platform/base/sx \
					platform/edrv/pmd \
                                        platform/edrv/btd/${BT_MODEL} \
					platform/base/std \
					platform/calib \
					platform/base/common \
					platform/csw \
					platform/chip/defs \
					platform/chip/hal \
					application/coolmmi/media/common \
					application/coolmmi/media/audio \
                	              application/coolmmi/mmi_csdapp/EbookReader \






# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv




# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAG += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG += NEED_BOOL

# FIXME Might be target specific
LOCAL_EXPORT_FLAG += __MCD__


## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


