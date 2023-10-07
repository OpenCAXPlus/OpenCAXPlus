// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TNaming_ListOfNamedShape_HeaderFile
#define _TNaming_ListOfNamedShape_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Address.hxx>
#include <Handle_TNaming_NamedShape.hxx>
#include <Handle_TNaming_ListNodeOfListOfNamedShape.hxx>
#include <Standard_Integer.hxx>
#include <Standard_Boolean.hxx>
class Standard_NoSuchObject;
class TNaming_ListIteratorOfListOfNamedShape;
class TNaming_NamedShape;
class TNaming_ListNodeOfListOfNamedShape;



class TNaming_ListOfNamedShape 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT TNaming_ListOfNamedShape();
  
  Standard_EXPORT TNaming_ListOfNamedShape(const TNaming_ListOfNamedShape& Other);
  
  Standard_EXPORT   void Assign (const TNaming_ListOfNamedShape& Other) ;
  void operator= (const TNaming_ListOfNamedShape& Other) 
{
  Assign(Other);
}
  
  Standard_EXPORT   Standard_Integer Extent()  const;
  
  Standard_EXPORT   void Clear() ;
~TNaming_ListOfNamedShape()
{
  Clear();
}
  
      Standard_Boolean IsEmpty()  const;
  
  Standard_EXPORT   void Prepend (const Handle(TNaming_NamedShape)& I) ;
  
  Standard_EXPORT   void Prepend (const Handle(TNaming_NamedShape)& I, TNaming_ListIteratorOfListOfNamedShape& theIt) ;
  
  Standard_EXPORT   void Prepend (TNaming_ListOfNamedShape& Other) ;
  
  Standard_EXPORT   void Append (const Handle(TNaming_NamedShape)& I) ;
  
  Standard_EXPORT   void Append (const Handle(TNaming_NamedShape)& I, TNaming_ListIteratorOfListOfNamedShape& theIt) ;
  
  Standard_EXPORT   void Append (TNaming_ListOfNamedShape& Other) ;
  
  Standard_EXPORT   Handle(TNaming_NamedShape)& First()  const;
  
  Standard_EXPORT   Handle(TNaming_NamedShape)& Last()  const;
  
  Standard_EXPORT   void RemoveFirst() ;
  
  Standard_EXPORT   void Remove (TNaming_ListIteratorOfListOfNamedShape& It) ;
  
  Standard_EXPORT   void InsertBefore (const Handle(TNaming_NamedShape)& I, TNaming_ListIteratorOfListOfNamedShape& It) ;
  
  Standard_EXPORT   void InsertBefore (TNaming_ListOfNamedShape& Other, TNaming_ListIteratorOfListOfNamedShape& It) ;
  
  Standard_EXPORT   void InsertAfter (const Handle(TNaming_NamedShape)& I, TNaming_ListIteratorOfListOfNamedShape& It) ;
  
  Standard_EXPORT   void InsertAfter (TNaming_ListOfNamedShape& Other, TNaming_ListIteratorOfListOfNamedShape& It) ;


friend class TNaming_ListIteratorOfListOfNamedShape;


protected:





private:



  Standard_Address myFirst;
  Standard_Address myLast;


};

#define Item Handle(TNaming_NamedShape)
#define Item_hxx <TNaming_NamedShape.hxx>
#define TCollection_ListNode TNaming_ListNodeOfListOfNamedShape
#define TCollection_ListNode_hxx <TNaming_ListNodeOfListOfNamedShape.hxx>
#define TCollection_ListIterator TNaming_ListIteratorOfListOfNamedShape
#define TCollection_ListIterator_hxx <TNaming_ListIteratorOfListOfNamedShape.hxx>
#define Handle_TCollection_ListNode Handle_TNaming_ListNodeOfListOfNamedShape
#define TCollection_ListNode_Type_() TNaming_ListNodeOfListOfNamedShape_Type_()
#define TCollection_List TNaming_ListOfNamedShape
#define TCollection_List_hxx <TNaming_ListOfNamedShape.hxx>

#include <TCollection_List.lxx>

#undef Item
#undef Item_hxx
#undef TCollection_ListNode
#undef TCollection_ListNode_hxx
#undef TCollection_ListIterator
#undef TCollection_ListIterator_hxx
#undef Handle_TCollection_ListNode
#undef TCollection_ListNode_Type_
#undef TCollection_List
#undef TCollection_List_hxx




#endif // _TNaming_ListOfNamedShape_HeaderFile
