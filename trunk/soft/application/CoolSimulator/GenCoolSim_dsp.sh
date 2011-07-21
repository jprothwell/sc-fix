#!/bin/bash
echo CT_ERES=${CT_ERES}
rm -f application/CoolSimulator/CoolSimulator.dsp 
rm -f application/CoolSimulator/IncOption.txt
#svn revert CoolSimulator.dsp
cp -f application/CoolSimulator/CoolSimulator.dsp.module temp.dsp
#svn revert IncOption.txt
cp -f application/CoolSimulator/IncOption.txt.module tempinc.txt

sed -i 's/\/I\ \"..\\mmi_customer/\/I\ \"..\\target_res\\XXTEMPXX\\mmi_customer/g' tempinc.txt
echo 111
sed -i 's/\/I\ \"..\\mmi_cfg/\/I\ \"..\\target_res\\XXTEMPXX\\mmi_cfg/g' tempinc.txt
sed -i "s/XXTEMPXX/${CT_ERES}/g" tempinc.txt 
echo 333
sed -i '/SOURCE=../ s/mmi_customer/target_res\\YYTEMPYY\\mmi_customer/g' temp.dsp
sed -i '/SOURCE=../ s/mmi_cfg/target_res\\YYTEMPYY\\mmi_cfg/g' temp.dsp
sed -i "s/YYTEMPYY/${CT_ERES}/g" temp.dsp
echo 666
sed  's/$/\r/' tempinc.txt > application/CoolSimulator/IncOption.txt
sed  's/$/\r/' temp.dsp > application/CoolSimulator/CoolSimulator.dsp
rm -f tempinc.txt
rm -f temp.dsp
echo coolsimulator generated
