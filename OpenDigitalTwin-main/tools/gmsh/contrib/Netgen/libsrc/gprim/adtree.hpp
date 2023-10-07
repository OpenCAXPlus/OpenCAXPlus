#ifndef FILE_ADTREE
#define FILE_ADTREE

/* *************************************************************************/
/* File:   adtree.hh                                                       */
/* Author: Joachim Schoeberl                                               */
/* Date:   16. Feb. 98                                                     */
/* Redesigned by Wolfram Muehlhuber, May 1998                              */
/* *************************************************************************/


namespace netgen
{

/**
  Alternating Digital Tree
 */

// #include "../include/mystdlib.h"
// #include "../include/myadt.hpp"

class ADTreeNode
{
public:
  ADTreeNode *left, *right, *father;
  int dim;
  float sep;
  float *data;
  float *boxmin;
  float *boxmax;
  int pi;
  int nchilds;

  ADTreeNode (int adim);
  ~ADTreeNode ();

  friend class ADTree;
};


class ADTreeCriterion
{
public:
  ADTreeCriterion() { }
  virtual int Eval (const ADTreeNode * node) const = 0;
};


class ADTree
{
  int dim;
  ADTreeNode * root;
  float *cmin, *cmax;
  Array<ADTreeNode*> ela;
  const ADTreeCriterion * criterion; 

  Array<ADTreeNode*> stack;
  Array<int> stackdir;
  int stackindex;

public:
  ADTree (int adim, const float * acmin, 
	   const float * acmax);
  ~ADTree ();

  void Insert (const float * p, int pi);
  // void GetIntersecting (const float * bmin, const float * bmax,
  //			Array<int> & pis) const;
  void SetCriterion (ADTreeCriterion & acriterion);
  void Reset ();
  int Next ();
  void GetMatch (Array<int> & matches);

  void DeleteElement (int pi);


  void Print (ostream & ost) const
    { PrintRec (ost, root); }

  void PrintRec (ostream & ost, const ADTreeNode * node) const;
};



class ADTreeNode3
{
public:
  ADTreeNode3 *left, *right, *father;
  float sep;
  float data[3];
  int pi;
  int nchilds;

  ADTreeNode3 ();
  void DeleteChilds ();
  friend class ADTree3;

  static BlockAllocator ball;
  void * operator new(size_t);
  void operator delete (void *);
};


class ADTree3
{
  ADTreeNode3 * root;
  float cmin[3], cmax[3];
  Array<ADTreeNode3*> ela;

public:
  ADTree3 (const float * acmin, 
	   const float * acmax);
  ~ADTree3 ();

  void Insert (const float * p, int pi);
  void GetIntersecting (const float * bmin, const float * bmax,
			Array<int> & pis) const;
  
  void DeleteElement (int pi);


  void Print (ostream & ost) const
    { PrintRec (ost, root); }

  void PrintRec (ostream & ost, const ADTreeNode3 * node) const;
};


/*

// divide each direction
#define ADTN_DIV 10
class ADTreeNode3Div
{
public:
  ADTreeNode3Div *father;
  ADTreeNode3Div *childs[ADTN_DIV];

  float minx, dist;
  float data[3];
  int pi;
  int nchilds;

  ADTreeNode3Div ();
  void DeleteChilds ();
  friend class ADTree3Div;

  static BlockAllocator ball;
  void * operator new(size_t);
  void operator delete (void *);
};


class ADTree3Div
{
  ADTreeNode3Div * root;
  float cmin[3], cmax[3];
  Array<ADTreeNode3Div*> ela;

public:
  ADTree3Div (const float * acmin, 
	   const float * acmax);
  ~ADTree3Div ();

  void Insert (const float * p, int pi);
  void GetIntersecting (const float * bmin, const float * bmax,
			Array<int> & pis) const;
  
  void DeleteElement (int pi);


  void Print (ostream & ost) const
    { PrintRec (ost, root); }

  void PrintRec (ostream & ost, const ADTreeNode3Div * node) const;
};




#define ADTN_SIZE 10

// multiple entries
class ADTreeNode3M
{
public:
  ADTreeNode3M *left, *right, *father;
  float sep;
  float data[ADTN_SIZE][3];
  int pi[ADTN_SIZE];
  int nchilds;

  ADTreeNode3M ();
  void DeleteChilds ();
  friend class ADTree3M;

  static BlockAllocator ball;
  void * operator new(size_t);
  void operator delete (void *);
};


class ADTree3M
{
  ADTreeNode3M * root;
  float cmin[3], cmax[3];
  Array<ADTreeNode3M*> ela;

public:
  ADTree3M (const float * acmin, 
	   const float * acmax);
  ~ADTree3M ();

  void Insert (const float * p, int pi);
  void GetIntersecting (const float * bmin, const float * bmax,
			Array<int> & pis) const;
  
  void DeleteElement (int pi);


  void Print (ostream & ost) const
    { PrintRec (ost, root); }

  void PrintRec (ostream & ost, const ADTreeNode3M * node) const;
};






class ADTreeNode3F
{
public:
  ADTreeNode3F *father;
  ADTreeNode3F *childs[8];
  float sep[3];
  float data[3];
  int pi;
  int nchilds;

  ADTreeNode3F ();
  void DeleteChilds ();
  friend class ADTree3F;

  static BlockAllocator ball;
  void * operator new(size_t);
  void operator delete (void *);
};

// fat tree
class ADTree3F
{
  ADTreeNode3F * root;
  float cmin[3], cmax[3];
  Array<ADTreeNode3F*> ela;

public:
  ADTree3F (const float * acmin, 
	   const float * acmax);
  ~ADTree3F ();

  void Insert (const float * p, int pi);
  void GetIntersecting (const float * bmin, const float * bmax,
			Array<int> & pis) const;
  
  void DeleteElement (int pi);


  void Print (ostream & ost) const
    { PrintRec (ost, root); }

  void PrintRec (ostream & ost, const ADTreeNode3F * node) const;
};




class ADTreeNode3FM
{
public:
  ADTreeNode3FM *father;
  ADTreeNode3FM *childs[8];
  float sep[3];
  float data[ADTN_SIZE][3];
  int pi[ADTN_SIZE];
  int nchilds;

  ADTreeNode3FM ();
  void DeleteChilds ();
  friend class ADTree3FM;

  static BlockAllocator ball;
  void * operator new(size_t);
  void operator delete (void *);
};

// fat tree
class ADTree3FM
{
  ADTreeNode3FM * root;
  float cmin[3], cmax[3];
  Array<ADTreeNode3FM*> ela;

public:
  ADTree3FM (const float * acmin, 
	   const float * acmax);
  ~ADTree3FM ();

  void Insert (const float * p, int pi);
  void GetIntersecting (const float * bmin, const float * bmax,
			Array<int> & pis) const;
  
  void DeleteElement (int pi);


  void Print (ostream & ost) const
    { PrintRec (ost, root); }

  void PrintRec (ostream & ost, const ADTreeNode3FM * node) const;
};



*/





class ADTreeNode6
{
public:
  ADTreeNode6 *left, *right, *father;
  float sep;
  float data[6];
  int pi;
  int nchilds;

  ADTreeNode6 ();
  void DeleteChilds ();
  friend class ADTree6;

  static BlockAllocator ball;
  void * operator new(size_t);
  void operator delete (void *);
};


class ADTree6
{
  ADTreeNode6 * root;
  float cmin[6], cmax[6];
  Array<ADTreeNode6*> ela;

public:
  ADTree6 (const float * acmin, 
	   const float * acmax);
  ~ADTree6 ();

  void Insert (const float * p, int pi);
  void GetIntersecting (const float * bmin, const float * bmax,
			Array<int> & pis) const;
  
  void DeleteElement (int pi);

  
  void Print (ostream & ost) const
  { PrintRec (ost, root); }
  int Depth () const
  { return DepthRec (root); }
  int Elements () const
  { return ElementsRec (root); }

  void PrintRec (ostream & ost, const ADTreeNode6 * node) const;
  int DepthRec (const ADTreeNode6 * node) const;
  int ElementsRec (const ADTreeNode6 * node) const;

  void PrintMemInfo (ostream & ost) const;
};




/*

class ADTreeNode6F
{
public:
  ADTreeNode6F * father;
  ADTreeNode6F * childs[64];
  
  float sep[6];
  float data[6];
  int pi;
  int nchilds;

  ADTreeNode6F ();
  void DeleteChilds ();
  friend class ADTree6F;

  static BlockAllocator ball;
  void * operator new(size_t);
  void operator delete (void *);
};


class ADTree6F
{
  ADTreeNode6F * root;
  float cmin[6], cmax[6];
  Array<ADTreeNode6F*> ela;

public:
  ADTree6F (const float * acmin, 
	   const float * acmax);
  ~ADTree6F ();

  void Insert (const float * p, int pi);
  void GetIntersecting (const float * bmin, const float * bmax,
			Array<int> & pis) const;
  
  void DeleteElement (int pi);


  void Print (ostream & ost) const
    { PrintRec (ost, root); }
  int Depth () const
    { return DepthRec (root); }

  void PrintRec (ostream & ost, const ADTreeNode6F * node) const;
  int DepthRec (const ADTreeNode6F * node) const;
};







*/





class Point3dTree 
{
  ADTree3 * tree;

public:
  DLL_HEADER Point3dTree (const Point<3> & pmin, const Point<3> & pmax);
  DLL_HEADER ~Point3dTree ();
  DLL_HEADER void Insert (const Point<3> & p, int pi);
  void DeleteElement (int pi) 
    { tree->DeleteElement(pi); }
  DLL_HEADER void GetIntersecting (const Point<3> & pmin, const Point<3> & pmax, 
			Array<int> & pis) const;
  const ADTree3 & Tree() const { return *tree; };
};



class Box3dTree
{
  ADTree6 * tree;
  Point<3> boxpmin, boxpmax;
public:
  Box3dTree (const Point<3> & apmin, const Point<3> & apmax);
  ~Box3dTree ();
  void Insert (const Point<3> & bmin, const Point<3> & bmax, int pi);
  void Insert (const Box<3> & box, int pi)
  {
    Insert (box.PMin(), box.PMax(), pi);
  }
  void DeleteElement (int pi) 
    { tree->DeleteElement(pi); }
  void GetIntersecting (const Point<3> & pmin, const Point<3> & pmax, 
			Array<int> & pis) const;

  const ADTree6 & Tree() const { return *tree; };
};

}

#endif
