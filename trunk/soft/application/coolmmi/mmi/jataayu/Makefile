## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/mmi/jataayu

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS := 
LOCAL_MODULE_DEPENDS +=         application/coolmmi/mmi/jataayu/JCommonModules
LOCAL_MODULE_DEPENDS +=        application/coolmmi/mmi/jataayu/jdd
ifndef MMI_SUPPORT_WAP
$(error "MMI_SUPPORT_WAP not defined,it should be defined in mmi_config.def")
endif
ifndef MMI_SUPPORT_MMS
$(error "MMI_SUPPORT_MMS not defined,it should be defined in mmi_config.def")
endif
ifeq "$(strip ${MMI_SUPPORT_MMS})" "NO"
  ifeq "$(strip ${MMI_SUPPORT_WAP})" "NO"
    $(error "MMI_SUPPORT_MMS or MMI_SUPPORT_WAP must be defined to YES")
  endif	
endif	

ifeq "$(strip ${MMI_SUPPORT_WAP})" "YES"
	LOCAL_MODULE_DEPENDS +=			application/coolmmi/mmi/jataayu/wap
endif
ifeq "$(strip ${MMI_SUPPORT_MMS})" "YES"
	LOCAL_MODULE_DEPENDS +=		application/coolmmi/mmi/jataayu/mms
endif	



ifeq "$(strip ${MMI_SUPPORT_JAT_OTA})" "YES" 
LOCAL_MODULE_DEPENDS +=				application/coolmmi/mmi/jataayu/prov
endif

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                ${MMI_CONFIG} \
                application/systeminc/mmi \
                application/adaptation/communication \
                application/adaptation/custom/app \
                application/adaptation/custom/audio/PSI \
                application/adaptation/custom/common \
                application/adaptation/device \
                application/coolmmi/debug \
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
                application/coolmmi/mmi/AsyncEvents \
                application/coolmmi/mmi/Audio \
                application/coolmmi/mmi/CallManagement \
                application/coolmmi/mmi/CommonScreens \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/DebugLevels \
                application/coolmmi/mmi/FileSystem \
                application/coolmmi/mmi/Framework \
                application/coolmmi/mmi/FunAndGames \
                application/coolmmi/mmi/GUI \
                application/coolmmi/mmi/IdleScreen \
                application/coolmmi/mmi/MiscFramework \
                application/coolmmi/mmi/Organizer \
                application/coolmmi/mmi/PROFILES \
                application/coolmmi/mmi/PhoneBook \
                application/coolmmi/mmi/Setting \
                application/coolmmi/mmi/ToDoList \
                application/coolmmi/mmi/gpio \
                application/coolmmi/mmi_csdapp \
                application/coolmmi/mmi_csdapp/EngineerMode \
                application/coolmmi/mmi_csdapp/FileMgr \
                application/coolmmi/mmi_csdapp/GDI \
                application/coolmmi/mmi_csdapp/MDI \
                application/coolmmi/mmi_csdapp/fmt \
                application/coolmmi/mmi_csdapp/mmiapi \
                ${MMI_CUSTOMER} \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
                application/systeminc/sst \
                platform \
                platform/base/std \
                platform/base/sx \
                                platform/chip/defs \
 platform/chip/hal \
                platform/chip/hal \
                application/coolmmi/media/audio \
                application/coolmmi/media/common \
                application/coolmmi/mmi/MainMenu \
                application/coolmmi/mmi_csdapp/EbookReader \
                application/coolmmi/mmi_csdapp/FMRadio \
				platform/csw  \
				application/coolmmi/jdd \





# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := yes


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
S_SRC :=
C_SRC := 


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

