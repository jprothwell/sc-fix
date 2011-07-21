## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/unit_test

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                    ${MMI_CONFIG} \
					application/systeminc/mmi \
					application/adaptation/communication \
					application/adaptation/custom/app \
					application/adaptation/custom/audio/PSI \
					application/adaptation/custom/common \
					application/adaptation/device \
					application/adaptation/custom/system \
					application/coolmmi/debug \
					application/coolmmi/mmi/dualsim \
					application/adaptation/device \
					application/coolmmi/mmi \
					application/systeminc/interface/adaptation \
					application/systeminc/interface/hwdrv \
					application/systeminc/interface/l1audio \
					application/systeminc/interface/media \
					application/systeminc/interface/mmi \
					application/systeminc/interface/os \
					application/systeminc/interface/ps \
					application/systeminc/interface/config \
					application/systeminc/interface/l1interface \
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
					application/coolmmi/mmi/Messages \
					application/coolmmi/mmi/MiscFramework \
					application/coolmmi/mmi/Organizer \
					application/coolmmi/mmi/PROFILES \
					application/coolmmi/mmi/PhoneBook \
					application/coolmmi/mmi/SAT \
					application/coolmmi/mmi/Setting \
					application/coolmmi/mmi/Shortcuts \
					application/coolmmi/mmi/StopWatch \
					application/coolmmi/mmi/ToDoList \
					application/coolmmi/mmi/TouchPanel \
					application/coolmmi/mmi/gpio \
					application/coolmmi/mmi/bluetooth \
					application/coolmmi/mmi_csdapp/EngineerMode \
					application/coolmmi/mmi_csdapp/FileMgr \
					application/coolmmi/mmi_csdapp/MDI \
					application/coolmmi/mmi_csdapp/fmt \
					application/coolmmi/mmi_csdapp/GDI \
					application/coolmmi/mmi_csdapp/mmiapi \
					${MMI_CUSTOMER} \
					application/coolmmi/mmi_csdapp \
					application/systeminc/ps/l4 \
					application/systeminc/ps/interfaces \
					application/systeminc/sst \
					platform \
					platform/base/sx \
					platform/edrv/pmd \
                                        platform/edrv/btd/asc3600 \
					platform/base/std \
					platform/calib \
					platform/base/common \
					platform/csw \
					platform/chip/defs \
					platform/chip/hal \
					application/coolmmi/media/common \
					application/coolmmi/media/audio \
                	application/coolmmi/mmi_csdapp/EbookReader \
                        application/coolmmi/adaptation\




# Set this to any non-null string to signal a module which 
# generates a binary (must contain a "main" entry point). 
# If left null, only a library will be generated.
IS_ENTRY_POINT := 

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
