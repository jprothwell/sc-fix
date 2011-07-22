#############################################################################
# Depending on the team, selects which module must be used as lib.
#############################################################################

# By default, all modules are used as a lib.
TEAM_BINARY_LIBS := platform/base/packed
TEAM_BINARY_LIBS += platform/chip/packed
TEAM_BINARY_LIBS += platform/stack/packed
TEAM_BINARY_LIBS += platform/svc/packed
TEAM_BINARY_LIBS += platform/vpp/packed
TEAM_BINARY_LIBS += platform/csw/packed
TEAM_BINARY_LIBS += platform/mdi/packed
ifneq ($(findstring rda,${BT_MODEL}),)
TEAM_BINARY_LIBS += platform/edrv/btd/rdaCommon
endif # (${BT_MODEL},)
# Check if CT_USER is set.
ifeq "${CT_USER}" ""
$(error "!!!! CT_USER not defined!!!!")
endif

ifeq "${CT_USER}" "ADMIN"
 TEAM_BINARY_LIBS := 
else
 ifeq "${CT_USER}" "MMI"
 else
  ifeq "${CT_USER}" "FAE"
   # Only stack is used as a lib
   TEAM_BINARY_LIBS := platform/stack/packed
  else
   ifeq "${CT_USER}" "CHIP"
    TEAM_BINARY_LIBS := platform/base/packed
    TEAM_BINARY_LIBS += platform/stack/packed
    TEAM_BINARY_LIBS += platform/svc/packed
    TEAM_BINARY_LIBS += platform/vpp/packed
    TEAM_BINARY_LIBS += platform/csw/packed
    TEAM_BINARY_LIBS += platform/mdi/packed
    ifneq ($(findstring rda,${BT_MODEL}),)
    TEAM_BINARY_LIBS += platform/edrv/btd/rdaCommon
    endif # (${BT_MODEL},)
   else
    $(error "!!!! CT_USER not supported! Please use ADMIN, CHIP, MMI or FAE!!!!")
   endif
  endif
 endif
endif

