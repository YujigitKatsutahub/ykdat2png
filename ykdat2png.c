#define VERSION "1.0.1"
/*
  ykgnuplot edit  by yuji katsuta 2023.05.13
 */
/**
* @file ykdat2png
* @brief gnuplot によるデータのプロット
* @author yuji katsuta
* @date 2023.05.12
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define GPSU (8*1024)
#define SSU  (8*1024)
#define USU  (1024)
#define MAXS "1023"

/* verprint start */
int ykdebugpr=0;

#define OK  0
#define ERR 1

int checkverflag(int argc, char **argv)
{
	int i, len, max=0;
	for (i=1; i<argc; i++) {
		len=strlen(argv[i]);
		if (len>=USU) return ERR;
		if ((max+=len)>(SSU-USU)) return ERR;
		if (strcmp(argv[i],"-ver")==OK) ykdebugpr|=0x10;	
	}
	return OK;
}

#define verprint(VER)	\
	static int verf=0; \
  if ((verf==0)&&(ykdebugpr&0x10)) { \
	  fprintf( stderr, \
		  		 "%s: Ver.%s is compiled on %s of %s.\n", \
			  	 __FILE__, VER,  __TIME__, __DATE__ \
				   ); \
		verf=1; \
  }
/* verprint end */

int main(int argc, char **argv)
{
  FILE *fw=NULL;
  char gp[GPSU],ctmp[SSU],ustr[USU];
  int i,i0,x=3,y=4,uflag=0,fflag=0,lflag=1;
	int epsflag=0;

	clock_t oldclock;
	
	if (checkverflag(argc, argv)) {
		errno=75;
		perror("*** string -> too long! ***");
		return ERR;
	}
	verprint(VERSION);
	
  if (((verf==0)&&(argc==1))||((verf==1)&&(argc==2))){
    printf(
      "usage:%s [-no] [-e] [-s] [-xNo] [-yNo] [-l]"
			" filename1 [-xNo] [-yNo] [-p] f2 ...\n"
      "      %s [-e] [-s] \'-$(ex1):(ex2)\' [-p] filename1 f2 ...\n"
      "     -xNo : x axis No.,      -yNo : y axis No.            [Ver. %s]\n"
      "     \'-$(ex1):(ex2)\' : ex1 : x axis expression, ex2 : y axis exp.\n"
      "     \'-$(-1*$3):(-$4)\' : x axis -1*(column 3), y axis -(column 4)\n"
      "     -no : no title\n"
      "     -e : eps output, -s : square, -l : with line, -p : with points\n"
      ,argv[0],argv[0],VERSION);
    return 129;
  }
  fw = fopen("ykgpl.dat","w");
  if (fw == NULL) {
    perror("*** open_failed ***");
    return ERR;
  }
	if (strncmp(argv[1],"-no",3)==OK) {
		i0=2;
	}else{
		i0=1;
		fprintf(fw,"set title \'ykdat2png  [Ver. %s]\'\n",VERSION);
	}
  for (i=i0;i<argc;i++){
    if (strncmp(argv[i],"-s",2)==OK){
      fprintf(fw,"set size square\n");
			continue;
    }
    if (strncmp(argv[i],"-e",2)==OK){
      epsflag=1;
    }
	}
	if (epsflag) {
		fprintf(fw,"set term post eps enhanced color\n");
		fprintf(fw,"set output \"ykdat2pngn.eps\"\n");
		fprintf(fw,"set nokey\n");
	}else{
		fprintf(fw,"set term pngcairo\n");
		fprintf(fw,"set output \"ykdat2pngn.png\"\n");
		fprintf(fw,"set nokey\n");
	}
  sprintf(gp,"plot ");
  for (i=i0;i<argc;i++){
    if (strncmp(argv[i],"-ver",2)==OK){
      continue;
    }
    if (strncmp(argv[i],"-e",2)==OK){
      continue;
    }
    if (strncmp(argv[i],"-s",2)==OK){
      continue;
    }
    if (strncmp(argv[i],"-l",2)==OK){
      lflag=1;
      continue;
    }
    if (strncmp(argv[i],"-p",2)==OK){
      lflag=0;
      continue;
    }
    if (strncmp(argv[i],"-$",2)==OK){
      sscanf(argv[i]+2,"%"MAXS"s",ustr);
      uflag=1;
      continue;
    }
    if (strncmp(argv[i],"-x",2)==OK){
      sscanf(argv[i]+2,"%d",&x);
      uflag=0;
      continue;
    }
    if (strncmp(argv[i],"-y",2)==OK){
      sscanf(argv[i]+2,"%d",&y);
      uflag=0;
      continue;
    }
    if (fflag!=0) strcat(gp,", ");
    fflag=1;
    if (lflag==1){
      if (uflag){
				sprintf(ctmp,"\"%s\" u %s w l",argv[i],ustr);
      }else{
				sprintf(ctmp,"\"%s\" u %d:%d w l",argv[i],x,y);
      }
    }else{
      if (uflag){
				sprintf(ctmp,"\"%s\" u %s",argv[i],ustr);
      }else{
				sprintf(ctmp,"\"%s\" u %d:%d",argv[i],x,y);
      }
    }
    strcat(gp,ctmp);
  }
  strcat(gp,"\n");
	fprintf(fw,gp,NULL);
	if (epsflag) {
		fprintf(fw,"set key\n");
		fprintf(fw,"set output \"ykdat2png.eps\"\n");
		fprintf(fw,"replot\n");
		fprintf(fw,"set terminal wxt\n");
		fprintf(fw,"replot\n");
	}else{
		fprintf(fw,"set key\n");
		fprintf(fw,"set output \"ykdat2png.png\"\n");
		fprintf(fw,"replot\n");
		fprintf(fw,"set terminal wxt\n");
		fprintf(fw,"replot\n");
	}
  fprintf(fw,"pause -1 'Hit Enter key'\n");
  fclose(fw);
	system("gnuplot ykgpl.dat");
	oldclock=clock();
	while ((double)(clock()-oldclock)/CLOCKS_PER_SEC<0.01); 
	system("rm ykgpl.dat");
  return OK;
}
