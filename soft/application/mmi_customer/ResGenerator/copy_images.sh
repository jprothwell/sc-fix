#!/bin/bash

echo "pwd start"

workfolder="../Images/""$1"
curfolder=`pwd`

cd $workfolder






if [ ! -d "./MainLCD/Submenu2/settings/Poweronoff/" ]; then
mkdir -p "./MainLCD/Submenu2/settings/Poweronoff/"
fi
cp ./MainLCD/Submenu/Settings/Poweronoff/ST_POFF.pbm ./MainLCD/Submenu2/settings/Poweronoff/
cp ./MainLCD/Submenu/Settings/Poweronoff/ST_PON.pbm ./MainLCD/Submenu2/settings/Poweronoff/
cp ./MainLCD/Submenu/SB_BG.gif ./MainLCD/Submenu2/

echo "copy images done"

cd $curfolder
