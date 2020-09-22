/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_SceneNode.h
**		Scene nodes represents entries in scene graph, they contain transformations and are attached to render entities
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DSM_SCENENODE_H_
#define _R_DSM_SCENENODE_H_

#include "..\\Core\\R_Core_BaseClass.h"
#include "..\\Core\\Math\\R_Core_Vector3D.h"
#include "..\\Core\\Math\\R_Core_Quaternion.h"
#include "..\\Core\\Math\\R_Core_Matrix.h"

#include <string>
#include <map>

namespace REDEEMER
{
	namespace DSM
	{
		class C_DSMSceneManager;

		/*!	\brief	Scene nodes represents entries in scene graph, they contain transformations and are attached to render entities
		*/
		class C_DSMSceneNode : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_DSMSceneNode ();

			/*!	Constructor
			*/
			C_DSMSceneNode (const std::wstring& name, C_DSMSceneManager* sceneManager);

			/*!	Destructor
			*/
			virtual ~C_DSMSceneNode ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Sets enabled updating world
			*/
			void SetEnabled (bool enabled = true);

			/*!	Checks, if world is enabled
			*/
			bool IsEnabled () const;

			/*!	Sets node's name
			*/
			void SetName (const std::wstring& name);

			/*!	Gets node's name
			*/
			std::wstring& GetName ();

			/*!	Updates scene node data
			*/
			virtual void Update ();

			/*!	Sets parent node
			*/
			void SetParentNode (C_DSMSceneNode* node);

			/*!	Returns parent node
			*/
			C_DSMSceneNode* GetParentNode ();

			/*!	Adds new child node
			*/
			void AddChildNode (C_DSMSceneNode* node);

			/*!	Adds new child node with give name
			*/
			void AddChildNode (const std::wstring& name, C_DSMSceneNode* node);

			/*!	Removes child node
			*/
			void RemoveChildNode (C_DSMSceneNode* node);

			/*!	Removes child node with given name
			*/
			void RemoveChildNode (const std::wstring& name);

			/*!	Removes child node with given index
			*/
			void RemoveChildNode (unsigned int index);

			/*!	Gets child node with given name
			*/
			C_DSMSceneNode* GetChildNode (const std::wstring& name);

			/*!	Gets child node with given index
			*/
			C_DSMSceneNode* GetChildNode (unsigned int index);

			/*!	Returns children nodes count
			*/
			unsigned int GetChildrenNodesCount () const;

			/*!	Sets scene manager this node is attached to
			*/
			void SetSceneManager (C_DSMSceneManager* sceneManager);

			/*!	Returns scene manager this node is attached to
			*/
			C_DSMSceneManager* GetSceneManager ();

			/*!	Sets node position in 3D
			*/
			virtual void SetPosition (const CORE::C_Vector3D<float>& position);

			/*!	Returns node local position
			*/
			virtual CORE::C_Vector3D<float>& GetPositionLocal ();

			/*!	Returns node world position (local added to parent's)
			*/
			CORE::C_Vector3D<float> GetPositionWorld ();

			/*!	Sets node scale vector
			*/
			void SetScale (const CORE::C_Vector3D<float>& scale);

			/*!	Returns node scale vector
			*/
			CORE::C_Vector3D<float>& GetScale ();

			/*!	Sets node rotation vector
			*/
			virtual void SetRotation (const CORE::C_Vector3D<float>& rotation);

			/*!	Sets node rotation using axis vector and angle
			*/
			virtual void SetRotation (const CORE::C_Vector3D<float>& axis, const float angle);

			/*!	Sets node rotation using another quaternion
			*/
			virtual void SetRotation (const CORE::C_Quaternion& quat);

			/*!	Returns node local rotation quaternion
			*/
			virtual CORE::C_Quaternion& GetRotationLocal ();

			/*!	Returns node world rotation quaternion (local added to parent's)
			*/
			CORE::C_Quaternion GetRotationWorld ();

			/*!	Returns node transformation matrix (contains all transformations combined)
			*/
			CORE::C_Matrix<float>& GetTransformationMatrix ();

			/*!	Checks of node is changed
			*/
			bool IsChanged () const;

		protected:		
			/*!	Updates scene node data
			*/
			virtual void BeforeUpdate ();

		protected:
			std::wstring							m_Name;					///<	Node name
			C_DSMSceneNode*							m_ParentNode;			///<	Parent node
			C_DSMSceneManager*						m_SceneManager;			///<	Scene manager this node is attached to
			std::map<std::wstring, C_DSMSceneNode*>	m_Children;				///<	Children nodes
			bool									m_IsEnabled;			///<	Tells, if scene node is enabled
			bool									m_IsChanged;			///<	Are we set another transformations that need to be recalculated?

			CORE::C_Vector3D<float>					m_Position;				///<	Node's position
			CORE::C_Vector3D<float>					m_Scale;				///<	Node's scale
			CORE::C_Quaternion						m_Rotation;				///<	Node's orientation
			CORE::C_Matrix<float>					m_TransformationMatrix;	///<	All transformations at once
		};

	}	//	namespace DSM
}	//	namespace REDEEMER

#endif	//	_R_DSM_SCENENODE_H_
