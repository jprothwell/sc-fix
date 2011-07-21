## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/thirdpartylibs/j2me/javacall

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS := 



# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS :=  \
		${MMI_CONFIG} \
                application/systeminc/mmi \
                application/adaptation/communication \
                application/adaptation/custom/app \
                application/adaptation/custom/audio/PSI \
                application/adaptation/custom/common \
                application/adaptation/device \
                application/adaptation/custom/ps/PSI \
                application/coolmmi/debug \
                application/thirdpartylibs/gb_code \
                application/systeminc/interface/adaptation \
                application/systeminc/interface/config \
                application/systeminc/interface/hwdrv \
                application/systeminc/interface/l1audio \
                application/systeminc/interface/l1interface \
                application/systeminc/interface/media \
                application/systeminc/interface/mmi \
                application/systeminc/interface/os \
                application/systeminc/interface/ps \
                application/coolmmi/mmi \
                application/coolmmi/main \
                application/coolmmi/mmi/Alarm \
                application/coolmmi/mmi/AsyncEvents \
                application/coolmmi/mmi/Audio \
                application/coolmmi/mmi/CPHS \
                application/coolmmi/mmi/CallManagement \
                application/coolmmi/mmi/Calls \
                application/coolmmi/mmi/ChatApp \
                application/coolmmi/mmi/CommonScreens \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/DataAccount \
                application/coolmmi/mmi/DebugLevels \
                application/coolmmi/mmi/FileSystem \
                application/coolmmi/mmi/Framework \
                application/coolmmi/mmi/FunAndGames \
                application/coolmmi/mmi/GUI \
                application/coolmmi/mmi/IdleScreen \
                application/coolmmi/mmi/MainMenu \
                application/coolmmi/mmi/MiscFramework \
                application/coolmmi/mmi/PROFILES \
                application/coolmmi/mmi/PhoneBook \
                application/coolmmi/mmi/SSC \
                application/coolmmi/mmi/Setting \
                application/coolmmi/mmi/gpio \
                application/coolmmi/mmi/TouchPanel \
                application/coolmmi/mmi_csdapp \
                application/coolmmi/mmi_csdapp/AudioPlayer \
                application/coolmmi/mmi_csdapp/EngineerMode \
                application/coolmmi/mmi_csdapp/FileMgr \
                application/coolmmi/mmi_csdapp/GDI \
                application/coolmmi/mmi_csdapp/MDI \
                application/coolmmi/mmi_csdapp/fmt \
                application/coolmmi/mmi_csdapp/mmiapi \
                application/coolmmi/mmi_csdapp/JavaInterface \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                application/systeminc/sst \
                platform \
                platform/base/std \
                platform/base/sx \
		  platform/chip/defs \
		  platform/chip/hal \
                platform/csw \
                platform/csw/cfw/tcpip \
	 	  application/coolmmi/media/common \
                application/coolmmi/mmi_csdapp/EbookReader \
                ${MMI_CUSTOMER} \
                application/coolmmi/mmi/dualsim \

#MIDP_API_DEPENDS := $(shell cat midp-include)

EXT_API_DEPENDS	:= \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/cpu/c \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/os/javacall \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/os/utilities \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/compiler \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/debugger \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/handles \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/interpreter \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/isolate \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/memory \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/natives \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/ROM \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/reflection \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/runtime \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/verifier \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/utilities \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/memoryprofiler \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/vm/share/float \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/anilib/share \
	application/thirdpartylibs/j2me/phoneme_feature/cldc/src/anilib/javacall \
	application/thirdpartylibs/j2me/phoneme_feature/output/cldc/javacall_csd_gcc/target/generated \
	application/thirdpartylibs/j2me/phoneme_feature/output/cldc/javacall_csd_gcc/target/generated/incls \
	application/thirdpartylibs/j2me/phoneme_feature/midp/src/core/memory/include
#EXT_API_DEPENDS += $(subst -I/cygdrive/E/project/JVMProject/soft/, ,MIDP_API_DEPENDS)

# Set this to any non-null string to signal a module which 
# generates a binary (must contain a "main" entry point). 
# If left null, only a library will be generated.
IS_ENTRY_POINT :=

# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL :=

# For a toplevel module, define which tests to include in delivery
# Skip the 'tests' in the names. Beware that everything from the 
# given tests directories will be included...
TOPLEVEL_DELIVER_TESTS := 

# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG += ENABLE_ON_DEVICE_DEBUG=0 LINUX ENABLE_NATIVE_AMS=0 NOT_USE_NATIVE_DIGITS=1 MINUS_ALWAYS_INFRONT=1 ENABLE_PCSL

# We don't want to handle the warnings as errors.
export WITHOUT_WERROR := 1


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
