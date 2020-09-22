/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_SceneNode.cpp
**		Scene nodes represents entries in scene graph, they contain transformations and are attached to render entities
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_DSM_SceneNode.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DSMSceneNode::C_DSMSceneNode () :
			m_IsEnabled (true),
			m_IsChanged (true),
			m_Name (L""),
			m_SceneManager (NULL),
			m_ParentNode (NULL),
			m_Position (CORE::C_Vector3D<float>::VECTOR_ZERO_VECTOR),
			m_Scale (CORE::C_Vector3D<float>::VECTOR_ONE_VECTOR)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneNode::C_DSMSceneNode (const std::wstring& name, C_DSMSceneManager* sceneManager)  :
			m_IsEnabled (true),
			m_IsChanged (true),
			m_Name (name),
			m_SceneManager (sceneManager),
			m_ParentNode (NULL),
			m_Position (CORE::C_Vector3D<float>::VECTOR_ZERO_VECTOR),
			m_Scale (CORE::C_Vector3D<float>::VECTOR_ONE_VECTOR)
		{
			m_Children.clear();			 
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneNode::~C_DSMSceneNode ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DSMSceneNode::Initialize ()
		{
			C_BaseClass::Initialize();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DSMSceneNode::Finalize ()
		{
			//	Remove all children nodes
			for (std::map<std::wstring, C_DSMSceneNode*>::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
			{
				REDEEMER_SAFE_DELETE (it->second);
			}

			m_Children.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::SetEnabled (bool enabled)
		{
			m_IsEnabled = enabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DSMSceneNode::IsEnabled () const
		{
			if(!m_IsEnabled)
				return false;
												
			if (m_ParentNode)
				return m_ParentNode->IsEnabled ();
			else
				return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::Update ()
		{
			if (!m_IsEnabled)
				return;

			//	Update transformations
			if (m_ParentNode)
			{
				if (m_ParentNode->IsChanged())
					m_IsChanged = true;
			}

			if (m_IsChanged)
			{
				CORE::C_Matrix<float> matParent;

				if (m_ParentNode)
					matParent = m_ParentNode->GetTransformationMatrix();

				CORE::C_Matrix<float> matTrans;
				CORE::C_Matrix<float> matScale;
				CORE::C_Matrix<float> matRot;

				matTrans.SetTranslation(m_Position);
				matRot = m_Rotation.GetMatrix();
				matScale.SetScale(m_Scale);

				// Combine transformations
				matScale *= matRot;
				matScale *= matTrans;

				m_TransformationMatrix = (matScale * matParent);
			}

			BeforeUpdate();

			//	Updates all children too
			if (!m_Children.empty())
			{
				for (std::map<std::wstring, C_DSMSceneNode*>::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
					it->second->Update();
			}	

			m_IsChanged = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::BeforeUpdate()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::SetName (const std::wstring& name)
		{
			m_Name = name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring& C_DSMSceneNode::GetName ()
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::SetParentNode (C_DSMSceneNode* node)
		{
			m_ParentNode = node;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneNode* C_DSMSceneNode::GetParentNode ()
		{
			return m_ParentNode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::AddChildNode (C_DSMSceneNode* node)
		{
			if (!node)
				return;

			std::wstring name = node->GetName();

			// The node already exists, so return
			if (m_Children.find (name) != m_Children.end())
			{
				REDEEMER_LOG << LOG_WARNING << "DSMSceneNode: Node: '" << name << "' already exists!" << LOG_ENDMESSAGE;

				return;
			}

			m_Children [name] = node;

			node->SetParentNode(this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::AddChildNode (const std::wstring& name, C_DSMSceneNode* node)
		{
			// The node already exists, so return
			if (m_Children.find (name) != m_Children.end())
			{
				REDEEMER_LOG << LOG_WARNING << "DSMSceneNode: Node: '" << name << "' already exists!" << LOG_ENDMESSAGE;

				return;
			}

			m_Children [name] = node;

			node->SetParentNode(this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::RemoveChildNode (C_DSMSceneNode* node)
		{
			if (!node)
				return;

			for (std::map<std::wstring, C_DSMSceneNode*>::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
			{
				if (it->second == node)
				{
					REDEEMER_SAFE_DELETE (it->second);
					m_Children.erase(it);

					break;
				}
			}

			REDEEMER_LOG << LOG_WARNING << "DSMSceneNode: There is no children node with name: '" << node->GetName() << LOG_ENDMESSAGE;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::RemoveChildNode (const std::wstring& name)
		{
			// Material does not exist, so exit
			if (m_Children.find (name) == m_Children.end())
			{
				REDEEMER_LOG << LOG_WARNING << "DSMSceneNode: There is no children node with name: '" << name << LOG_ENDMESSAGE;

				return;
			}

			REDEEMER_SAFE_DELETE (m_Children [name]);
			m_Children.erase (name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::RemoveChildNode (unsigned int index)
		{
			unsigned int counter = 0;

			for (std::map<std::wstring, C_DSMSceneNode*>::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
			{
				if (counter == index)
				{
					REDEEMER_SAFE_DELETE (it->second);
					m_Children.erase(it);

					break;
				}
			}

			REDEEMER_LOG << LOG_WARNING << "DSMSceneNode: There is no children node with index: '" << index << LOG_ENDMESSAGE;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneNode* C_DSMSceneNode::GetChildNode (const std::wstring& name)
		{
			if (m_Children.find (name) == m_Children.end())
			{
				REDEEMER_LOG << LOG_WARNING << "DSMSceneNode: There is no children node with name: '" << name << LOG_ENDMESSAGE;

				return NULL;
			}

			return m_Children[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneNode* C_DSMSceneNode::GetChildNode (unsigned int index)
		{
			unsigned int counter = 0;

			for (std::map<std::wstring, C_DSMSceneNode*>::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
			{
				if (counter == index)
					return (it->second);
			}

			REDEEMER_LOG << LOG_WARNING << "DSMSceneNode: There is no children node with index: '" << index << LOG_ENDMESSAGE;

			return NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_DSMSceneNode::GetChildrenNodesCount () const
		{
			return m_Children.size(); 
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::SetSceneManager (C_DSMSceneManager* sceneManager)
		{
			m_SceneManager = sceneManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneManager* C_DSMSceneNode::GetSceneManager ()
		{
			return m_SceneManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::SetPosition (const CORE::C_Vector3D<float>& position)
		{
			m_Position = position;
			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float>& C_DSMSceneNode::GetPositionLocal ()
		{
			return m_Position;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_DSMSceneNode::GetPositionWorld ()
		{
			return CORE::C_Vector3D<float>(m_TransformationMatrix._m41, m_TransformationMatrix._m42, m_TransformationMatrix._m43);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::SetScale (const CORE::C_Vector3D<float>& scale)
		{
			m_Scale = scale;
			m_IsChanged = true;			
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float>& C_DSMSceneNode::GetScale ()
		{
			return m_Scale;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::SetRotation (const CORE::C_Vector3D<float>& rotation)
		{
			m_Rotation = rotation;

			CORE::C_Matrix<float>	matRot;
			matRot.RotateYawPitchRoll (rotation);

			m_Rotation.FromMatrix (matRot);
			m_Rotation.Normalize();

			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::SetRotation (const CORE::C_Vector3D<float>& axis, const float angle)
		{
			m_Rotation.FromAngleAxis (angle, axis);
			m_Rotation.Normalize();

			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNode::SetRotation (const CORE::C_Quaternion& quat)
		{
			m_Rotation = quat;
			m_Rotation.Normalize();

			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Quaternion& C_DSMSceneNode::GetRotationLocal ()
		{
			return m_Rotation;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Quaternion C_DSMSceneNode::GetRotationWorld ()
		{
			CORE::C_Quaternion quatParent;

			if (m_ParentNode)
				quatParent = m_ParentNode->GetRotationLocal();

			return (GetRotationLocal() * quatParent);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Matrix<float>& C_DSMSceneNode::GetTransformationMatrix ()
		{
			return m_TransformationMatrix;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DSMSceneNode::IsChanged () const
		{
			return m_IsChanged;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DSM
}	//	namespace REDEEMER
