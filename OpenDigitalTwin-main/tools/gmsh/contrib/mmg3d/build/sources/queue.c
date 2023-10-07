/****************************************************************************
Logiciel initial: MMG3D Version 4.0
Co-auteurs : Cecile Dobrzynski et Pascal Frey.
Propriétaires :IPB - UPMC -INRIA.

Copyright © 2004-2005-2006-2007-2008-2009-2010-2011, 
diffusé sous les termes et conditions de la licence publique générale de GNU
Version 3 ou toute version ultérieure.  

Ce fichier est une partie de MMG3D.
MMG3D est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier
suivant les termes de la licence publique générale de GNU
Version 3 ou toute version ultérieure.
MMG3D est distribué dans l'espoir qu'il sera utile, mais SANS 
AUCUNE GARANTIE ; sans même garantie de valeur marchande.  
Voir la licence publique générale de GNU pour plus de détails.
MMG3D est diffusé en espérant qu’il sera utile, 
mais SANS AUCUNE GARANTIE, ni explicite ni implicite, 
y compris les garanties de commercialisation ou 
d’adaptation dans un but spécifique. 
Reportez-vous à la licence publique générale de GNU pour plus de détails.
Vous devez avoir reçu une copie de la licence publique générale de GNU 
en même temps que ce document. 
Si ce n’est pas le cas, aller voir <http://www.gnu.org/licenses/>.
/****************************************************************************
Initial software: MMG3D Version 4.0
Co-authors: Cecile Dobrzynski et Pascal Frey.
Owners: IPB - UPMC -INRIA.

Copyright © 2004-2005-2006-2007-2008-2009-2010-2011, 
spread under the terms and conditions of the license GNU General Public License 
as published Version 3, or (at your option) any later version.

This file is part of MMG3D
MMG3D is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.
MMG3D is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with MMG3D. If not, see <http://www.gnu.org/licenses/>.  
****************************************************************************/
#include "mesh.h"
 
pQueue MMG_kiuini(pMesh mesh,int nbel,double declic,int base) {
  pQueue   q;
  pTetra   pt;
  int      k;

  q = (Queue*)M_malloc(sizeof(Queue),"kiuini");
  assert(q);
  q->stack = (int*)M_calloc((1+nbel),sizeof(int),"kiuini.stack");
  assert(q->stack);

  q->cur = 0;
  for (k=1; k<=mesh->ne; k++) {
    pt = &mesh->tetra[k];
    if ( !pt->v[0] || pt->qual < declic )    continue;
    else if ( base > 0 && pt->flag < base )  continue;

    q->stack[q->cur] = k;
    q->cur = k;
  }

  return(q);
}


void MMG_kiufree(pQueue q) {
  M_free(q->stack);
  M_free(q);
}


int MMG_kiudel(pQueue q,int iel) {
  int   k;

  if ( !q->stack[0] )
    return(0);

  else if ( q->cur != iel && !q->stack[iel] )
    return(0);

  else if ( iel == q->stack[0] ) {
    if ( iel == q->cur ) {
      q->cur = 0;
      q->stack[0] = 0;
      return(1);
    }
    else {
      q->stack[0]   = q->stack[iel];
      q->stack[iel] = 0; 
      return(1);
    }
  }

  else {
    for (k=iel-1; k>0; k--)
      if ( q->stack[k] == iel )  break;
assert(k>0);
    if ( iel == q->cur ) {
      q->cur        = k;
      q->stack[k]   = 0;
      q->stack[iel] = 0;
      return(1);
    }
    else {
      q->stack[k]   = q->stack[iel];
      q->stack[iel] = 0;
      return(1);
    }
  }

  return(0);
}


int MMG_kiuput(pQueue q,int iel) {
  int    k;

  if ( !q->stack[0] )
    return(0);

  else if ( iel == q->cur || q->stack[iel] )
    return(0);

  else if ( iel > q->cur ) {
    q->stack[q->cur] = iel;
    q->stack[iel]    = 0;
    q->cur = iel;
    return(1);
  }

  else if ( iel < q->stack[0] ) {
    q->stack[iel] = q->stack[0];
    q->stack[0]   = iel;
    return(1);
  }

  else {
    for (k=iel-1; k>=0; k--)
      if ( q->stack[k] )  break;
assert(k>-1);
    q->stack[iel] = q->stack[k];
    q->stack[k]   = iel;
    return(1);
  }

  return(0);
}


int MMG_kiupop(pQueue q) {
  int  cur;

  cur = q->stack[0];
  q->stack[0]   = q->stack[cur];
  q->stack[cur] = 0;
  if ( q->cur == cur )  q->cur = 0;

  return(cur);
}


