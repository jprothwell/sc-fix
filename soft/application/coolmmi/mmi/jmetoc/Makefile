## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/mmi/jmetoc

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS := 

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS :=  \
                ${MMI_CONFIG} \
                application/systeminc/mmi \
                application/coolmmi/mmi/Messages \
                application/coolmmi/mmi/dualsim \
                application/coolmmi/mmi/DataAccount \
                application/coolmmi/mmi/CommonScreens \
                application/coolmmi/mmi/SAT \
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
                platform/csw/cfw/tcpip \
                application/coolmmi/mmi \
                application/coolmmi/mmi/AsyncEvents \
                application/coolmmi/mmi/Audio \
                application/coolmmi/mmi/FunAndGames \
                application/coolmmi/mmi/TouchPanel \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/DebugLevels \
                application/coolmmi/mmi/FileSystem \
                application/coolmmi/mmi/Framework \
                application/coolmmi/mmi/cdflashus \
                application/coolmmi/mmi/GUI \
                application/coolmmi/mmi/IdleScreen \
                application/coolmmi/mmi/MainMenu \
                application/coolmmi/mmi/MiscFramework \
                application/coolmmi/mmi/PROFILES \
                application/coolmmi/mmi/PhoneBook \
                application/coolmmi/mmi/Setting \
                application/coolmmi/mmi_csdapp \
                application/coolmmi/mmi_csdapp/EngineerMode \
                application/coolmmi/mmi_csdapp/GDI \
                application/coolmmi/mmi_csdapp/mmiapi \
                application/adaptation/custom/app \
                application/systeminc/sst \
                application/systeminc/interface/adaptation \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                platform \
                platform/base/std \
                platform/base/sx \
				platform/chip/defs \
				platform/chip/hal \
                platform/csw \
				application/adaptation/custom/ps/PSI \
                application/coolmmi/debug \
                application/coolmmi/media/common \
                application/coolmmi/mmi_csdapp/EbookReader \
                ${MMI_CUSTOMER} \
                

ifeq "$(strip ${MMI_SUPPORT_LANG_SM_CHINESE})" "YES" 
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_240_320" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/240_320
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_176_220" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/176_220
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_220_176" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/220_176
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_128_160" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/128_160
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_128_128" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/128_128
endif
else
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_240_320" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/240_320_EN
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_176_220" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/176_220_EN
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_220_176" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/220_176_EN
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_128_160" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/128_160_EN
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_128_128" 
LOCAL_API_DEPENDS += application/coolmmi/mmi/jmetoc/128_128_EN
endif
endif

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
