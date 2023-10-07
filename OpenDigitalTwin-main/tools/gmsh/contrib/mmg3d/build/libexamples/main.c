/*Authors Cécile Dobrzynski

Example for using mmg3dlib

*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

#include "libmmg3d.h"
int main(int argc,char *argv[]) {
  MMG_pMesh  mmgMesh;
  MMG_pSol   mmgSol;
	int        opt[9],k;
  
  fprintf(stdout,"  -- TEST MMG3DLIB \n");

  mmgMesh = (MMG_pMesh)calloc(1,sizeof(MMG_Mesh));
  assert(mmgMesh);

  /* allocation */
  mmgMesh->np     = 12;
  mmgMesh->nt     = 0;
  mmgMesh->ne     = 12;


  mmgMesh->npmax  = 500000;
  mmgMesh->ntmax  = 1000000;
  mmgMesh->nemax  = 3000000;

  mmgMesh->point = (MMG_pPoint)calloc(mmgMesh->npmax+1,sizeof(MMG_Point));
  assert(mmgMesh->point);
  mmgMesh->tetra = (MMG_pTetra)calloc(mmgMesh->nemax+1,sizeof(MMG_Tetra));
  assert(mmgMesh->tetra);
  mmgMesh->tria  = (MMG_pTria)calloc(mmgMesh->ntmax+1,sizeof(MMG_Tria));
  assert(mmgMesh->tria);
  mmgMesh->adja = (int*)calloc(4*mmgMesh->nemax+5,sizeof(int));
  assert(mmgMesh->adja);
  mmgMesh->disp = (MMG_pDispl)calloc(1,sizeof(MMG_Displ));
  assert(mmgMesh->disp);
  mmgMesh->disp->mv = (double*)calloc(3*(mmgMesh->npmax + 1),sizeof(double));
  assert(mmgMesh->disp->mv);
  mmgMesh->disp->alpha = (short*)calloc(mmgMesh->npmax+1,sizeof(short));
  assert(mmgMesh->disp->alpha);
 
  /*coordinates vertices*/ 
	mmgMesh->point[1].c[0]  = 0.;  mmgMesh->point[1].c[1]  = 0.; mmgMesh->point[1].c[2]  = 0.; mmgMesh->point[1].ref  = 0; 
	mmgMesh->point[2].c[0]  = 0.5; mmgMesh->point[2].c[1]  = 0;  mmgMesh->point[2].c[2]  = 0;  mmgMesh->point[2].ref  = 0; 
	mmgMesh->point[3].c[0]  = 0.5; mmgMesh->point[3].c[1]  = 0;  mmgMesh->point[3].c[2]  = 1;  mmgMesh->point[3].ref  = 0; 
	mmgMesh->point[4].c[0]  = 0;   mmgMesh->point[4].c[1]  = 0;  mmgMesh->point[4].c[2]  = 1;  mmgMesh->point[4].ref  = 0; 
	mmgMesh->point[5].c[0]  = 0;   mmgMesh->point[5].c[1]  = 1;  mmgMesh->point[5].c[2]  = 0;  mmgMesh->point[5].ref  = 0; 
	mmgMesh->point[6].c[0]  = 0.5; mmgMesh->point[6].c[1]  = 1;  mmgMesh->point[6].c[2]  = 0;  mmgMesh->point[6].ref  = 0; 
	mmgMesh->point[7].c[0]  = 0.5; mmgMesh->point[7].c[1]  = 1;  mmgMesh->point[7].c[2]  = 1;  mmgMesh->point[7].ref  = 0; 
	mmgMesh->point[8].c[0]  = 0;   mmgMesh->point[8].c[1]  = 1;  mmgMesh->point[8].c[2]  = 1;  mmgMesh->point[8].ref  = 0; 
	mmgMesh->point[9].c[0]  = 1;   mmgMesh->point[9].c[1]  = 0;  mmgMesh->point[9].c[2]  = 0;  mmgMesh->point[9].ref  = 0; 
	mmgMesh->point[10].c[0] = 1;   mmgMesh->point[10].c[1] = 1;  mmgMesh->point[10].c[2] = 0;  mmgMesh->point[10].ref = 0; 
	mmgMesh->point[11].c[0] = 1;   mmgMesh->point[11].c[1] = 0;  mmgMesh->point[11].c[2] = 1;  mmgMesh->point[11].ref = 0; 
	mmgMesh->point[12].c[0] = 1;   mmgMesh->point[12].c[1] = 1;  mmgMesh->point[12].c[2] = 1;  mmgMesh->point[12].ref = 0; 
  
  /*tetra*/
	mmgMesh->tetra[1].v[0]  = 1;  mmgMesh->tetra[1].v[1]  = 2;  mmgMesh->tetra[1].v[2]  = 4;  mmgMesh->tetra[1].v[3]  = 8;  mmgMesh->tetra[1].ref  = 1; 
	mmgMesh->tetra[2].v[0]  = 8;  mmgMesh->tetra[2].v[1]  = 3;  mmgMesh->tetra[2].v[2]  = 2;  mmgMesh->tetra[2].v[3]  = 7;  mmgMesh->tetra[2].ref  = 1; 
	mmgMesh->tetra[3].v[0]  = 2;  mmgMesh->tetra[3].v[1]  = 5;  mmgMesh->tetra[3].v[2]  = 6;  mmgMesh->tetra[3].v[3]  = 8;  mmgMesh->tetra[3].ref  = 1; 
	mmgMesh->tetra[4].v[0]  = 8;  mmgMesh->tetra[4].v[1]  = 5;  mmgMesh->tetra[4].v[2]  = 1;  mmgMesh->tetra[4].v[3]  = 2;  mmgMesh->tetra[4].ref  = 1; 
	mmgMesh->tetra[5].v[0]  = 2;  mmgMesh->tetra[5].v[1]  = 7;  mmgMesh->tetra[5].v[2]  = 8;  mmgMesh->tetra[5].v[3]  = 6;  mmgMesh->tetra[5].ref  = 1; 
	mmgMesh->tetra[6].v[0]  = 2;  mmgMesh->tetra[6].v[1]  = 4;  mmgMesh->tetra[6].v[2]  = 3;  mmgMesh->tetra[6].v[3]  = 8;  mmgMesh->tetra[6].ref  = 1; 
	mmgMesh->tetra[7].v[0]  = 2;  mmgMesh->tetra[7].v[1]  = 9;  mmgMesh->tetra[7].v[2]  = 3;  mmgMesh->tetra[7].v[3]  = 7;  mmgMesh->tetra[7].ref  = 2; 
	mmgMesh->tetra[8].v[0]  = 7;  mmgMesh->tetra[8].v[1]  = 11; mmgMesh->tetra[8].v[2]  = 9;  mmgMesh->tetra[8].v[3]  = 12; mmgMesh->tetra[8].ref  = 2; 
	mmgMesh->tetra[9].v[0]  = 9;  mmgMesh->tetra[9].v[1]  = 6;  mmgMesh->tetra[9].v[2]  = 10; mmgMesh->tetra[9].v[3]  = 7;  mmgMesh->tetra[9].ref  = 2; 
	mmgMesh->tetra[10].v[0] = 7;  mmgMesh->tetra[10].v[1] = 6;  mmgMesh->tetra[10].v[2] = 2;  mmgMesh->tetra[10].v[3] = 9;  mmgMesh->tetra[10].ref = 2;
	mmgMesh->tetra[11].v[0] = 9;  mmgMesh->tetra[11].v[1] = 12; mmgMesh->tetra[11].v[2] = 7;  mmgMesh->tetra[11].v[3] = 10; mmgMesh->tetra[11].ref = 2; 
	mmgMesh->tetra[12].v[0] = 9;  mmgMesh->tetra[12].v[1] = 3;  mmgMesh->tetra[12].v[2] = 11; mmgMesh->tetra[12].v[3] = 7;  mmgMesh->tetra[12].ref = 2;
  
  
  /*metric*/
  mmgSol           = (MMG_pSol)calloc(1,sizeof(MMG_Sol));
  assert(mmgSol); 
  mmgSol->offset = 1;

  /*scalaire size*/
	mmgSol->np = mmgMesh->np;
	mmgSol->npmax = mmgMesh->npmax;
  mmgSol->met    = (double*)calloc(mmgSol->npmax+1,(int)mmgSol->offset*sizeof(double));
  assert(mmgSol->met);  
  mmgSol->metold = (double*)calloc(mmgSol->npmax+1,mmgSol->offset*sizeof(double));
  assert(mmgSol->metold);
	for(k=1 ; k<=mmgMesh->np ; k++) {
		mmgSol->met[k] = 0.5;           
	}
   
  opt[0]=4; //splitting
  opt[1]=0; //debug
  opt[2]=64; //par default 64
  opt[3]=0;//noswap
  opt[4]=0;//noinsert
  opt[5]=0;//nomove
  opt[6]=5; //imprim
	opt[7]=3;  //renum
	opt[8]=500; //renum
  
  if(MMG_mmg3dlib(opt,mmgMesh,mmgSol)) {
		fprintf(stdout,"BAD ENDING OF MMG3DLIB\n");
  }

  /*save result*/
	MMG_saveMesh(mmgMesh,"result.mesh");

  /*save metric*/
	MMG_saveSol(mmgMesh,mmgSol,"result.sol");

  /* free mem */
  free(mmgMesh->point);
  free(mmgMesh->disp->alpha);
  free(mmgMesh->disp->mv);
  free(mmgMesh->disp);
  free(mmgMesh->tria);
  free(mmgMesh->tetra);
  free(mmgMesh);
  if ( mmgSol->npfixe )  free(mmgSol->met);
  free(mmgSol);


  return(0);
}
