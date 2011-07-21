## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/mmi_csdapp/EngineerMode

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS :=  \
                ${MMI_CONFIG} \
                application/systeminc/mmi \
                application/adaptation/communication \
                application/adaptation/custom/VIM528 \
                application/adaptation/custom/app \
                application/adaptation/custom/audio/PSI \
                application/adaptation/custom/common \
                application/adaptation/device \
                application/adaptation/custom/lily \
                application/adaptation/custom/ps/PSI \
                application/adaptation/custom/system \
                application/coolmmi/debug \
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
                application/coolmmi/main \
                application/coolmmi/media/camera \
                application/coolmmi/media/common \
                application/coolmmi/mmi \
                application/coolmmi/mmi/Alarm \
                application/coolmmi/mmi/AsyncEvents \
                application/coolmmi/mmi/Audio \
                application/coolmmi/mmi/CallManagement \
                application/coolmmi/mmi/CommonScreens \
                application/coolmmi/mmi/DI \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/DebugLevels \
                application/coolmmi/mmi/DataAccount \
                application/coolmmi/mmi/FileSystem \
                application/coolmmi/mmi/Framework \
                application/coolmmi/mmi/GUI \
                application/coolmmi/mmi/IdleScreen \
                application/coolmmi/mmi/MainMenu \
                application/coolmmi/mmi/MiscFramework \
                application/coolmmi/mmi/PROFILES \
                application/coolmmi/mmi/PhoneBook \
                application/coolmmi/mmi/Setting \
                application/coolmmi/mmi/gpio \
				application/coolmmi/mmi/SSC \
                application/coolmmi/mmi/bluetooth \
                application/coolmmi/mmi/TouchPanel \
                application/coolmmi/mmi_csdapp/Camera \
                application/coolmmi/mmi_csdapp/AudioPlayer \
                application/coolmmi/mmi_csdapp/FileMgr \
                application/coolmmi/mmi_csdapp/fmt \
                application/coolmmi/mmi_csdapp/GDI \
                application/coolmmi/mmi_csdapp/MDI \
                application/coolmmi/mmi_csdapp/mmiapi \
                ${MMI_CUSTOMER}/CustResource \
                ${MMI_CUSTOMER}/Customize \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                application/systeminc/sst \
                application/coolmmi/mmi/Bluetooth/${MMI_BT_MODEL}/BluetoothMMI/BtComon \
                application/coolmmi/mmi/Bluetooth/${MMI_BT_MODEL}/BluetoothMMI/BtApp \
                application/coolmmi/mmi/Bluetooth/${MMI_BT_MODEL} \
                platform \
                platform/base/std \
                platform/base/sx \
                platform/chip/defs \
                platform/chip/hal \
                platform/csw \
				application/coolmmi/mmi_csdapp/EbookReader \
                ${MMI_CUSTOMER} \
				\
				\
				\
				\
	platform/edrv/aud \
	platform/edrv/aud/analog \
	platform/edrv/aud/fm \
	platform/edrv/aud/maxim \
	platform/edrv/aud/null \
	platform/edrv/fmd \
	platform/edrv/fmd/${FM_MODEL} \
	platform/edrv/lcdd/${LCD_MODEL} \
	platform/edrv/mcd \
	platform/edrv/mcd/${MCD_MODEL} \
	platform/edrv/memd \
	platform/edrv/pmd \
	platform/edrv/rfd \
	platform/edrv/tsd \
	platform/calib \
	platform/base/common \
	target \
	target/${CT_TARGET} \
	platform/mdi/





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
# Simulation specific inputs
INPUT_FILES :=  
## or include a local make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
