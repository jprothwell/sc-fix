# ARMCC Makefile
# (c) Copyright Parthus Technologies Inc. 2000

$(info CT_RELEASE = $(CT_RELEASE))
ifneq ("$(CT_RELEASE)", "debug")
USER_DEF = -DDEBUG=0
else
USER_DEF = -DDEBUG=1
endif
PLATFORM_INCLUDE = ./platform_include.csd
# Unix Style
include $(PLATFORM_INCLUDE)

DEFINES = $(USER_DEF) -DCOMBINED_HOST=0
ARCH_FLAGS = $(DEBUG_FLAGS)
CFLAGS = -g $(ARCH_FLAGS) $(INCLUDE) -D_REENTRANT $(DEFINES) -DMTK# -apcs /interwork -DMTK
CFLAGS += -O2 -Wshadow -g -mfix-abijump -msoft-float -mreduced-regs -ffixed-t3 -ffixed-t4 -ffixed-t5 -ffixed-t6 -ffixed-t7 -ffixed-s2 -ffixed-s3 -ffixed-s4 -ffixed-s5 -ffixed-s6 -ffixed-s7  -G0 -Wall -march=xcpu -mtune=xcpu -Wa,-march=xcpu,-mtune=xcpu -EL -mexplicit-relocs -falign-functions=16 -falign-jumps=16 -fweb -frename-registers -mmemcpy -mmips-tfile -nostartfiles -nostdlib -nostdinc -nodefaultlibs -c -pipe -ffixed-fp -mips16
LDFLAGS = 

CC = mips-elf-gcc 
AR = mips-elf-ar
RANLIB = 

TOP_DIR = ..
PLATFORM_DIR = $(SOFT_WORKDIR)/platform

COOLSAND_DIR = $(TOP_DIR)/../../.
#TARGET_DIR = d:\\cygwin\\usr\\src\\bt_host\\arch\\armcc

TARGET_DIR = $(TOP_DIR)/arch/csd-sxr
#TARGET_DIR = r:\\devel\\bt_host\\arch\\armcc
#TOP_DIR = r:\\devel\\bt_host
OUT_DIR = $(TARGET_DIR)/objects
TARGET_RULES = $(TARGET_DIR)/Make.rules
INCLUDE = $(LOCAL_INC) -Iinclude/ -I$(TOP_DIR)/include -I$(TOP_DIR)/hci/include -I$(TOP_DIR)/l2cap/include -I$(TOP_DIR)/manager/include -I$(TOP_DIR)/sdp/include -I$(TOP_DIR)/api/include -I$(TOP_DIR)/rfcomm/include

layer_dirs = rfcomm hci l2cap sdp generic manager hfp a2dp obex tcs headset

export COOLSAND_DIR OUT_DIR TARGET_RULES CC CFLAGS CPPFLAGS TOP_DIR TARGET_DIR AR RANLIB PLATFORM_INCLUDE

all: subdirs library

subdirs:
	mkdir -p objects
	echo "PLATFORM_DIR = $(PLATFORM_DIR)"
	set -e ; for i in $(layer_dirs) ; do $(MAKE) -C ../../$$i ; done
	set -e ; $(MAKE) -f Makefile.csd

clean:
	find . -name "*.[oa]" -exec rm {} \;
	find . -name "*~" -exec rm {} \;

CSD_OBJ = hcit_csd_uart.o papi_csd.o

library: $(CSD_OBJ)
	$(AR) cru $(COOLSAND_DIR)/lib/rdabt_$(CT_RELEASE).a ../$(OUT_DIR)/*.o $(CSD_OBJ)
#	mkdir -p $(COOLSAND_DIR)/lib
#	cp  rdabt.a $(COOLSAND_DIR)/lib/
#	cp  $(TOP_DIR)/../user/*.h $(COOLSAND_DIR)/include/
	

