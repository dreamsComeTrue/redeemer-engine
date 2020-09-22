/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	RedeemerEngineDefines.h
**		Various macros for engine
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _REDEEMERENGINEDEFINES_H_
#define _REDEEMERENGINEDEFINES_H_

//	AUDIO
#include "Audio\\R_Audio_AudioManager.h"
#include "Audio\\R_Audio_AudioSound.h"
#include "Audio\\R_Audio_AudioStream.h"
#include "Audio\\R_Audio_SoundBuffer.h"
#include "Audio\\R_Audio_EAX2System.h"

//	CORE
#include "Core\\R_Core_BaseClass.h"
#include "Core\\R_Core_Window.h"
#include "Core\\R_Core_VideoMode.h"
#include "Core\\R_Core_Singleton.h"
#include "Core\\R_Core_SmartPointer.h"
#include "Core\\R_Core_System.h"
#include "Core\\R_Core_Exception.h"
#include "Core\\R_Core_Convert.h"
#include "Core\\R_Core_SystemInfo.h"
#include "Core\\R_Core_Timer.h"
#include "Core\\R_Core_UUID.h"
#include "Core\\Math\\R_Core_Vector2D.h"
#include "Core\\Math\\R_Core_Vector3D.h"
#include "Core\\Math\\R_Core_Vector4D.h"
#include "Core\\Math\\R_Core_Matrix.h"
#include "Core\\Math\\R_Core_Quaternion.h"
#include "Core\\Math\\R_Core_Rectangle.h"
#include "Core\\Math\\R_Core_Plane3D.h"
#include "Core\\Math\\R_Core_AABBox3D.h"
#include "Core\\Math\\R_Core_MathUtils.h"

//	DSM
#include "DSM\\R_DSM_WorldSceneManager.h"
#include "DSM\\R_DSM_SceneManager.h"
#include "DSM\\R_DSM_SceneNode.h"
#include "DSM\\R_DSM_SceneNodeEntity.h"
#include "DSM\\R_DSM_BoxSceneNode.h"
#include "DSM\\R_DSM_PlaneSceneNode.h"
#include "DSM\\R_DSM_SkySceneNode.h"
#include "DSM\\R_DSM_SkyBoxSceneNode.h"
#include "DSM\\R_DSM_Frustum.h"
#include "DSM\\R_DSM_CameraSceneNode.h"
#include "DSM\\R_DSM_Camera3DSceneNode.h"

//	FILESYSTEM
#include "FileSystem\\R_FileSystem_Logger.h"
#include "FileSystem\\R_FileSystem_FileSystemManager.h"
#include "FileSystem\\R_FileSystem_DataFile.h"
#include "FileSystem\\R_FileSystem_PhysicalFileUNICODE.h"
#include "FileSystem\\R_FileSystem_PhysicalFileANSI.h"
#include "FileSystem\\R_FileSystem_PackFile.h"
#include "FileSystem\\R_FileSystem_CompressFile.h"
#include "FileSystem\\R_FileSystem_MemoryFile.h"
#include "FileSystem\\R_FileSystem_XMLDocument.h"

//	GUI
#include "GUI\\R_GUI_GUIManager.h"
#include "GUI\\R_GUI_GUISystem.h"
#include "GUI\\R_GUI_GUISkin.h"
#include "GUI\\R_GUI_GUISkinManager.h"
#include "GUI\\R_GUI_GUIFontManager.h"
#include "GUI\\R_GUI_GUIFont.h"
#include "GUI\\R_GUI_GUIImage.h"
#include "GUI\\R_GUI_GUIStaticText.h"
#include "GUI\\R_GUI_GUIPanel.h"
#include "GUI\\R_GUI_GUIButton.h"

//	INPUT
#include "Input\\R_Input_InputManager.h"
#include "Input\\R_Input_InputSystem.h"
#include "Input\\R_Input_Mouse.h"
#include "Input\\R_Input_Keyboard.h"
#include "Input\\R_Input_JoyStick.h"
#include "Input\\R_Input_ForceFeedback.h"
#include "Input\\R_Input_ForceFeedbackEffect.h"

//	PHYSICS
#include "Physics\\R_Physics_SceneManager.h"
#include "Physics\\R_Physics_WorldSceneManager.h"
#include "Physics\\R_Physics_Material.h"
#include "Physics\\R_Physics_CharacterController.h"
#include "Physics\\R_Physics_Actor.h"
#include "Physics\\R_Physics_Shape.h"
#include "Physics\\R_Physics_ShapeBox.h"

//	RENDER
#include "Render\\R_Render_RenderDevice.h"
#include "Render\\R_Render_Material.h"
#include "Render\\R_Render_RenderTechnique.h"
#include "Render\\R_Render_RenderPass.h"
#include "Render\\R_Render_MaterialLibrary.h"
#include "Render\\R_Render_MaterialManager.h"
#include "Render\\R_Render_Texture.h"
#include "Render\\R_Render_TextureSampler.h"
#include "Render\\R_Render_TextureManager.h"
#include "Render\\R_Render_Shader.h"
#include "Render\\R_Render_ShaderEntity.h"
#include "Render\\R_Render_ShaderVariable.h"
#include "Render\\R_Render_ShaderManager.h"
#include "Render\\R_Render_RenderManager.h"
#include "Render\\R_Render_VertexBuffer.h"
#include "Render\\R_Render_VertexFormat.h"
#include "Render\\R_Render_VertexBufferManager.h"
#include "Render\\R_Render_RenderEntity.h"
#include "Render\\R_Render_Sprite.h"
#include "Render\\R_Render_PostProcessStage.h"

//	SCRIPT
#include "Script\\R_Script_ScriptManager.h"
#include "Script\\R_Script_Script.h"


namespace REDEEMER
{
	class C_RedeemerEngine;

}	//	namespace REDEEMER

#endif	//	_REDEEMERENGINEDEFINES_H_
