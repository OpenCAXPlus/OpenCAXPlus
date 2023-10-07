// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TDocStd_SequenceNodeOfSequenceOfApplicationDelta_HeaderFile
#define _TDocStd_SequenceNodeOfSequenceOfApplicationDelta_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_TDocStd_SequenceNodeOfSequenceOfApplicationDelta.hxx>

#include <Handle_TDocStd_ApplicationDelta.hxx>
#include <TCollection_SeqNode.hxx>
#include <TCollection_SeqNodePtr.hxx>
class TDocStd_ApplicationDelta;
class TDocStd_SequenceOfApplicationDelta;



class TDocStd_SequenceNodeOfSequenceOfApplicationDelta : public TCollection_SeqNode
{

public:

  
    TDocStd_SequenceNodeOfSequenceOfApplicationDelta(const Handle(TDocStd_ApplicationDelta)& I, const TCollection_SeqNodePtr& n, const TCollection_SeqNodePtr& p);
  
      Handle(TDocStd_ApplicationDelta)& Value()  const;




  DEFINE_STANDARD_RTTI(TDocStd_SequenceNodeOfSequenceOfApplicationDelta)

protected:




private: 


  Handle(TDocStd_ApplicationDelta) myValue;


};

#define SeqItem Handle(TDocStd_ApplicationDelta)
#define SeqItem_hxx <TDocStd_ApplicationDelta.hxx>
#define TCollection_SequenceNode TDocStd_SequenceNodeOfSequenceOfApplicationDelta
#define TCollection_SequenceNode_hxx <TDocStd_SequenceNodeOfSequenceOfApplicationDelta.hxx>
#define Handle_TCollection_SequenceNode Handle_TDocStd_SequenceNodeOfSequenceOfApplicationDelta
#define TCollection_SequenceNode_Type_() TDocStd_SequenceNodeOfSequenceOfApplicationDelta_Type_()
#define TCollection_Sequence TDocStd_SequenceOfApplicationDelta
#define TCollection_Sequence_hxx <TDocStd_SequenceOfApplicationDelta.hxx>

#include <TCollection_SequenceNode.lxx>

#undef SeqItem
#undef SeqItem_hxx
#undef TCollection_SequenceNode
#undef TCollection_SequenceNode_hxx
#undef Handle_TCollection_SequenceNode
#undef TCollection_SequenceNode_Type_
#undef TCollection_Sequence
#undef TCollection_Sequence_hxx




#endif // _TDocStd_SequenceNodeOfSequenceOfApplicationDelta_HeaderFile
