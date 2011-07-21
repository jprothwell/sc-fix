## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/media/image

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS :=  \
                ${MMI_CONFIG} \
                application/systeminc/mmi \
                application/systeminc/interface/hwdrv \
                application/coolmmi/media/common \
                application/systeminc/interface/config \
                platform \
                application/coolmmi/mmi/FileSystem \
                application/coolmmi/mmi \
                platform/csw \
                platform/base/sx \
                platform/base/std \
                platform/chip/defs \
                platform/chip/hal \
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
                application/adaptation/custom/VIM528 \
                application/coolmmi/mmi/DI \
		  application/coolmmi/mmi/GUI \
		  application/coolmmi/main \
		  application/coolmmi/mmi_csdapp/GDI \
		  application/coolmmi/mmi/DebugLevels \


# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAG += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG +=

#CT_MIPS16_CFLAGS := -mno-mips16
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
