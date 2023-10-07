// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _MeshVS_StdMapNodeOfMapOfTwoNodes_HeaderFile
#define _MeshVS_StdMapNodeOfMapOfTwoNodes_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_MeshVS_StdMapNodeOfMapOfTwoNodes.hxx>

#include <MeshVS_TwoNodes.hxx>
#include <TCollection_MapNode.hxx>
#include <TCollection_MapNodePtr.hxx>
class MeshVS_TwoNodesHasher;
class MeshVS_MapOfTwoNodes;
class MeshVS_MapIteratorOfMapOfTwoNodes;



class MeshVS_StdMapNodeOfMapOfTwoNodes : public TCollection_MapNode
{

public:

  
    MeshVS_StdMapNodeOfMapOfTwoNodes(const MeshVS_TwoNodes& K, const TCollection_MapNodePtr& n);
  
      MeshVS_TwoNodes& Key()  const;




  DEFINE_STANDARD_RTTI(MeshVS_StdMapNodeOfMapOfTwoNodes)

protected:




private: 


  MeshVS_TwoNodes myKey;


};

#define TheKey MeshVS_TwoNodes
#define TheKey_hxx <MeshVS_TwoNodes.hxx>
#define Hasher MeshVS_TwoNodesHasher
#define Hasher_hxx <MeshVS_TwoNodesHasher.hxx>
#define TCollection_StdMapNode MeshVS_StdMapNodeOfMapOfTwoNodes
#define TCollection_StdMapNode_hxx <MeshVS_StdMapNodeOfMapOfTwoNodes.hxx>
#define TCollection_MapIterator MeshVS_MapIteratorOfMapOfTwoNodes
#define TCollection_MapIterator_hxx <MeshVS_MapIteratorOfMapOfTwoNodes.hxx>
#define Handle_TCollection_StdMapNode Handle_MeshVS_StdMapNodeOfMapOfTwoNodes
#define TCollection_StdMapNode_Type_() MeshVS_StdMapNodeOfMapOfTwoNodes_Type_()
#define TCollection_Map MeshVS_MapOfTwoNodes
#define TCollection_Map_hxx <MeshVS_MapOfTwoNodes.hxx>

#include <TCollection_StdMapNode.lxx>

#undef TheKey
#undef TheKey_hxx
#undef Hasher
#undef Hasher_hxx
#undef TCollection_StdMapNode
#undef TCollection_StdMapNode_hxx
#undef TCollection_MapIterator
#undef TCollection_MapIterator_hxx
#undef Handle_TCollection_StdMapNode
#undef TCollection_StdMapNode_Type_
#undef TCollection_Map
#undef TCollection_Map_hxx




#endif // _MeshVS_StdMapNodeOfMapOfTwoNodes_HeaderFile
