layer = uitron

local_srcs = papi_csd.c hcit_csd_uart.c
local_objs = $(local_srcs:.c=.o)

include $(PLATFORM_INCLUDE)

LOCAL_INCD = $(LOCAL_INC) -I$(PLATFORM_DIR)/include -I../../include -I../../rfcomm/include -I../../hci/include
 
all: $(local_objs)

papi_csd.o: papi_csd.c
	echo "PLATFORM_DIR = $(PLATFORM_DIR)"
	$(CC) -c $(CFLAGS) -I$(TARGET_DIR)/include $(LOCAL_INCD)  $< -o $@ 

hcit_csd_uart.o: hcit_csd_uart.c
	$(CC) -c $(CFLAGS) -I$(TARGET_DIR)/include $(LOCAL_INCD)  $< -o $@ 
