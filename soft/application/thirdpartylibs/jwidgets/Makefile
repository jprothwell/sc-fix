## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/thirdpartylibs/jwidgets

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                ${MMI_CONFIG} \
                application/coolmmi/debug \
                application/coolmmi/mmi \
                application/coolmmi/mmi/CommonScreens \
                application/coolmmi/mmi/DebugLevels \
                application/coolmmi/mmi/Framework \
                application/coolmmi/mmi/GUI \
                application/coolmmi/mmi/Messages \
                application/coolmmi/mmi_csdapp \
                application/systeminc/mmi \
                application/adaptation/communication \
                application/adaptation/device \
                application/systeminc/interface/config \
                application/systeminc/interface/mmi \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/FileSystem \
                application/coolmmi/mmi/IdleScreen \
                application/coolmmi/mmi/MiscFramework \
                application/coolmmi/mmi/TouchPanel \
                application/coolmmi/mmi_csdapp/GDI \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                platform \
                platform/base/sx \
                platform/base/std \
                platform/csw \
                application/adaptation/custom/audio/PSI \
                application/systeminc/interface/hwdrv \
                application/systeminc/interface/l1interface \
                application/systeminc/interface/media \
                application/systeminc/interface/os \
                application/systeminc/interface/ps \
                application/coolmmi/mmi_csdapp/EngineerMode \
                application/coolmmi/mmi_csdapp/mmiapi \
                platform/chip/defs \
                platform/chip/hal \
                application/systeminc/interface/l1audio \
				application/adaptation/custom/ps/PSI \
                application/coolmmi/media/common \
                application/coolmmi/mmi_csdapp/EbookReader \
                ${MMI_CUSTOMER} \
				application/coolmmi/mmi/dualsim \
                application/coolmmi/mmi/jataayu/jdd \
                application/coolmmi/mmi/jataayu \
                application/coolmmi/mmi/jataayu/jcore/code/common \
                application/coolmmi/mmi/jataayu/jcore/code/browser \
                application/coolmmi/mmi/jataayu/jcore/code/transport \
				application/coolmmi/mmi/jataayu/wap \
				application/coolmmi/mmi/jataayu/mms \
				application/coolmmi/mmi_csdapp/MDI \
				application/coolmmi/mmi/Audio \
				application/coolmmi/mmi/PROFILES
				



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

# We don't want to handle the warnings as errors.
export WITHOUT_WERROR := 1
MYCFLAGS         += -fno-gnu-keywords
MYCFLAGS         += -fno-operator-names
MYCFLAGS         += -fno-exceptions
MYCFLAGS         += -fno-optional-diags
MYCFLAGS         += -fno-rtti

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files
C++_SRC := ${notdir ${wildcard src/*.cpp}} #uncomment to tack all .cpp files
## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
