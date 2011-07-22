#!/bin/bash

#script executable
find ${SOFT_WORKDIR}/application/mmi_customer -name *.sh | xargs chmod +x
find ${SOFT_WORKDIR}/application/target_res -name *.sh | xargs chmod +x

echo "Cleaning all target..."
rm -rf ${SOFT_WORKDIR}/build
echo "Clean done."

LOGGINGDIR=${SOFT_WORKDIR}/env/utils/logging
echo "Create logging file"
rm -rf ${LOGGINGDIR}
mkdir -p ${LOGGINGDIR}
touch ${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
echo "Create logging file done."

###################TARGET 1#########################
TARGET=gallite_g021_foursim_3216
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=ADMIN CT_TARGET=${TARGET} CT_OPT=dbg_size CT_RELEASE=debug CT_RESGEN=yes NUMBER_OF_SIM=2 CT_PRODUCT=g021_foursim_3216_221 \
>>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt 2>&1
if [ $? != 0 ];then
	echo "Build failed for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
else 
	echo "Build success for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
fi

###################TARGET 2#########################
TARGET=gallite_g021_foursim_3216
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=ADMIN CT_TARGET=${TARGET} CT_OPT=dbg_size CT_RELEASE=debug CT_RESGEN=yes NUMBER_OF_SIM=3 CT_PRODUCT=g021_foursim_3216_331 clean lod \
>>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt 2>&1
if [ $? != 0 ];then
	echo "Build failed for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
else 
	echo "Build success for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
fi

###################TARGET 3#########################
TARGET=gallite_c006l_threesim_6432
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=ADMIN CT_TARGET=${TARGET} CT_OPT=dbg_size CT_RELEASE=debug CT_RESGEN=yes clean lod \
>>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt 2>&1
if [ $? != 0 ];then
	echo "Build failed for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
else 
	echo "Build success for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
fi

###################TARGET 4#########################
TARGET=gallite_g800_6432
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=ADMIN CT_TARGET=${TARGET} CT_OPT=dbg_size CT_RELEASE=debug CT_RESGEN=yes clean lod \
>>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt 2>&1
if [ $? != 0 ];then
	echo "Build failed for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
else 
	echo "Build success for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
fi

###################TARGET 5#########################
TARGET=gallite_hs001
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=ADMIN CT_TARGET=${TARGET} CT_OPT=dbg_size CT_RELEASE=debug CT_RESGEN=yes clean lod \
>>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt 2>&1
if [ $? != 0 ];then
	echo "Build failed for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
else 
	echo "Build success for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
fi

###################TARGET 6#########################
TARGET=gallite_q6
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=ADMIN CT_TARGET=${TARGET} CT_OPT=dbg_size CT_RELEASE=debug CT_RESGEN=yes clean lod \
>>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt 2>&1
if [ $? != 0 ];then
	echo "Build failed for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
else 
	echo "Build success for ${TARGET}">>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
fi


echo "Build all targets done!!!"

