#!/usr/bin/bash

RESTRICTION=0

if [ "x$1" != "x" ]; then
    RESTRICTION=1
fi

echo ====== Delete source files not open to customer ======
echo

echo "------ Enter working dir: ${SOFT_WORKDIR} ------"
echo

cd ${SOFT_WORKDIR}
if [ "x$?" != "x0" ]; then
    echo
    echo "!!!!!! Failed to enter working dir !!!!!!"
    exit 1
fi

echo "------ Start to delete source files ------"
echo

rm -fr ./platform/stack/*/src/
rm -fr ./platform/stack/src/

if [ "x${RESTRICTION}" = "x1" ]; then

rm -fr ./platform/base/*/src/
rm -fr ./platform/base/src/
rm -fr ./platform/chip/*/*/models/
rm -fr ./platform/chip/*/*/src/
rm -fr ./platform/chip/*/src/
rm -fr ./platform/chip/src/
rm -fr ./platform/csw/*/*/src/
rm -fr ./platform/csw/*/src/
rm -fr ./platform/csw/src/
rm -fr ./platform/edrv/btd/asc3600/src/
rm -fr ./platform/edrv/btd/rdaCommon/*/src/
rm -fr ./platform/edrv/btd/rdaCommon/rda_bt_lib/bt_host/
rm -fr ./platform/mdi/alg/
rm -fr ./platform/mdi/mmc/
rm -fr ./platform/mdi/src/
rm -fr ./platform/svc/*/*/*/src/
rm -fr ./platform/svc/*/*/src/
rm -fr ./platform/svc/*/src/
rm -fr ./platform/svc/src/
rm -fr ./platform/vpp/*/*/*/src/
rm -fr ./platform/vpp/*/*/src/
rm -fr ./platform/vpp/*/src/
rm -fr ./platform/vpp/src/

rm -fr ./platform_test/

rm -fr ./toolpool/t_flash_programmer/SourceDepends/src
rm -fr ./toolpool/t_flash_programmer/SourceDepends/include
rm -fr ./toolpool/flash_programmer/SourceDepends/src
rm -fr ./toolpool/flash_programmer/SourceDepends/include

fi

echo ====== Done ======
