#**************************************************************#
# COPY THIS FILE AS "Makefile" IN THE "src" DIR OF YOUR MODULE #
# AND CUSTOMIZE IT TO FIT YOUR NEEDS.                          #
#**************************************************************#


## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

## ---------------------------------------- ##
## Include CompileModules.def if it exists  ##
## ---------------------------------------- ##
ifneq "$(wildcard $(SOFT_WORKDIR)/env/compilation/CompileModules.def)" ""
include ${SOFT_WORKDIR}/env/compilation/CompileModules.def
else
COMPILE_MODE := ALL
endif

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##


# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"



ifeq "$(strip ${COMPILE_MODE})" "APP" 

include ${SOFT_WORKDIR}/env/compilation/CompileAPPModules.mk

endif
ifeq "$(strip ${COMPILE_MODE})" "APLA" 

include ${SOFT_WORKDIR}/env/compilation/CompileAPPModules.mk

endif

ifeq ($(findstring application,${GLOBAL_BINARY_LIBS}),)

ifeq "$(strip ${COMPILE_MODE})" "ALL" 

LOCAL_MODULE_DEPENDS := 
LOCAL_MODULE_DEPENDS += application/coolmmi
LOCAL_MODULE_DEPENDS += ${MMI_CUSTOMER}
LOCAL_MODULE_DEPENDS += application/adaptation
LOCAL_MODULE_DEPENDS += application/thirdpartylibs
endif

else # GLOBAL_BINARY_LIBS

LOCAL_BINARY_LIBS    += application
LOCAL_MODULE_DEPENDS := application

endif # GLOBAL_BINARY_LIBS

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := 

# Set this to any non-null string to signal a module which
# generates a binary (must contain a "main" entry point). 
# If left null, only a library will be generated.
IS_TOP_LEVEL := yes

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := yes

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


