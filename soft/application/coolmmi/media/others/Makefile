## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk


# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/media/others

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                ${MMI_CONFIG} \
                application/systeminc/mmi \
                application/systeminc/interface/hwdrv \
                application/systeminc/interface/l1audio \
                application/systeminc/interface/config \
                application/coolmmi/media/common \
                application/coolmmi/mmi_csdapp/EngineerMode \
                platform \
                application/coolmmi/mmi/FileSystem \
                application/coolmmi/mmi \
                platform/csw \
                platform/base/sx \
                platform/base/std \
                platform/chip/defs \
                platform/chip/hal \
                application/adaptation/device \
                application/coolmmi/mmi/Framework \
                application/coolmmi/mmi_csdapp/FileMgr \
                application/coolmmi/mmi_csdapp/GDI \
                application/coolmmi/mmi_csdapp/MDI \
                application/adaptation/communication \
                application/systeminc/interface/l1interface \
                application/systeminc/interface/media \
                application/systeminc/interface/mmi \
                application/systeminc/interface/os \
                application/systeminc/interface/ps \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/GUI \
                application/coolmmi/mmi_csdapp/fmt \
                application/coolmmi/mmi_csdapp/mmiapi \
                application/coolmmi/mmi/MiscFramework \
                application/coolmmi/debug \
                application/adaptation/custom/audio/PSI \
                application/coolmmi/media/audio \
                application/coolmmi/media/camera \
                application/coolmmi/media/image \
                application/coolmmi/mmi_csdapp \
                application/coolmmi/mmi_csdapp/AudioPlayer \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                application/coolmmi/mmi/DI \
                platform/edrv/camdold \
                platform/edrv/aud \
                platform/mdi/mmc \
                platform/mdi \


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
