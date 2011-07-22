## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/vpp

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS ?= platform \
					 platform/base/common \
					 platform/chip/${CT_ASIC} \
                     platform/chip/hal

# Compile the sub-modules, except when the "platform/vpp" must be used as a library.
ifeq ($(findstring platform/vpp/packed,${GLOBAL_BINARY_LIBS}),)

# vpp speech
ifeq "${CHIP_WITH_VOC}" "1"
 ifeq "${CHIP_WITH_VOC_ROM}" "1"
  LOCAL_MODULE_DEPENDS += platform/vpp/speech/romz
 else
  LOCAL_MODULE_DEPENDS += platform/vpp/speech/std
 endif
else
  LOCAL_MODULE_DEPENDS += platform/vpp/speech/soft
endif

# vpp amjp
LOCAL_MODULE_DEPENDS += platform/vpp/amjp

# Vpp AMJR
ifeq "${CSW_EXTENDED_API}" "1"
LOCAL_MODULE_DEPENDS += platform/vpp/amjr
endif

else # GLOBAL_BINARY_LIBS

LOCAL_BINARY_LIBS    += platform/vpp/packed/vpp_${CT_ASIC}
LOCAL_MODULE_DEPENDS := platform/vpp/packed/vpp_${CT_ASIC}

endif # GLOBAL_BINARY_LIBS

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= yes

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := yes

C_SRC := vpp_map.c

IS_TOP_LEVEL := yes

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
