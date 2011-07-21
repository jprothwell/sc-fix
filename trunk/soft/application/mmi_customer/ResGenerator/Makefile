include custom_option.txt
include custom_include.txt
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

ifneq "$(shell uname | grep -c CYGWIN)" "0"
    export CYGWIN := 1
	MINGW_PREFIX =
else
	MINGW_PREFIX ?= i586-mingw32msvc
endif

ifeq "${CYGWIN}" "1"
GCC=gcc
GXX=g++
else
GCC=${MINGW_PREFIX}-gcc
GXX=${MINGW_PREFIX}-g++
endif


PLUTO_INC =  \
	 -I "../CustomerInc" \
	 -I "../include" \
	 -I "../Customize/include" \
	 -I "${SOFT_WORKDIR}/${MMI_CONFIG}/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Calls/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/GUI/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Setting/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/DebugLevels/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Framework/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/CommonScreens/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/MiscFramework/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/FunAndGames/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/NITZHandler/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/fmt/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/PhotoEditor/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/FileMgr/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/GDI/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/mmiapi/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/vObjects/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/main/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/TouchPanel/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Alarm/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/SAT/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Shortcuts/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/PROFILES/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/CallManagement/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/IdleScreen/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/CPHS/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/EngineerMode/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Organizer/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/include"  \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/ToDoList/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/StopWatch/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/PhoneBook/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/SSC/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/SoundRecorder/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/MainMenu/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/Camera/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/FMScheduleRec/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/AudioPlayer/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/FMRadio/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/Video/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/JavaInterface/include"	 \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Help/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/DateTime/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/ChatApp/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Messages/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Calendar/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Resource/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/include" \
	 -I "${SOFT_WORKDIR}/application/adaptation/device/include" \
	 -I "${SOFT_WORKDIR}/${MMI_CUSTOMER}/CustResource/include"  \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/EbookReader/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/Bluetooth/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/dualsim/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/jataayu/mms/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/jataayu/wap/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/jataayu/prov/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/jataayu/JCommonModules/JProvisioning/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/jataayu/JCommonModules/JProfileHandler/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/DataAccount/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi_csdapp/analogtv/include" \
	 -I "${SOFT_WORKDIR}/application/coolmmi/mmi/kk/include" 

	 
ifeq "${CYGWIN}" "1"
SYS_INC =  \
	 -I "/usr/include" \
	 -I "/usr/include/mingw"
else
SYS_INC =  \
	 -I "/usr/${MINGW_PREFIX}/include" 
endif

INC = $(PLUTO_INC) ${CUSTOM_INC} ${SYS_INC}

MTK_DEFS = \
	-D "__MULTI_BIN_LOAD__" \
#	-D "__EXE_DLL__"

SHARE_DEFS = \
	-D "WIN32"\
	-D "_DEBUG"\
	-D "_WINDOWS"\
	-D "_MBCS"\
	-D "_CONSOLE"\
	-D "__UCS2_ENCODING"\
	-D "MMI_ON_WIN32"\
	-D "__RESOURCE_GEN_"\
	-D "_L4_COMMON_STRUCT_H"\
	-D "DEVELOPER_BUILD_FIRST_PASS"\
	-D "__EM_MODE__"\
	-D "_AFXDLL"\
	-D "__MOD_SMSAL__"\
	-D "__SAT__"\
	-D "SHORTCUTS_APP"\
	-D "__POPULATE_ENGINE_"\
	-D "DEBUG_WINDOW"\
	-D "VM_SUPPORT" \
	-D "_WINERROR_H" \
	${CUSTOM_OPTION}

CFLAGS = -mno-cygwin

DEFINE = $(MTK_DEFS) $(SHARE_DEFS)

ENUM_LIST_H = ./enum_list.h

OBJDIR=obj
OBJECTS = $(PLUTOMMI_OBJS) $(RES_OBJS) $(MTK_OBJS)

PLUTOMMI_OBJS = \
	$(OBJDIR)/CustResDeclare.o\
	$(OBJDIR)/UCS2.o\
	$(OBJDIR)/WriteRes.o\
	$(OBJDIR)/ExportMMIFunc.o\
	$(OBJDIR)/ResBytestream.o\
	$(OBJDIR)/BMPLoader.o\
	$(OBJDIR)/bmp2ems.o\
	$(OBJDIR)/GIFLoader.o\
	$(OBJDIR)/ImageGetDimension.o\

MTK_OBJS = \
    $(OBJDIR)/resgenerator.o

RES_SOURCES = $(patsubst ../Res_MMI/%, %,$(wildcard ../Res_MMI/*.c))

RES_OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(RES_SOURCES))

READEXCEL_OBJS = $(OBJDIR)/readexcel.o\
          $(OBJDIR)/FontRes.o\

PLMNCREATE_OBJS = plmncreate.o

INC_PATH_LIST = $(patsubst "%",%,$(filter "%",$(PLUTO_INC)))

H_LIST = $(filter-out %enum_list.h,$(foreach list, $(INC_PATH_LIST), $(wildcard $(list)/*.h)))

FLTFLAGS="-s   10240"   
export   FLTFLAGS	

.SUFFIXES: .o .cpp .c

readexcel.o: readexcel.c $(H_LIST) $(ENUM_LIST_H)
	@echo Compiling $< & \
	$(GCC) $(CFLAGS) $(DEFINE) $(INC) -g $< -o -c $@
	#$gcc  $(DEFINE) $(INC) -g -w -c $< -o $@

FontRes.o: FontRes.c $(H_LIST) $(ENUM_LIST_H)
	@echo Compiling $< & \
	$(GCC) $(CFLAGS) $(DEFINE) $(INC) -g $< -o -c $@
	#gcc  $(DEFINE) $(INC) -g -w -c $< -o $@
		
plmncreate.o: plmncreate.c $(H_LIST)
	@echo Compiling $< & \
	$(GCC) $(DEFINE) $(INC) -g -w -c $< -o $@

$(OBJDIR)/%.o: %.c $(H_LIST)
	@echo Compiling $< & \
	$(GCC)  $(DEFINE) $(INC) -g -w -c $< -o $@

.cpp.o:
$(OBJDIR)/%.o: %.cpp $(H_LIST)
	@echo Compiling $< & \
	$(GXX) $(DEFINE) $(INC) -g -w -c $< -o $@
#$(GCC) $(DEFINE) $(INC) -dD -E -c $< -o $@



VPATH=../Res_MMI:.:../CustResource:${SOFT_WORKDIR}/application/coolmmi/MMI/Resource:${SOFT_WORKDIR}/application/coolmmi/MMI/Resource/src:${SOFT_WORKDIR}/application/coolmmi/MMI/MiscFramework/MiscFrameworkSrc: .:../CustResource/src 
MK_OBJDIR : 
	mkdir -p obj
resgenerator.exe: MK_OBJDIR $(OBJECTS)
	@echo Linking $< & \
	$(GXX) -g -o ./res_temp/resgenerator.exe  $(OBJECTS) -Wl,-Map,$(OBJDIR)/mtk_resgenerator.map
#       $(GCC)  -mno-cygwin -g -o mtk_resgenerator  $(OBJECTS) -Wl,-Map,mtk_resgenerator.map

readexcel.exe: $(READEXCEL_OBJS)
	@echo Linking $< & \
	$(GCC) $(CFLAGS) -g -o ./res_temp/readexcel.exe  $(READEXCEL_OBJS) -Wl,-Map,$(OBJDIR)/readexcel.map

plmncreate.exe: $(PLMNCREATE_OBJS) MK_OBJDIR $(OBJDIR)/UCS2.o 
	@echo Linking $< & \
	$(GCC) -g -o ./res_temp/plmncreate.exe  $(PLMNCREATE_OBJS) $(OBJDIR)/UCS2.o

#Debug/ResGenerator.obj Debug/ResGeneratorDoc.obj Debug/ResGeneratorView.obj Debug/MainFrm.obj Debug/DebugInit.obj Debug/Debug.obj
