
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_240_320" 
LOCAL_LIB += application/adaptation/java/lib/libQQ2009S_ZTE_U230_Build060.a
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_176_220" 
ifneq (,$(findstring __MMI_LARGE_FONTS__,$(MMI_EXPORT_FLAG)))
LOCAL_LIB += application/adaptation/java/lib/libqq2007.a
else
LOCAL_LIB += application/adaptation/java/lib/libQQ2009S_ZTE_U230_Build060.a
endif
endif

