/**
**	you should make sure:
**	1)file format:bmp
**	2)the original picture is larger than secret picture
**	3)No compressed	
**/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
struct stat statbuff;
void stegPic(){
	FILE *fp;
	char picpath[]="D:\\test.bmp";	//original picture path
	char datapath[]="D:\\data.bmp";	//secret picture path
	long size1,size2;
	unsigned char* buffer1=NULL;
	unsigned char* buffer2=NULL;
	long long int i,j;
	unsigned long int length1=0;
	unsigned long int length2=0;
	unsigned long int height1=0;
	unsigned long int height2=0;
	
	/******create buffer1******/
	if(!(fp=fopen(picpath,"rb"))){
		exit(0);
	}
	stat(picpath, &statbuff);	
	size1=(statbuff.st_size);
	buffer1=(unsigned char*)malloc(size1);
	fread(buffer1,size1,1,fp); 

	//get the length
	rewind(fp);
	unsigned char buffer[4];
	fseek(fp,18L,0);	//the picture length is in 18L
	fread(buffer,4L,1,fp);
	for(i=3;i>=0;i--){
		length1=length1*256+buffer[i];	//hex to dec
	}
	printf("length1:%d\n",length1);
	
	//get the height
	rewind(fp);
	fseek(fp,22L,0);	//the picture height is in 22L
	fread(buffer,4L,1,fp);
	for(i=3;i>=0;i--){
		height1=height1*256+buffer[i];	//hex to dec
	} 
	printf("height1:%d\n",height1);
	printf("size1:%d\n\n",size1);
	fclose(fp);
	
	
	/******deal with buffer2(is same as buffer1)******/
	if(!(fp=fopen(datapath,"rb"))){
		exit(0);
	}
	stat(datapath,&statbuff);
	size2=(statbuff.st_size);
	buffer2=(unsigned char*)malloc(size2);
	fread(buffer2,size2,1,fp);
	
	rewind(fp);
	fseek(fp,18L,0);
	fread(buffer,4L,1,fp);
	for(i=3;i>=0;i--){
		length2=length2*256+buffer[i];
	}
	printf("length2:%d\n",length2);
	
	rewind(fp);
	fseek(fp,22L,0);
	fread(buffer,4L,1,fp);
	for(i=3;i>=0;i--){
		height2=height2*256+buffer[i];
	}
	printf("height2:%d\n",height2);
	printf("size2:%d\n\n",size2);
	fclose(fp);
	
/******/	
	//1pix 3bytes(R,G,B)
	unsigned long int line=0;
	int header=54;	//bmp file header 54 bytes
	i=header;	//offset
    j=header;
    
    //must be multiples of 4
	if ((length1)%4!=0){
		length1+=4-((length1)%4);
	}
	if ((length2)%4!=0){
		length2+=4-((length2)%4);
	}
	
	//Steg process
	while(line<height2){
		for(	 ;i<(header+(line)*length1*3+length2*3);i++,j++)
		{
			//write in red plane zero 
			buffer1[i]=buffer1[i]&0xFE;	
			buffer2[j]=buffer2[j]&0x01;	
			buffer1[i]=buffer1[i]|buffer2[j];
		}
		line++;
		while(i<(header+line*length1*3)) i++;
	}

	//saved
	if(!(fp=fopen("D:\\solved.bmp","wb"))){ 
		exit(0); 
	} 
	fwrite(buffer1,size1,1,fp);
	fclose(fp);
	printf("steg execute successfully!\n");
}
int main(void){
	stegPic();
	return 0;
}
