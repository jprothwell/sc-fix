## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/stack/l1

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := \
	platform/phy  \
	platform/chip/defs  \
	platform \
	platform/base/common \
	platform/base/sx \
	platform/base/std \
	platform/chip/hal \
	platform/chip/pal \
	platform/stack \
	platform/stack/cmn \
	platform/stack/fsm \
	platform/stack/itf \
	platform/stack/cod \
	platform/base/sap

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS ?= 

LOCAL_EXPORT_FLAG += __XCPU__ 

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H := 

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##

LOCAL_EXPORT_FLAG +=  

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include ../stack.mk
include src/make.srcs
## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
