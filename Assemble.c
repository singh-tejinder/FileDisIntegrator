#include"disector.h"

void main()
{
 FILE *fn;
 char lname[20],orgname[20],gname[20],tname[20],str[10];
 char *rep;
 float filesize;
 int fptr;
 unsigned int offset,size,ctr, status, mode, residue, gx,tx,poss;
 struct file_tag
 {
  char name[20];
  unsigned int num;
  unsigned int unit;
  float fsize;
 }file_info;
 do{
 mode=getresp();
 if(mode==1)
 {
 status=0;
 while(1)
 {
 clrscr();
 printf("\t\t \"FILE disINTEGRATOR\" By Tejinder Singh\n");
 for(tx=0;tx<78;tx++)
 printf("%c",196);
 printf("\n");

  printf("Enter File Name (with file extension)\nFormat (NNNNN.EEE) : ");
  scanf("%s",orgname);
  for(ctr=0;ctr<strlen(orgname);ctr++)
  {
   if(orgname[(strlen(orgname)-ctr)]=='.')
   status=1;
  }
  if(status==1)
  break;
 }
 strupr(orgname);

 for(ctr=0;ctr<strlen(orgname);ctr++)
 {
  if(orgname[ctr]=='.')
  break;
  gname[ctr]=orgname[ctr];
 }
 gname[ctr]='\0';
 printf("Finding the file size...\r");
 filesize=FindSize(orgname);
 file_info.fsize=filesize;
 Convert(&filesize,&rep);
 printf("File Size     : %.2f %s \n\n",filesize,rep);

 strcpy(file_info.name,orgname);
 strcpy(lname,gname);
 strcpy(tname,lname);
 strcat(lname,".LOG");
 ctr=1;
 offset=0;

 printf("Size of one File (in K Bytes), Range: 1 to 32768 \n");
 printf("For making 1Mbyte, write 1024 : ");
 scanf("%d",&size);
 poss=(file_info.fsize)/(size*1024.0);
 poss++;
 printf("Approx. Number of files :%d",poss);
 printf("\n\nFiles created...\n");
 for(tx=0;tx<78;tx++)
 printf("%c",205);
 printf("\n");
 filesize=0.0;
 do{
   itoa(ctr, str, 10);
   strcat(tname,str);
   strcat(tname,".DAT");
   status=CopyData(tname,orgname,offset,size,&filesize);
   Convert(&filesize,&rep);
   if((ctr%2)==0)
   printf("%s (%.2f %s)\n",tname,filesize,rep);
   else
   printf("%s (%.2f %s)\t\t",tname,filesize,rep);
   offset+=size;
   strcpy(tname,gname);
   ctr++;
   if(poss<ctr)
   break;
   if(filesize> file_info.fsize)
   break;
   if(kbhit())
   if(getch()=='e')
   break;
   }while(status);
 ctr--;
 fn=fopen(lname,"wb");
 if(fn==NULL)
 {
   printf("Cannot Open \"%s\"File",lname);
   getch();
   exit(0);
 }
 strcpy(file_info.name,orgname);
 file_info.num=ctr;
 file_info.unit=size;
 fwrite(&file_info,sizeof(file_info),1,fn);
 fclose(fn);
 if((ctr%2) != 0)
 printf("\n");
  for(tx=0;tx<78;tx++)
 printf("%c",205);
 printf("\n");
 printf("Total Number of Files created : %d\n",ctr);
 printf("File \" %s \" Disintegrated SUCCESSFULLY\n",orgname);
 printf("Press any key to exit\n");
 getch();
}
else if(mode==2)
{
 clrscr();
 printf("Enter LOG File Name : ");
 scanf("%s",lname);
 strupr(lname);
 for(tx=0;tx<strlen(lname);tx++)
 {
  if(lname[tx]=='.')
  break;
  tname[tx]=lname[tx];
 }
  tname[tx]='\0';
  strcpy(lname,tname);
  strcpy(gname,lname);
  strcat(lname,".LOG");

  lname[ (strlen(lname)-4) ]='\0';
  strcat(lname,".LOG");

 fn=fopen(lname,"rb");
 if(fn==NULL)
 {
   printf("Cannot Open \"%s\"File",lname);
   getch();
   exit(0);
 }
 fread(&file_info,sizeof(file_info),1,fn);
 fclose(fn);
 /* Checking the presence of all files */
 for(ctr=1;ctr<=file_info.num;ctr++)
  {
   itoa(ctr,str,10);
   strcat(tname,str);
   strcat(tname,".DAT");
   fptr = open(tname,O_RDONLY|O_BINARY);
   if(fptr==-1)
   {
    printf("\nError : File \" %s\" is not present\n",tname);
    printf("Make sure to bring all the files and \n");
    printf("then restart the program\n");
    getch();
    exit(0);
   }
   close(fptr);
   strcpy(tname,gname);
  }
 offset=0;
 size=file_info.unit;
 residue=0;
 gx=0;
 BuildStatusBar(file_info.fsize);
 for(ctr=1; ctr<= file_info.num; ctr++)
 {
  itoa(ctr, str, 10);
  strcat(tname,str);
  strcat(tname,".DAT");
  gx=(50.0*ctr)/(file_info.num);
  for(tx=residue;tx<gx;tx++)
  StatusBar(file_info.name,tname,tx,file_info.num);

  status=CopyData(file_info.name,tname,offset,size,&filesize);
  residue=gx;
  strcpy(tname,gname);
  }
  /* Removing all the files */
  for(ctr=1;ctr<=file_info.num;ctr++)
  {
   itoa(ctr,str,10);
   strcat(tname,str);
   strcat(tname,".DAT");
   remove(tname);
   strcpy(tname,gname);
  }
  StatusBar(file_info.name,tname,50,file_info.num);
  Result(file_info.name,file_info.num);
  remove(lname);
 }
 else if(mode==3)
 exit(0);
 }while(1);
}
