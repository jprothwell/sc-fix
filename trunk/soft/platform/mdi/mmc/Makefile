## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/mdi/mmc

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
						platform/mdi \
						platform/mdi/alg/isp \
						platform/mdi/alg/wma_decoder \
						platform/mdi/alg/amrring_codec_voc \
						platform/mdi/alg/audiojpeg_dec_voc \
						platform/mdi/alg/audiojpeg_enc_voc \
						platform/mdi/alg/preview\
						platform/mdi/alg/avi_dec \
						platform/mdi/alg/avi_enc \
						platform/mdi/alg/h263_dec \
						platform/mdi/alg/mpeg4_dec \
						platform/mdi/alg/rm_dec \
						platform/mdi/alg/ra8lbr_dec \
						platform/mdi/alg/isomedia_demux \
						platform/mdi/alg/analogtv \
						platform/csw \
						platform/edrv/lcdd \
						platform/chip/defs \
						platform/ \
						application/coolmmi/debug \
                                                application/systeminc/mmi \
						application/coolmmi/interface/config \
						application/coolmmi/mmi/AsyncEvents \
                application/coolmmi/media/common \
                application/coolmmi \
                application/coolmmi/adaptation \
                application/adaptation/custom/audio/PSI \
                application/adaptation/device \
                application/systeminc/interface/config \
                application/systeminc/interface/hwdrv \
                application/systeminc/interface/l1audio \
                application/systeminc/interface/l1interface \
                application/systeminc/interface/media \
                application/systeminc/interface/mmi \
                application/systeminc/interface/os \
                application/systeminc/interface/ps \
                application/coolmmi/mmi \
                application/coolmmi/mmi/Audio \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/DateTime \
                application/coolmmi/mmi/FileSystem \
                application/coolmmi/mmi/Framework \
                application/coolmmi/mmi/GUI \
				application/coolmmi/mmi/IdleScreen \
                application/coolmmi/mmi/MiscFramework \
                application/coolmmi/mmi/PROFILES \
                application/coolmmi/mmi/gpio \
                application/coolmmi/mmi_csdapp/EngineerMode \
                application/coolmmi/mmi_csdapp/GDI \
                application/coolmmi/mmi_csdapp/MDI \
                application/coolmmi/mmi_csdapp/mmiapi \
                application/systeminc/ps/interfaces \
                application/systeminc/ps/l4 \
		application/coolmmi/mmi/di \
                  		platform/base/std \
                  		platform/base/common \
                		platform/base/sx \
                		platform/calib \
                		platform/chip/regs/$(CT_ASIC)\
                		platform/chip/defs \
                		platform/chip/hal \
                		platform/edrv/aud \
                		platform/edrv/aud/fm \
                		platform/edrv/fmd \
                		target \
                		platform/edrv/camdold \
                		platform/edrv/atvd \
                		platform/edrv/pmd \
                		platform/edrv/cammd \
                        platform \
                        platform/svc/uctls \
                        platform/svc/umss/storage/ram      \
                        platform/svc/umss      \
                        platform/svc/umss/transport/boscsi \
                        platform/svc/utraces	\
	                    platform/csw \
	                    platform/svc/ars         \
	                    platform/vpp/speech \
                		application/coolmmi/mmi/DI \
                		application/coolmmi/custom/drv \
                		application/coolmmi/interface/hwdrv \
                		application/coolmmi/mmi_csdapp/Camera \
                		application/coolmmi/mmi_csdapp/filemgr \
                		application/coolmmi/mmi_csdapp/fmt \
                		application/coolmmi/mmi_csdapp/FileMgr \
                		application/coolmmi/mmi/Resource \
                		application/coolmmi/mmi_customer \
                		application/coolmmi/mmi_csdapp/EbookReader \
                		application/coolmmi/media/image \
                		application/coolmmi/custom/app \
                		application/coolmmi/custom/common \
                		application/coolmmi/sst \
                		application/coolmmi/interface/adaptation \
                		application/coolmmi/mmi_csdapp/Video \

# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := no


# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG += 

#CFLAGS += -isystem 

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##

include src/make.srcs


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
