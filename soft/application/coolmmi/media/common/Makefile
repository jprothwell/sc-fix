## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/media/common

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                ${MMI_CONFIG} \
                application/systeminc/mmi \
                application/coolmmi/mmi/FileSystem \
                application/systeminc/interface/config \
                application/coolmmi/mmi \
                platform \
                platform/csw \
                platform/base/sx \
                platform/base/std \
                platform/chip/defs \
                platform/chip/hal \
                application/systeminc/interface/hwdrv \
                application/adaptation/custom/app \
                application/adaptation/custom/audio/PSI \
                application/adaptation/device \
                application/systeminc/interface/l1audio \
                application/systeminc/interface/l1interface \
                application/systeminc/interface/media \
                application/systeminc/interface/mmi \
                application/systeminc/interface/os \
                application/systeminc/interface/ps \
                application/coolmmi/media/audio \
                application/coolmmi/media/others \
                application/coolmmi/mmi_csdapp/mmiapi \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                application/systeminc/interface/adaptation \
                application/coolmmi/mmi/Framework \
                application/systeminc/sst \
                application/coolmmi/debug \
                application/coolmmi/media/camera \
                application/coolmmi/media/image \
                application/coolmmi/media/video \
                application/coolmmi/media/webcam \
				application/coolmmi/mmi/GUI


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
