## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := target

LOCAL_API_DEPENDS :=  platform                          \
                      target/${CT_TARGET}               \
                      platform/stack                    \
                      platform/vpp                      \
                      platform/base/common              \
                      platform/calib               \
                      platform/base/sx                  \
                      platform/base/std                 \
                      platform/chip/hal                 \
                      platform/chip/defs                \
                      platform/chip/bcpu                \
                      platform/chip/pal                 \
                      platform/edrv/pmd                 \
                      platform/edrv/pmd/${PM_MODEL}     \
                      platform/edrv/rfd                 \
                      platform/edrv/memd                \
                      platform/csw                      \
                      platform/csw/base                 \
                      platform/edrv/memd/${FLSH_MODEL}  \
                      platform/edrv/mcd/mcd_${MCD_IF}		\
                      platform/edrv/lcdd                \
                      platform/edrv/lcdd/${LCD_MODEL}/${LCD_IF} \
                      platform/edrv/camdold             \
                      platform/edrv/camdold/${CAMERA_MODEL} \
                      platform/mdi/                     \
                      platform/svc/uctls                \
                      platform/svc/umss                 \
                      platform/svc/utraces              \
                      platform/svc/uvideos              \
                      platform/svc/ars                  \
                      platform/svc/cams                 \
                      platform/svc/vois                 \
                      platform/edrv/aud                 \
                      platform/edrv/aud/bt              \
                      platform/edrv/aud/codec_gallite   \
                      platform/edrv/aud/fm              \
                      platform/edrv/aud/maxim           \
                      platform/edrv/aud/null            \
                      platform/edrv/aud/ti              \
                      platform/edrv/aud/tv              \

ifeq "${CSW_EXTENDED_API_CAMERA}" "1"
LOCAL_API_DEPENDS +=  platform/edrv/camd
endif

ifeq "${CSW_EXTENDED_API_AUDIO_VIDEO}" "1"
LOCAL_API_DEPENDS +=  platform/svc/avps
LOCAL_API_DEPENDS +=  platform/svc/mps
else
LOCAL_API_DEPENDS +=  platform/svc/aps
endif

#deprecated MCD_MODEL
ifneq (${MCD_MODEL},)
$(warning MCD_MODEL is deprecated, use MCD_IF instead)
endif # MCD_MODEL

#mcd as optional config:
ifneq (${MCD_IF},)
LOCAL_API_DEPENDS += platform/edrv/mcd                  \
                     platform/edrv/mcd/mcd_${MCD_IF}

LOCAL_EXPORT_FLAG += TGT_WITH_MCD TGT_WITH_MCD_IF_${MCD_IF}
endif # MCD_IF

#fm as optional config:
ifneq (${FM_MODEL},)
ifneq ($(findstring rda5888,${FM_MODEL}), )

LOCAL_API_DEPENDS += platform/edrv/fmd                  \
                     platform/edrv/fmd/rda5888

LOCAL_EXPORT_FLAG += TGT_WITH_FM TGT_WITH_FM_MODEL_${FM_MODEL}

else

LOCAL_API_DEPENDS += platform/edrv/fmd                  \
                     platform/edrv/fmd/${FM_MODEL}

LOCAL_EXPORT_FLAG += TGT_WITH_FM TGT_WITH_FM_MODEL_${FM_MODEL}

endif

endif # FM_MODEL

#gsensor as optional config:
ifneq (${GSENSOR_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/gsensor                  \
                     platform/edrv/gsensor/${GSENSOR_MODEL}

LOCAL_EXPORT_FLAG += TGT_WITH_GSENSOR TGT_WITH_GSENSOR_MODEL_${GSENSOR_MODEL}
endif # GSENSOR_MODEL

#gpioi2c as optional config:
ifneq (${I2C_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/i2cgpio                  \
                     platform/edrv/i2cgpio/${I2C_MODEL}

LOCAL_EXPORT_FLAG += TGT_WITH_GPIOI2C TGT_WITH_GPIOI2C_MODEL_${I2C_MODEL}
endif # I2C_MODEL

#bt as optional config:
ifneq (${BT_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/btd
LOCAL_API_DEPENDS += platform/edrv/btd/${BT_MODEL}
LOCAL_API_DEPENDS += platform/edrv/btd/rdaCommon/rda_bt_lib

LOCAL_EXPORT_FLAG += TGT_WITH_BT
endif # BT_MODEL

#atv as optional config:
ifneq (${ATV_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/atvd

LOCAL_EXPORT_FLAG += TGT_WITH_ATV TGT_WITH_ATV_MODEL_${ATV_MODEL}
endif # ATV_MODEL

#ts as optional config:
ifneq (${TSD_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/tsd                  \
                     platform/edrv/tsd/${TSD_MODEL}

LOCAL_EXPORT_FLAG += TGT_WITH_TS TGT_WITH_TS_MODEL_${TSD_MODEL}
endif # TSD_MODEL

LOCAL_MODULE_DEPENDS := ${TARGET_MODULE_DEPENDS}

# This is a top-level module
IS_TOP_LEVEL := yes

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= yes

# Ask target to register the extra modules.
ifeq "${TGT_REGISTER_PLATFORM_VERSIONS}" "1"
LOCAL_EXPORT_FLAG += TGT_REGISTER_PLATFORM_VERSIONS
endif #TGT_REGISTER_PLATFORM_VERSIONS

# Some USB flags.
include ${SOFT_WORKDIR}/platform/svc/uctls/uctls.def

LOCAL_EXPORT_FLAG += FLASH_BASE=${FLASH_BASE}

ifneq "${CT_PRODUCT}" ""
LOCAL_EXPORT_FLAG += CT_PRODUCT=${CT_PRODUCT}
# Always compile the file using CT_PRODUCT.
LOCAL_SRC_FORCE_BUILD := target_config.c
endif # CT_PRODUCT


# Add Module define
LOCAL_EXPORT_FLAG += CT_ASIC=${CT_ASIC}
LOCAL_EXPORT_FLAG += PA_MODEL=${PA_MODEL}
LOCAL_EXPORT_FLAG += FLSH_MODEL=${FLSH_MODEL}
LOCAL_EXPORT_FLAG += LCD_MODEL=${LCD_MODEL}
LOCAL_EXPORT_FLAG += CAMERA_MODEL=${CAMERA_MODEL}
LOCAL_EXPORT_FLAG += BT_MODEL=${BT_MODEL}
LOCAL_EXPORT_FLAG += ATV_MODEL=${ATV_MODEL}
LOCAL_EXPORT_FLAG += FM_MODEL=${FM_MODEL}
LOCAL_EXPORT_FLAG += CT_TARGET=${CT_TARGET}

## ------------------------------------------------------------------------- ##
##                         Source files                                      ##
## ------------------------------------------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

