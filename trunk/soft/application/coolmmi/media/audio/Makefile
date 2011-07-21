## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/media/audio

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                ${MMI_CONFIG} \
                application/coolmmi/debug \
                application/coolmmi/mmi/FileSystem \
                platform/csw \
                application/systeminc/mmi \
                platform \
                platform/chip/defs \
                platform/chip/hal \
                application/coolmmi/media/common \
                application/systeminc/interface/config \
                application/coolmmi/mmi \
                platform/base/sx \
                platform/base/std \
                application/adaptation/custom/app \
                application/adaptation/custom/audio/PSI \
                application/adaptation/device \
                application/systeminc/interface/hwdrv \
                application/systeminc/interface/l1audio \
                application/systeminc/interface/l1interface \
                application/systeminc/interface/media \
                application/systeminc/interface/mmi \
                application/systeminc/interface/os \
                application/systeminc/interface/ps \
                application/coolmmi/media/others \
                application/coolmmi/mmi_csdapp/mmiapi \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                application/systeminc/interface/adaptation \
                application/coolmmi/mmi/Framework \
                application/systeminc/sst \
                application/coolmmi/media/camera \
                application/coolmmi/media/video \
                application/coolmmi/media/mp3dec \
				application/coolmmi/mmi/GUI \
				application/coolmmi/mmi/MiscFramework \
                application/coolmmi/mmi_csdapp/AudioPlayer \
                application/coolmmi/mmi_csdapp/FMRadio \
				application/coolmmi/mmi_csdapp/FileMgr \
				application/adaptation/communication \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi_csdapp/fmt \
                application/coolmmi/mmi_csdapp/MDI \
                application/coolmmi/mmi/Bluetooth/${MMI_BT_MODEL}/BluetoothMMI/BtComon \
                application/coolmmi/mmi/Bluetooth/${MMI_BT_MODEL}/BluetoothMMI/BtApp \
                application/coolmmi/mmi/Bluetooth/${MMI_BT_MODEL}\
                application/coolmmi/mmi/Setting \
                application/coolmmi/mmi/debuglevels \
                application/thirdpartylibs/magic_sound \
                       

    LOCAL_API_DEPENDS +=  platform/mdi



# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := 


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
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


