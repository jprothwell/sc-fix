## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/mmi/ChatApp

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS :=  \
                ${MMI_CONFIG} \
                application/systeminc/mmi \
                application/adaptation/communication \
                application/adaptation/custom/audio/PSI \
                application/adaptation/device \
                application/systeminc/interface/config \
                application/systeminc/interface/hwdrv \
                application/systeminc/interface/l1audio \
                application/systeminc/interface/l1interface \
                application/systeminc/interface/media \
                application/systeminc/interface/mmi \
                application/systeminc/interface/os \
                application/systeminc/interface/ps \
                application/coolmmi/main \
                application/coolmmi/mmi/dualsim \
                application/coolmmi/mmi \
                application/coolmmi/mmi/AsyncEvents \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/FileSystem \
                application/coolmmi/mmi/Framework \
                application/coolmmi/mmi/FunAndGames \
                application/coolmmi/mmi/GUI \
                application/coolmmi/mmi/IdleScreen \
                application/coolmmi/mmi/MiscFramework \
                application/coolmmi/mmi/PROFILES \
                application/coolmmi/mmi/gpio \
                application/coolmmi/mmi/TouchPanel \
                application/coolmmi/mmi_csdapp/EngineerMode \
                application/coolmmi/mmi_csdapp/FileMgr \
                application/coolmmi/mmi_csdapp/GDI \
                application/coolmmi/mmi_csdapp/MDI \
                application/coolmmi/mmi_csdapp/fmt \
                application/coolmmi/mmi_csdapp/mmiapi \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                platform \
                platform/base/std \
                platform/base/sx \
				platform/chip/defs \
				platform/chip/hal \
                platform/csw \
                application/adaptation/custom/app \
                application/adaptation/custom/common \
                application/coolmmi/mmi/Alarm \
                application/coolmmi/mmi/Calendar \
                application/coolmmi/mmi/MainMenu \
                application/coolmmi/mmi/Messages \
                application/coolmmi/mmi/PhoneBook \
                application/coolmmi/mmi/Setting \
                application/systeminc/interface/adaptation \
                application/coolmmi/mmi/Audio \
                application/systeminc/sst \
                application/coolmmi/mmi/CommonScreens \
                application/coolmmi/mmi/DebugLevels \
                application/adaptation/custom/ps/PSI \
				application/coolmmi/debug \
                application/coolmmi/mmi_csdapp/EbookReader \
                ${MMI_CUSTOMER} \
				application/coolmmi/media/common



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
