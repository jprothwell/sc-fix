#**************************************************************#
# COPY THIS FILE AS "Makefile" IN THE "src" DIR OF YOUR MODULE #
# AND CUSTOMIZE IT TO FIT YOUR NEEDS.                          #
#**************************************************************#


## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application/coolmmi/main

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
                     application/adaptation/java \
                     application/coolmmi/debug \
                     application/adaptation/device \
                     application/systeminc/interface/adaptation \
                     application/systeminc/interface/config \
                     application/systeminc/interface/hwdrv \
                     application/systeminc/interface/l1audio \
                     application/systeminc/interface/l1interface \
                     application/systeminc/interface/media \
                     application/systeminc/interface/mmi \
                     application/systeminc/interface/os \
                     application/systeminc/interface/ps \
                     application/coolmmi/main \
                     application/coolmmi/mmi/dualsim \
                     application/unit_test \
                     application/coolmmi/media/common \
                     application/coolmmi/mmi \
                     application/coolmmi/mmi/Alarm \
                     application/coolmmi/mmi/AsyncEvents \
                     application/coolmmi/mmi/Audio \
                     application/coolmmi/mmi/CPHS \
                     application/coolmmi/mmi/CallManagement \
                     application/coolmmi/mmi/Calls \
                     application/coolmmi/mmi/CommonScreens \
                     application/coolmmi/mmi/DI \
                     application/coolmmi/mmi/DateTime \
                     application/coolmmi/mmi/DebugLevels \
                     application/coolmmi/mmi/FileSystem \
                     application/coolmmi/mmi/Framework \
                     application/coolmmi/mmi/FunAndGames \
                     application/coolmmi/mmi/GUI \
                     application/coolmmi/mmi/IdleScreen \
                     application/coolmmi/mmi/Messages \
                     application/coolmmi/mmi/MiscFramework \
                     application/coolmmi/mmi/Organizer \
                     application/coolmmi/mmi/PROFILES \
                     application/coolmmi/mmi/PhoneBook \
                     application/coolmmi/mmi_csdapp/EbookReader \
                     application/thirdpartylibs/gb_code \
                     application/coolmmi/mmi/SAT \
                     application/coolmmi/mmi/SP  \
                     application/coolmmi/mmi/Setting \
                     application/coolmmi/mmi/Shortcuts \
                     application/coolmmi/mmi/StopWatch \
                     application/coolmmi/mmi/ToDoList \
                     application/coolmmi/mmi/gpio \
					 application/coolmmi/mmi/usb \
                     application/coolmmi/mmi_csdapp/Camera \
                     application/coolmmi/mmi_csdapp/EngineerMode \
                     application/coolmmi/mmi_csdapp/FMRadio \
                     application/coolmmi/mmi_csdapp/FileMgr \
                     application/coolmmi/mmi_csdapp/GDI \
                     application/coolmmi/mmi_csdapp/MDI \
                     application/coolmmi/mmi_csdapp/PhotoEditor \
                     application/coolmmi/mmi_csdapp/Video \
                     application/coolmmi/mmi_csdapp/fmt \
                     application/coolmmi/mmi_csdapp/mmiapi \
                     application/coolmmi/mmi_csdapp/vObjects \
                     application/coolmmi/mmi_csdapp/analogtv \
                     application/coolmmi/mmi_csdapp/JavaInterface \
                     ${MMI_CUSTOMER} \
                     ${MMI_CUSTOMER}/CustResource \
                     application/systeminc/ps/interfaces \
                     application/systeminc/ps/l4 \
                     application/systeminc/sst \
                     platform \
                     platform/base/std \
	                 platform/stack \
                     platform/base/sx \
                     platform/chip/defs \
                     platform/chip/hal \
                     platform/csw \
                     platform/edrv/pmd \
                     platform/svc/uctls \
                     platform/svc/umss      \
                     platform/svc/umss/storage/ram      \
                     platform/svc/umss/transport/boscsi \
                     platform/svc/utraces	\
                     platform/svc/uvideos	\
	       			platform/csw \
	         		platform/edrv/pmd \
	         		platform/edrv/btd/asc3600\
		application/coolmmi/mmi/jataayu/jcore/code/browser \
		application/coolmmi/mmi/jataayu/jcore/code/transport \
		application/coolmmi/mmi/jataayu/jcore/code/ota/Provisioning/srcs \
		application/coolmmi/mmi/jataayu/jcore/code/downloadclient \
		application/coolmmi/mmi/jataayu/jcore/code/mms \
		application/coolmmi/mmi/jataayu/jcore/code/common \
		application/coolmmi/mmi/jataayu \
					application/coolmmi/mmi/jataayu/wap \
					application/coolmmi/mmi/jataayu/mms \
	         		application/coolmmi/mmi/DataAccount \
	         		application/coolmmi/mmi/jataayu/jdd \
					application/coolmmi/mmi/TouchPanel \
					application/coolmmi/mmi/bluetooth \
		   application/coolmmi/mmi/Bluetooth/${MMI_BT_MODEL}/BluetoothMMI/BtComon \
                 application/coolmmi/mmi/Bluetooth/${MMI_BT_MODEL}/BluetoothMMI/BtApp \
                 application/coolmmi/mmi/Bluetooth/${MMI_BT_MODEL} \
					platform/mdi

# Set this to any non-null string to signal a module which
# generates a binary (must contain a "main" entry point). 
# If left null, only a library will be generated.
IS_TOP_LEVEL := yes

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##

# Assembly / C code
#S_SRC := ${notdir ${wildcard *.S}}
#C_SRC := ${notdir ${wildcard *.c}}
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files

# Added the src dir of the parent module to get the auto-generated version header file.
LOCAL_ADD_INCLUDE += application/src

# Simulation specific inputs
INPUT_FILES :=  

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##

include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


