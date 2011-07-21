#include <stdio.h>
#include <errno.h>

int main (int argc,char *argv[])
{
	unsigned long count=0;
	FILE *in=NULL;
	FILE *out=NULL;
	unsigned char c;
	if((in = fopen(argv[1],"r+")) == NULL){
		fprintf(stderr, "error open file %s\n", argv[1]);
		return 1;
	}
	if((out= fopen(argv[2],"w+")) == NULL){
		fprintf(stderr, "error open file %s\n", argv[2]);		
		return 1;
	}
	while(fread(&c, 1, sizeof(char), in) != 0)
	{
		fprintf(out,"0x%x,",c);
		count++;
		if (count%16 ==0)
			fprintf(out,"\n");
	}
	printf("count =%d\n",count-1);
	fclose(in);
	fclose(out);
	return 0;
}
