#!/usr/bin/perl
use strict;
#arg1 ��������ļ���
#arg2 �������ļ���
#arg3 ���������
#arg4 ��������ַ���
#arg5 �������ִ���ʽ��0��ע��(//)��1��ע��(/**/)��2���滻�� 3��ɾ����4������
#arg6 �������ʽ���滻�������ӣ����ʾ���滻�����ӵ��ַ������ڵ��ļ�������Ϊ��

#

my ($src_file,$des_file, $pnum,@src_strs,@pmodes,@des_strs)= @_;
my $n=0;
$src_file=$ARGV[$n];
$n=$n+1;
$des_file=$ARGV[$n];
$n=$n+1;
$pnum=$ARGV[$n];
$n=$n+1;
my $tmp=$pnum;

while($tmp>0) {
$src_strs[$tmp-1]=$ARGV[$n];
$n=$n+1;
$pmodes[$tmp-1]=$ARGV[$n];
$n=$n+1;
$des_strs[$tmp-1]=$ARGV[$n];
#print $des_strs[$tmp-1];
if($des_strs[$tmp-1] ne "") {
	open(FILE,"<$des_strs[$tmp-1]");
	$des_strs[$tmp-1]="";
	while(<FILE>){
   $des_strs[$tmp-1].=$_;
    }
  }
close FILE;   
$n=$n+1;
$tmp--;
}

my $i;
open(FILE,"<$src_file");
  while(<FILE>){
   $i.=$_;
    }
close FILE;

$tmp=$pnum;
while($tmp>0) {
	if($pmodes[$tmp-1]==0) {
		$i =~s/$src_strs[$tmp-1]/\/\/$src_strs[$tmp-1]/gs;
	}
	elsif($pmodes[$tmp-1]==1) {
		$i =~s/$src_strs[$tmp-1]/\/\*$src_strs[$tmp-1]\*\//gs;
	}
	elsif($pmodes[$tmp-1]==2) {
	}
	elsif($pmodes[$tmp-1]==3) {
		$i =~s/$src_strs[$tmp-1]/ /gs;
	}
	elsif($pmodes[$tmp-1]==4) {
		#print "--$des_strs[$tmp-1]--";
		$i =~s/$src_strs[$tmp-1]/ $des_strs[$tmp-1]/g
	}
	else {
	}
	$tmp--;
}
#system("rm $des_file");
open(FILE,">$des_file");
  print FILE $i;
