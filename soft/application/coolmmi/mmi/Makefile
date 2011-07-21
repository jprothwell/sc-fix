## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/mmi

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS := 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/cdflashus
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/decompress
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Resource
LOCAL_MODULE_DEPENDS += application/coolmmi/mmi/jmetoc 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/FunAndGames 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Audio 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/GUI 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/IdleScreen 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/MainMenu   
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Calls  
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/DateTime    
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/DebugLevels 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/PhoneBook  
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/SAT     
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Setting  
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/CommonScreens 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/PROFILES 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/EMS       
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Messages	
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/ToDoList    
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Alarm	
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/StopWatch	
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Organizer	
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Calendar 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/gpio  
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/gsensorapp  
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/AsyncEvents 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/CPHS 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/MiscFramework 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/CallManagement 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Framework 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Shortcuts 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/FileSystem 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/SoundRecorder 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/TouchPanel 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/SP 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/DI  
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/ChatApp	
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/usb	
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/BIDI 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/dualsim 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/NITZHandler

ifeq "${TTS_MODEL}" "ejtts"
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/ejTTS
endif

ifeq "$(strip ${MMI_SUPPORT_BLUETOOTH})" "YES"
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/Bluetooth
endif

ifeq "$(strip ${MMI_SUPPORT_VIDEOCHAT})" "YES" 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/kk
endif

ifeq "$(strip ${MMI_SUPPORT_IPTV})" "YES" 
LOCAL_MODULE_DEPENDS +=	application/coolmmi/mmi/zyx
endif
				
ifeq "$(strip ${MMI_SUPPORT_WAPMMS})" "YES" 
LOCAL_MODULE_DEPENDS += application/coolmmi/mmi/jataayu

endif

ifeq "$(strip ${MMI_SUPPORT_GPRS})" "YES" 
LOCAL_MODULE_DEPENDS += application/coolmmi/mmi/DataAccount
endif

LOCAL_LIB += application/coolmmi/mmi/penpower_hwre/lib/libmmi_penpower_hwre_debug.a

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS :=  

# Set this to any non-null string to signal a module which 
# generates a binary (must contain a "main" entry point). 
# If left null, only a library will be generated.
IS_ENTRY_POINT :=

# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := yes

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
S_SRC :=
C_SRC := 


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
