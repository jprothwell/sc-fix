#!/usr/bin/bash


SOURCEPATH=${SOFT_WORKDIR}/toolpool/t_flash_programmer/SourceDepends/src
HEADERPATH=${SOFT_WORKDIR}/toolpool/t_flash_programmer/SourceDepends/include

echo "Start to copy src to SourceDepends"

mkdir -p ${SOURCEPATH}
mkdir -p ${HEADERPATH}

cp ${SOFT_WORKDIR}/platform/csw/base/ffs/src/*     ${SOURCEPATH}

cp ${SOFT_WORKDIR}/platform/csw/base/vds/src/*     ${SOURCEPATH}

cp ${SOFT_WORKDIR}/platform/csw/base/ml/src/*      ${SOURCEPATH}

cp ${SOFT_WORKDIR}/platform/csw/base/sul/src/*     ${SOURCEPATH}

cp ${SOFT_WORKDIR}/platform/csw/base/ver/src/*     ${SOURCEPATH}

cp ${SOFT_WORKDIR}/platform/base/std/src/*         ${SOURCEPATH}
cp ${SOFT_WORKDIR}/platform/base/std/include/*     ${HEADERPATH}

cp ${SOFT_WORKDIR}/platform/base/sx/src/*          ${SOURCEPATH}
cp ${SOFT_WORKDIR}/platform/base/sx/include/*      ${HEADERPATH}

cp ${SOFT_WORKDIR}/platform/base/dbg/src/*         ${SOURCEPATH}

cp ${SOFT_WORKDIR}/platform/chip/hal/src/*         ${SOURCEPATH}
cp ${SOFT_WORKDIR}/platform/chip/hal/include/*     ${HEADERPATH}

cp ${SOFT_WORKDIR}/platform/chip/boot/src/*        ${SOURCEPATH}
cp ${SOFT_WORKDIR}/platform/chip/boot/include/*    ${HEADERPATH}

cp ${SOFT_WORKDIR}/platform/chip/boot/gallite/src/*        ${SOURCEPATH}
cp ${SOFT_WORKDIR}/platform/chip/boot/gallite/include/*    ${HEADERPATH}

cp ${SOFT_WORKDIR}/platform/csw/driver/src/*       ${SOURCEPATH}

cp ${SOFT_WORKDIR}/platform/csw/tools/src/*        ${SOURCEPATH}

cp ${SOFT_WORKDIR}/platform/svc/fmg/src/*          ${SOURCEPATH}
cp ${SOFT_WORKDIR}/platform/svc/fmg/include/*      ${HEADERPATH}

cp ${SOFT_WORKDIR}/platform/csw/base/cos/src/cos_mem.c ${SOURCEPATH}
cp ${SOFT_WORKDIR}/platform/csw/base/tm/src/tm_time.c  ${SOURCEPATH}
cp ${SOFT_WORKDIR}/platform/csw/base/tm/src/tm_prv.h   ${SOURCEPATH}

cd ${HEADERPATH}
rm *.xcp
cd ${SOURCEPATH}
rm hal_ana_audio.c \
    hal_ana_afc.c\
    hal_ana_pa.c \
    hal_ana_rfif.c \
    hal_camera_greenstone.c

cd ../..
