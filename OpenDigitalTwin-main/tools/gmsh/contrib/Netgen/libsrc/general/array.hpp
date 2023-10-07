#ifndef FILE_Array
#define FILE_Array

/**************************************************************************/
/* File:   array.hpp                                                      */
/* Author: Joachim Schoeberl                                              */
/* Date:   01. Jun. 95                                                    */
/**************************************************************************/


namespace netgen
{

  // template <class T, int B1, int B2> class IndirectArray;



  /**
     A simple array container.
     Array represented by size and data-pointer.
     No memory allocation and deallocation, must be provided by user.
     Helper functions for printing. 
     Optional range check by macro RANGE_CHECK
  */

  template <class T, int BASE = 0>
  class FlatArray
  {
  protected:
    /// the size
    int size;
    /// the data
    T * data;
  public:

    /// provide size and memory
    FlatArray (int asize, T * adata) 
      : size(asize), data(adata) { ; }

    /// the size
    int Size() const { return size; }

    int Begin() const { return BASE; }
    int End() const { return size+BASE; }

    /// Access array. BASE-based
    T & operator[] (int i) const
    {
#ifdef DEBUG
      if (i-BASE < 0 || i-BASE >= size)
	cout << "array<" << typeid(T).name() << "> out of range, i = " << i << ", s = " << size << endl;
#endif

      return data[i-BASE]; 
    }

    /// Access array, one-based  (old fashioned)
    T & Elem (int i)
    {
#ifdef DEBUG
      if (i < 1 || i > size)
	cout << "Array<" << typeid(T).name() 
	     << ">::Elem out of range, i = " << i
	     << ", s = " << size << endl;
#endif

      return ((T*)data)[i-1]; 
    }
  
    /// Access array, one-based  (old fashioned)
    const T & Get (int i) const 
    {
#ifdef DEBUG
      if (i < 1 || i > size)
	cout << "Array<" << typeid(T).name() << ">::Get out of range, i = " << i
	     << ", s = " << size << endl;
#endif

      return ((const T*)data)[i-1]; 
    }

    /// Access array, one-based  (old fashioned)
    void Set (int i, const T & el)
    { 
#ifdef DEBUG
      if (i < 1 || i > size)
	cout << "Array<" << typeid(T).name() << ">::Set out of range, i = " << i
	     << ", s = " << size << endl;
#endif

      ((T*)data)[i-1] = el; 
    }

    /// access first element
    T & First () const
    {
      return data[0];
    }


    /// access last element. check by macro CHECK_RANGE
    T & Last () const
    {
      return data[size-1];
    }

    /// Fill array with value val
    FlatArray & operator= (const T & val)
    {
      for (int i = 0; i < size; i++)
	data[i] = val;
      return *this;
    }

    /// takes range starting from position start of end-start elements
    const FlatArray<T> Range (int start, int end)
    {
      return FlatArray<T> (end-start, data+start);
    }

    /// first position of element elem, returns -1 if element not contained in array 
    int Pos(const T & elem) const
    {
      int pos = -1;
      for(int i=0; pos==-1 && i < this->size; i++)
	if(elem == data[i]) pos = i;
      return pos;
    }

    /// does the array contain element elem ?
    bool Contains(const T & elem) const
    {
      return ( Pos(elem) >= 0 );
    }
  };



  // print array
  template <class T, int BASE>
  inline ostream & operator<< (ostream & s, const FlatArray<T,BASE> & a)
  {
    for (int i = a.Begin(); i < a.End(); i++)
      s << i << ": " << a[i] << endl;
    return s;
  }



  /** 
      Dynamic array container.
   
      Array<T> is an automatically increasing array container.
      The allocated memory doubles on overflow. 
      Either the container takes care of memory allocation and deallocation,
      or the user provides one block of data.
  */
  template <class T, int BASE = 0> 
  class Array : public FlatArray<T, BASE>
  {
  protected:
    using FlatArray<T,BASE>::size;
    using FlatArray<T,BASE>::data;

    /// physical size of array
    int allocsize;
    /// memory is responsibility of container
    bool ownmem;

  public:

    /// Generate array of logical and physical size asize
    explicit Array(int asize = 0)
      : FlatArray<T, BASE> (asize, asize ? new T[asize] : 0)
    {
      allocsize = asize; 
      ownmem = 1;
    }

    /// Generate array in user data
    Array(int asize, T* adata)
      : FlatArray<T, BASE> (asize, adata)
    {
      allocsize = asize; 
      ownmem = 0;
    }

    /// array copy 
    explicit Array (const Array<T> & a2)
      : FlatArray<T, BASE> (a2.Size(), a2.Size() ? new T[a2.Size()] : 0)
    {
      allocsize = size;
      ownmem = 1;
      for (int i = BASE; i < size+BASE; i++)
	(*this)[i] = a2[i];
    }



    /// if responsible, deletes memory
    ~Array()
    {
      if (ownmem)
	delete [] data;
    }

    /// Change logical size. If necessary, do reallocation. Keeps contents.
    void SetSize(int nsize)
    {
      if (nsize > allocsize) 
	ReSize (nsize);
      size = nsize; 
    }

    /// Change physical size. Keeps logical size. Keeps contents.
    void SetAllocSize (int nallocsize)
    {
      if (nallocsize > allocsize)
	ReSize (nallocsize);
    }


    /// Add element at end of array. reallocation if necessary.
    int Append (const T & el)
    {
      if (size == allocsize) 
	ReSize (size+1);
      data[size] = el;
      size++;
      return size;
    }

    template <typename T2, int B2>
    void Append (FlatArray<T2, B2> a2)
    {
      if (size+a2.Size() > allocsize)
	ReSize (size+a2.Size());
      for (int i = 0; i < a2.Size(); i++)
	data[size+i] = a2[i+B2];
      size += a2.Size();
    }


    /// Delete element i (0-based). Move last element to position i.
    void Delete (int i)
    {
#ifdef CHECK_Array_RANGE
      RangeCheck (i+1);
#endif

      data[i] = data[size-1];
      size--;
      //    DeleteElement (i+1);
    }


    /// Delete element i (1-based). Move last element to position i.
    void DeleteElement (int i)
    {
#ifdef CHECK_Array_RANGE
      RangeCheck (i);
#endif

      data[i-1] = data[size-1];
      size--;
    }

    /// Delete last element. 
    void DeleteLast ()
    {
      size--;
    }

    /// Deallocate memory
    void DeleteAll ()
    {
      if (ownmem)
	delete [] data;
      data = 0;
      size = allocsize = 0;
    }

    /// Fill array with val
    Array & operator= (const T & val)
    {
      FlatArray<T, BASE>::operator= (val);
      return *this;
    }

    /// array copy
    Array & operator= (const Array & a2)
    {
      SetSize (a2.Size());
      for (int i = BASE; i < size+BASE; i++)
	(*this)[i] = a2[i];
      return *this;
    }

    /// array copy
    Array & operator= (const FlatArray<T> & a2)
    {
      SetSize (a2.Size());
      for (int i = BASE; i < size+BASE; i++)
	(*this)[i] = a2[i];
      return *this;
    }


  private:

    /// resize array, at least to size minsize. copy contents
    void ReSize (int minsize)
    {
      int nsize = 2 * allocsize;
      if (nsize < minsize) nsize = minsize;

      if (data)
	{
	  T * p = new T[nsize];
	
	  int mins = (nsize < size) ? nsize : size; 
	  memcpy (p, data, mins * sizeof(T));

	  if (ownmem)
	    delete [] data;
	  ownmem = 1;
	  data = p;
	}
      else
	{
	  data = new T[nsize];
	  ownmem = 1;
	}
    
      allocsize = nsize;
    }
  };



  template <class T, int S> 
  class ArrayMem : public Array<T>
  {
    using Array<T>::size;
    using Array<T>::data;
    using Array<T>::ownmem;

    // T mem[S];     // Intel C++ calls dummy constructor
    // char mem[S*sizeof(T)];
    double mem[(S*sizeof(T)+7) / 8];
  public:
    /// Generate array of logical and physical size asize
    explicit ArrayMem(int asize = 0)
      : Array<T> (S, static_cast<T*> (static_cast<void*>(&mem[0])))
    {
      size = asize;
      if (asize > S)
	{
	  data = new T[asize];
	  ownmem = 1;
	}
      // SetSize (asize);
    }

    ArrayMem & operator= (const T & val)  
    {
      Array<T>::operator= (val);
      return *this;
    }

    /// array copy
    ArrayMem & operator= (const FlatArray<T> & a2)
    {
      this->SetSize (a2.Size());
      for (int i = 0; i < size; i++)
	(*this)[i] = a2[i];
      return *this;
    }

  };





  /*
    template <class T, int B1, int B2>
    class IndirectArray
    {
    const FlatArray<T, B1> & array;
    const FlatArray<int, B2> & ia; 

    public:
    IndirectArray (const FlatArray<T,B1> & aa, const FlatArray<int, B2> & aia)
    : array(aa), ia(aia) { ; }
    int Size() const { return ia.Size(); }
    const T & operator[] (int i) const { return array[ia[i]]; }
    };
  */









  ///
  template <class T, int BASE = 0> 
  class MoveableArray 
  {
    int size;
    int allocsize;
    DynamicMem<T> data;

  public:

    MoveableArray()
    { 
      size = allocsize = 0; 
      data.SetName ("MoveableArray");
    }

    MoveableArray(int asize)
      : size(asize), allocsize(asize), data(asize)
    { ; }
  
    ~MoveableArray () { ; }

    int Size() const { return size; }

    void SetSize(int nsize)
    {
      if (nsize > allocsize) 
	{
	  data.ReAlloc (nsize);
	  allocsize = nsize;
	}
      size = nsize;
    }

    void SetAllocSize (int nallocsize)
    {
      data.ReAlloc (nallocsize);
      allocsize = nallocsize;
    }

    ///
    T & operator[] (int i)
    { return ((T*)data)[i-BASE]; }

    ///
    const T & operator[] (int i) const
    { return ((const T*)data)[i-BASE]; }

    ///
    T & Elem (int i)
    { return ((T*)data)[i-1]; }
  
    ///
    const T & Get (int i) const 
    { return ((const T*)data)[i-1]; }

    ///
    void Set (int i, const T & el)
    { ((T*)data)[i-1] = el; }

    ///
    T & Last ()
    { return ((T*)data)[size-1]; }
  
    ///
    const T & Last () const
    { return ((const T*)data)[size-1]; }
  
    ///
    int Append (const T & el)
    {
      if (size == allocsize) 
	{
	  SetAllocSize (2*allocsize+1);
	}
      ((T*)data)[size] = el;
      size++;
      return size;
    }
  
    ///
    void Delete (int i)
    {
      DeleteElement (i+1);
    }

    ///
    void DeleteElement (int i)
    {
      ((T*)data)[i-1] = ((T*)data)[size-1];
      size--;
    }
  
    ///
    void DeleteLast ()
    { size--; }

    ///
    void DeleteAll ()
    {
      size = allocsize = 0;
      data.Free();
    }

    ///
    void PrintMemInfo (ostream & ost) const
    {
      ost << Size() << " elements of size " << sizeof(T) << " = " 
	  << Size() * sizeof(T) << endl;
    }

    MoveableArray & operator= (const T & el)
    {
      for (int i = 0; i < size; i++)
	((T*)data)[i] = el;
      return *this;
    }


    MoveableArray & Copy (const MoveableArray & a2)
    {
      SetSize (a2.Size());
      for (int i = 0; i < this->size; i++)
	data[i] = a2.data[i];
      return *this;
    }

    /// array copy
    MoveableArray & operator= (const MoveableArray & a2)
    {
      return Copy(a2);
    }


    void SetName (const char * aname)
    {
      data.SetName(aname);
    }
  private:
    ///
    //MoveableArray & operator= (MoveableArray &); //???
    ///
    //MoveableArray (const MoveableArray &); //???
  };


  template <class T>
  inline ostream & operator<< (ostream & ost, MoveableArray<T> & a)
  {
    for (int i = 0; i < a.Size(); i++)
      ost << i << ": " << a[i] << endl;
    return ost;
  }



  /// bubble sort array
  template <class T>
  inline void BubbleSort (const FlatArray<T> & data)
  {
    for (int i = 0; i < data.Size(); i++)
      for (int j = i+1; j < data.Size(); j++)
	if (data[i] > data[j])
	  {
	    T hv = data[i];
	    data[i] = data[j];
	    data[j] = hv;
	  }
  }

  /// bubble sort array
  template <class T, class S>
  inline void BubbleSort (FlatArray<T> & data, FlatArray<S> & slave)
  {
    for (int i = 0; i < data.Size(); i++)
      for (int j = i+1; j < data.Size(); j++)
	if (data[i] > data[j])
	  {
	    T hv = data[i];
	    data[i] = data[j];
	    data[j] = hv;
	    
	    S hvs = slave[i];
	    slave[i] = slave[j];
	    slave[j] = hvs;
	  }
  }


  template <class T, class S>
  void QuickSortRec (FlatArray<T> & data,
		     FlatArray<S> & slave,
		     int left, int right)
  {
    int i = left;
    int j = right;
    T midval = data[(left+right)/2];
  
    do
      {
	while (data[i] < midval) i++;
	while (midval < data[j]) j--;
      
	if (i <= j)
	  {
	    Swap (data[i], data[j]);
	    Swap (slave[i], slave[j]);
	    i++; j--;
	  }
      }
    while (i <= j);
    if (left < j) QuickSortRec (data, slave, left, j);
    if (i < right) QuickSortRec (data, slave, i, right);
  }

  template <class T, class S>
  void QuickSort (FlatArray<T> & data, FlatArray<S> & slave)
  {
    QuickSortRec (data, slave, 0, data.Size()-1);
  }









  template <class T> 
  void Intersection (const FlatArray<T> & in1, const FlatArray<T> & in2, 
		     Array<T> & out)
  {
    out.SetSize(0);
    for(int i=0; i<in1.Size(); i++)
      if(in2.Contains(in1[i]))
	out.Append(in1[i]);
  }
  template <class T> 
  void Intersection (const FlatArray<T> & in1, const FlatArray<T> & in2, const FlatArray<T> & in3,
		     Array<T> & out)
  {
    out.SetSize(0);
    for(int i=0; i<in1.Size(); i++)
      if(in2.Contains(in1[i]) && in3.Contains(in1[i]))
	out.Append(in1[i]);
  }


}

#endif

