#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BLOCK 1024

char storage[BLOCK];
char e[]="Exit",pd[]="Disintegrate the File",ps[]="Assemble the File";

void display(int c1, int c2, int c3)
{

   textcolor(c1);
   gotoxy(20,6);
   cprintf("%s",pd);

   textcolor(c2);
   gotoxy(20,8);
   cprintf("%s",ps);

   textcolor(c3);
   gotoxy(20,10);
   cprintf("%s",e);
}

int getresp()
{
 char ans;
 int ctr=1,x;

 while(ans!='\r')
 {
  clrscr();
  textcolor(7);
  gotoxy(15,1);
  cprintf("Welcome to \"FILE disINTEGRATOR\"  BY TEJINDER SINGH");
  printf("\n");
  for(x=4;x<=76;x++)
   cprintf("%c",196);
   printf("\n");
   gotoxy(7,3);
   cprintf("To Change the option : Use TAB key");
   printf("\n");
   gotoxy(7,4);
   cprintf("To Select the option : Use ENTER key");

  switch(ctr)
  {
   case 1 : display(14,7,7); break;
   case 2 : display(7,14,7); break;
   case 3 : display(7,7,14); break;
  }

  ans=getch();
  if(ans=='\t')
  {
   if(ctr==1)
     ctr=2;
    else if(ctr==2)
	 ctr=3;
	 else if(ctr==3)
	      ctr=1;

  }
  }
  textcolor(7);
  return ctr;
}


int CopyData(char *outfile, char *infile, unsigned int offset, unsigned int size, float *filesize)
{

 unsigned int bytes,status=1,counter;
 int inptr,outptr;
 *filesize=0.0;

 inptr = open(infile,O_RDONLY|O_BINARY);
 if(inptr==-1)
 {printf("\n\t");
  cprintf("File \"%s\" Does not exist",infile);
  getch();
  exit(0);
 }
 outptr = open(outfile, O_APPEND|O_BINARY|O_RDWR,S_IWRITE);
 if(outptr==-1)
 outptr = open(outfile, O_CREAT|O_BINARY|O_RDWR,S_IWRITE);

 if(outptr==-1)
 {
  printf("Error in creating \"%s\" file\n",outfile);
  getch();
  exit(0);
 }
 for(counter=0;counter<offset;counter++)
   bytes=read(inptr,storage,BLOCK);

 for(counter=0;counter<size;counter++)
 {
   bytes=read(inptr,storage,BLOCK);
   *filesize = *filesize + bytes;
   if(bytes>0)
    write(outptr,storage,bytes);
   if(bytes>0 && bytes<BLOCK)
    status=0;

 }
 close(inptr);
 close(outptr);
 return status;
}

void Convert(float *filesize,char **rep)
{
 unsigned int ktr=0;

 while((*filesize)>1024)
  {
   ktr++;
   (*filesize)=(*filesize)/1024.0;
  }

 switch(ktr)
 {
  case 0:   *rep="Bytes";
	    break;
  case 1:   *rep="Kilo Byte";
	    break;
  case 2:   *rep="Mega Byte";
	    break;
  case 3:   *rep="Giga Byte";
	    break;
  case 4:   *rep="Tera Byte";
	    break;
  case 5:   *rep="Peta Byte";
	    break;
  case 6:   *rep="Exa Byte";
	    break;
  default:  *rep="Out-of-Range";
 }
}



void BuildStatusBar(float filesize)
{
 int x,y;
 char *reply;
 clrscr();
 /* First pass */
 textcolor(BLUE);
 for(y=0;y<=15;y++)
 for(x=0;x<=79;x++)
 cprintf("%c",219);
 /* Horizontal upperline */
 textcolor(15);
 textbackground(BLUE);
 gotoxy(3,1);
 cprintf("%c",218);
 gotoxy(4,1);
 for(x=4;x<=77;x++)
 cprintf("%c",196);
 cprintf("%c",191);

 /* Vertical left right lines */
 for(y=2;y<=15;y++)
 {
  gotoxy(3,y);
  cprintf("%c",179);
  gotoxy(78,y);
  cprintf("%c",179);
 }

/* Horizontal bottom line */
 gotoxy(3,16);
 cprintf("%c",192);
 gotoxy(4,16);
 for(x=4;x<=77;x++)
 cprintf("%c",196);
 cprintf("%c",217);

/* blank status bar */
 gotoxy(15,10);
 textcolor(15);
 for(x=0;x<=56;x++)
 cprintf("%c",177);

 Convert(&filesize,&reply);
 gotoxy(6,6);
 cprintf("File Size          : %.2f %s",filesize,reply);
}

void StatusBar(char *orgname, char *tname,unsigned int x,unsigned int present)
{
 int y;
 textbackground(BLUE);
 gotoxy(20,2);
 textcolor(11);
 cprintf("\"FILE disINTEGRATOR\" By Tejinder Singh");
 gotoxy(10,3);
 for(y=0;y<62;y++)
 cprintf("%c",205);
 textcolor(15);
 gotoxy(6,5);
 cprintf("Building the File  : %s",orgname);
 gotoxy(6,7);
 cprintf("Files Present      : %d",present);
 gotoxy(6,8);
 cprintf("File being Extracted : %s",tname);

 /* filling status bar */
 textcolor(14);
 gotoxy(6,10);
 cprintf("%3d%c complete ",x*2,37);
 gotoxy(20+x,10);
 delay(10);       /* Speed for moving strip */
 cprintf("%c",219);
 cprintf("%c",219);
 delay(200);
}

void Result(char *orgname,unsigned int x)
{
  int i;
  textcolor(BLUE);
  gotoxy(6,8);
  for(i=0;i<40;i++)
  cprintf("%c",219);
  textcolor(15);
  gotoxy(6,5);
  cprintf("File \"%s\" is Built Successfully",orgname);
  gotoxy(6,8);
  cprintf("Total number of files used %d",x);
  delay(2000);
  textcolor(14);
  gotoxy(10,12);
  cprintf("Press any key to exit");
  getch();
  /* Restore the defaults */
  textcolor(7);
  textbackground(0);
  clrscr();
}

float FindSize(char *infile)
{
 unsigned int bytes;
 float filesize=0.0;
 int inptr;
 inptr=open(infile,O_RDONLY|O_BINARY);
 if(inptr==-1)
 {
  printf("File %s does not exist",infile);
  getch();
  exit(0);
 }

 do{
 bytes=read(inptr,storage,BLOCK);
 filesize=filesize + bytes;
 }while(bytes>0);
 close(inptr);
 return filesize;
}

