## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := ${MMI_CUSTOMER}/CustResource

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                ${MMI_CONFIG} \
                application/systeminc/mmi \
                application/adaptation/custom/ps/PSI \
                application/coolmmi/mmi \
                application/coolmmi/mmi/Alarm \
                application/coolmmi/mmi/Calendar \
                application/coolmmi/mmi/CallManagement \
                application/coolmmi/mmi/Calls \
                application/coolmmi/mmi/CommonScreens \
                application/coolmmi/mmi/DebugLevels \
                application/coolmmi/mmi/FunAndGames \
                application/coolmmi/mmi/GUI \
                application/coolmmi/mmi/Messages \
                application/coolmmi/mmi/MiscFramework \
                application/coolmmi/mmi/Organizer \
                application/coolmmi/mmi/PhoneBook \
                application/coolmmi/mmi/Setting \
                application/coolmmi/mmi/ToDoList \
                application/coolmmi/mmi_csdapp/EngineerMode \
                ${MMI_CUSTOMER} \
                application/adaptation/communication \
                application/adaptation/custom/audio/PSI \
                application/adaptation/device \
                application/systeminc/interface/config \
                application/systeminc/interface/hwdrv \
                application/systeminc/interface/l1interface \
                application/systeminc/interface/media \
                application/systeminc/interface/mmi \
                application/systeminc/interface/os \
                application/systeminc/interface/ps \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/DataAccount \
                application/coolmmi/mmi/Framework \
                application/coolmmi/mmi/IdleScreen \
                application/coolmmi/mmi/TouchPanel \
                application/coolmmi/mmi_csdapp/GDI \
                application/coolmmi/mmi_csdapp/mmiapi \
                application/coolmmi/mmi_csdapp/analogtv \
                application/coolmmi/mmi_csdapp/FMScheduleRec \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                platform \
                platform/base/sx \
                platform/base/std \
                platform/csw \
                platform/mdi \
                application/systeminc/interface/l1audio \
                application/coolmmi/mmi/FileSystem \
                platform/chip/defs \
                platform/chip/hal \
                application/coolmmi/mmi_csdapp/AudioPlayer \
                application/adaptation/custom/system \
                application/coolmmi/verno \
                application/coolmmi/mmi/Audio \
		application/coolmmi/debug \
                application/coolmmi/media/common \
                application/coolmmi/mmi_csdapp/EbookReader \
		application/coolmmi/mmi/MainMenu \
                application/coolmmi/mmi_csdapp/Camera \
                application/coolmmi/mmi_csdapp/FileMgr \
                application/coolmmi/mmi_csdapp/Video \
		${MMI_CUSTOMER}/CustResource/ringData \
		application/coolmmi/mmi_csdapp/FMRadio \
		application/coolmmi/mmi_csdapp/AudioPlayer
		

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
