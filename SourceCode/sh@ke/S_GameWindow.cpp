/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	S_GameWindow.cpp
**		Game window class
*/

//------------------------------------------------------------------------------------------------------------------------
#include "S_GameWindow.h"
#include "..\\Redeemer\\RedeemerEngine.h"
#include "..\\Redeemer\\RedeemerEngineDefines.h"

namespace REDEEMER
{
	namespace SHAKE
	{
		// Function implementation with native calling convention
		void PrintString(std::string &str)
		{
			std::wstring out;

			CORE::C_Convert::ConvertCharsToUnicode(&out, str);

			MessageBox (0, out.c_str(), L"Skrypty :)", MB_OK | MB_ICONERROR);
		}

		//------------------------------------------------------------------------------------------------------------------------
		S_GameWindow::S_GameWindow ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		S_GameWindow::~S_GameWindow ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		int S_GameWindow::Initialize ()
		{
			CORE::S_CreateWindowInfo* cwi = GetCreateInfo();

			cwi->caption		= L"sh@ke [Powered by Redeemer (C) Engine]";
			cwi->className		= L"sh@ke";
			cwi->basicStyles	= WS_OVERLAPPEDWINDOW | WS_VISIBLE;
			cwi->extendedStyles	= 0;
			cwi->classStyles	= 0;
			cwi->width			= 800;
			cwi->height			= 600;
			cwi->x				= 0;
			cwi->y				= 0;
			cwi->brush			= C_RedeemerEngine::GetSingleton().GetSystem()->LoadBrush(12345608);
			cwi->cursor			= C_RedeemerEngine::GetSingleton().GetSystem()->LoadCursorFromResource(0);
			cwi->iconBig		= C_RedeemerEngine::GetSingleton().GetSystem()->LoadIconFromResource(101);
			cwi->iconSmall		= C_RedeemerEngine::GetSingleton().GetSystem()->LoadIconFromResource(101);
			cwi->clientRect		= true;

			//m_IsFullScreen		= true;
		  
			try
			{	
				C_RedeemerEngine::GetSingleton().GetSystem()->LaunchWindow (this);
			}
			catch (CORE::C_Excpetion& exception)
			{
				C_RedeemerEngine::GetSingleton().GetLogger()->FlushToFileWithError (exception.File(), exception.Function(), exception.Line(), exception.Description());

				return -1;
			}

			return 0;
		}


		GUI::C_GUIImage* image;
		GUI::C_GUIStaticText* staticText;
		GUI::C_GUIPanel* panel;
		GUI::C_GUIFont* font;
		GUI::C_GUIButton* button;
		DSM::C_DSMCamera3DSceneNode* camera;
		DSM::C_DSMSceneManager* sceneManager;
						 
		int posX;
		int posY;

		//------------------------------------------------------------------------------------------------------------------------
				 
		int S_GameWindow::Finalize ()
		{	
			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------
		void DrawFloorCeiling ()
		{
			DSM::C_DSMPlaneSceneNode* planeNode = sceneManager->CreatePlaneSceneNode(CORE::C_Plane3D<float> (CORE::C_Vector3D<float> (0.0f, 0.0f, 0.0f), CORE::C_Vector3D<float>::VECTOR_UNIT_Y), 400, CORE::C_Vector2D<float>(400.0f, 400.0f));

			sceneManager->GetRootNode()->AddChildNode( L"FloorPlane", planeNode);
			planeNode->GetRenderEntity(0)->SetMaterial (C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"FloorMaterial"));
			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"FloorMaterial")->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).SetAddressingState (RENDER::TAT_AddressU, RENDER::TAM_Wrap);
			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"FloorMaterial")->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).SetAddressingState (RENDER::TAT_AddressV, RENDER::TAM_Wrap);
			planeNode->SetPosition(CORE::C_Vector3D<float>(0.0f, -2.0f, 0.0f));
		}

		//------------------------------------------------------------------------------------------------------------------------

		//	TYPE
		int planeIndex = 0;

		void DrawWall (int x, int y, int type)
		{
			if (type == 0)
				return;

			DSM::C_DSMBoxSceneNode* boxNode = sceneManager->CreateBoxSceneNode(CORE::C_AABBox3D<float>(CORE::C_Vector3D<float>(-4,-4,-4), CORE::C_Vector3D<float>(4,4,4)));

			std::wostringstream stream;
			stream << L"BoxNode_" << planeIndex;

			sceneManager->GetRootNode()->AddChildNode(stream.str(), boxNode);
			boxNode->GetRenderEntity(0)->SetMaterial (C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"PlaneMaterial"));

			switch (type)
			{
			case 1:
				boxNode->SetPosition(CORE::C_Vector3D<float>((float)x * 8, 2.1f, (float)y * 8));
				break;
			}	

			planeIndex ++;
		}

		const int SZEROKOSC = 20;

		int levelBlocks[SZEROKOSC][SZEROKOSC];

		void LoadLevel ()
		{
			DrawFloorCeiling();

			FILESYSTEM::C_PhysicalFileANSI* mapa = static_cast<FILESYSTEM::C_PhysicalFileANSI*>(C_RedeemerEngine::GetSingleton ().GetFileSystemManager ()->GetSingleton ().
				GetFileManager ().OpenPhysicalFile (L"GameData\\mapa.txt", FILESYSTEM::FOM_Read, FILESYSTEM::PFT_ANSI));
						
			std::string mapaData = mapa->GetData();

			int x = 0;
			int y = 0;

			for (unsigned int i = 0; i < mapaData.length(); i ++)
			{
				if (!((mapaData[i] - 48 >= 0) && (mapaData[i] - 48 <= 4)))
						continue;

	 
				levelBlocks[x][y] = mapaData[i] - 48;

				x ++;

				if (x > SZEROKOSC - 1)
				{
					x = 0;
					y ++;
				}
			}

			for (unsigned int i = 0; i < SZEROKOSC; ++i)
				for (unsigned int j = 0; j < SZEROKOSC; ++j)
				DrawWall(i, j, levelBlocks[i][j]);
		}

		CORE::C_Timer timer;
		unsigned long counter;
		unsigned long diff;

		void S_GameWindow::OnCreate()
		{									  
			CenterWindow();

			const INPUT::C_MouseState &ms = C_RedeemerEngine::GetSingleton().GetInputManager()->GetMouse().GetMouseState();
			ms.DisplayAreaWidth = GetWidth (false);
			ms.DisplayAreaHeight = GetHeight (false);

	//		FILESYSTEM::C_PackFile* pack = static_cast<FILESYSTEM::C_PackFile*>(C_RedeemerEngine::GetSingleton ().GetFileSystemManager ().GetSingleton ().
	//			GetFileManager ().OpenPACKFile (L"GameData\\paczka.pack", /*FILESYSTEM::FILEOPENMODE_WRITE | */FILESYSTEM::FOM_Read | FILESYSTEM::FOM_Binary));

			FILESYSTEM::C_PhysicalFileANSI* plikFizyczny2 = static_cast<FILESYSTEM::C_PhysicalFileANSI*>(C_RedeemerEngine::GetSingleton ().GetFileSystemManager ()->GetSingleton ().
				GetFileManager ().OpenPhysicalFile (L"GameData\\a.wav", FILESYSTEM::FOM_Read | FILESYSTEM::FOM_Binary, FILESYSTEM::PFT_ANSI));
			
	//		FILESYSTEM::C_PhysicalFileANSI* plikFizyczny3 = static_cast<FILESYSTEM::C_PhysicalFileANSI*>(C_RedeemerEngine::GetSingleton ().GetFileSystemManager ().GetSingleton ().
	//			GetFileManager ().OpenPhysicalFile (L"GameData\\a.wav", FILESYSTEM::FOM_Read | FILESYSTEM::FOM_Binary, FILESYSTEM::PFT_ANSI));
	 
	//		FILESYSTEM::C_CompressFile* compress = pack->GetCompressFile("GameData\\a.ogg");

//			AUDIO::C_SoundBuffer* buffer = C_RedeemerEngine::GetSingleton().GetAudioManager().CreateSoundBufferFromFile(plikFizyczny2);
//			AUDIO::C_AudioSound* sound = C_RedeemerEngine::GetSingleton().GetAudioManager().CreateSoundFromBuffer(buffer);
			
//			sound->SetPosition(CORE::C_Vector3D<float> (0,0,0));
//			sound->SetMinDistance(100);
//			sound->Play ();

	//		C_RedeemerEngine::GetSingleton().GetAudioManager().GetEAX2System ().SetListenerEnvironmentPreset (AUDIO::LEP_Psychotic);

//			AUDIO::C_AudioStream* stream2 = C_RedeemerEngine::GetSingleton().GetAudioManager().CreateStreamSoundFromFile(plikFizyczny2);
  
//			stream2->SetPosition(CORE::C_Vector3D<float> (0,0,0));
///			stream2->SetMinDistance(20);
//			stream2->Play();


		//	pack->AddFile(plikFizyczny2);	
		//	pack->AddFile(plikFizyczny3);

/*			C_RedeemerEngine::GetSingleton().GetScriptManager().RegisterGlobalFunction("void print(string &in)", PrintString);

			FILESYSTEM::C_PhysicalFileANSI* skrypt = static_cast<FILESYSTEM::C_PhysicalFileANSI*>(C_RedeemerEngine::GetSingleton ().GetFileSystemManager ().GetSingleton ().
				GetFileManager ().OpenPhysicalFile (L"GameData\\skrypt.script", FILESYSTEM::FOM_Read| FILESYSTEM::FOM_Binary, FILESYSTEM::PFT_ANSI));

			SCRIPT::C_Script* script = C_RedeemerEngine::GetSingleton().GetScriptManager().CreateScript(skrypt, "void main()", "modul1");

			C_RedeemerEngine::GetSingleton().GetScriptManager().AddScript(L"script", script);
//			script->SetUpdateFunction("Update");

			C_RedeemerEngine::GetSingleton().GetScriptManager().ExecuteScript(script);
				C_RedeemerEngine::GetSingleton().GetScriptManager().SaveCompiledScriptToFile(L"script", "wynik.bcode");
*/
			FILESYSTEM::C_PhysicalFileANSI* skrypt = static_cast<FILESYSTEM::C_PhysicalFileANSI*>(C_RedeemerEngine::GetSingleton ().GetFileSystemManager ()->GetSingleton ().
				GetFileManager ().OpenPhysicalFile (L"GameData\\MaterialLibrary.library", FILESYSTEM::FOM_Read, FILESYSTEM::PFT_ANSI));

			FILESYSTEM::C_XMLDocument* xml = NULL;
			
			xml = C_RedeemerEngine::GetSingleton ().GetFileSystemManager ()->GetSingleton ().GetFileManager ().CreateXMLDocument(skrypt);

			RENDER::C_MaterialManager* manager = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->CreateMaterialManager(L"SimpleMaterialManager");
			manager->Parse(xml);

			REDEEMER_SAFE_DELETE(xml);

			sceneManager = C_RedeemerEngine::GetSingleton().GetDSMWorldSceneManager()->CreateSceneManager(L"SampleScene");

			DSM::C_DSMSkyBoxSceneNode* skyBox = sceneManager->CreateSkyBoxSceneNode(20);

			skyBox->SetSkyMaterial(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"SkyBoxMaterial"));

			sceneManager->GetRootNode()->AddChildNode(L"SkyBox", skyBox);
			sceneManager->AddNode(L"SkyBox", skyBox);
			skyBox->SetScale(CORE::C_Vector3D<float>(20, 20, 20));

			camera = sceneManager->Create3DCameraSceneNode(10.0f);
			sceneManager->GetRootNode()->AddChildNode(L"Camera", camera);
			sceneManager->SetActiveCamera(camera);
			sceneManager->AddNode(L"Camera", camera);
			camera->SetPosition(CORE::C_Vector3D<float>(10,3,10));

			FILESYSTEM::C_PhysicalFileANSI* guiDane = static_cast<FILESYSTEM::C_PhysicalFileANSI*>(C_RedeemerEngine::GetSingleton ().GetFileSystemManager ()->GetSingleton ().
				GetFileManager ().OpenPhysicalFile (L"GameData\\GUI.skin", FILESYSTEM::FOM_Read, FILESYSTEM::PFT_ANSI));

			FILESYSTEM::C_XMLDocument* xml2 = NULL;

			xml2 = C_RedeemerEngine::GetSingleton ().GetFileSystemManager ()->GetSingleton ().GetFileManager ().CreateXMLDocument(guiDane);

			GUI::C_GUISkin* skin = C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->GetSkinManager()->CreateSkinDefinition(L"skin", xml2);

			REDEEMER_SAFE_DELETE(xml2);
									
			FILESYSTEM::C_PhysicalFileANSI* guiFont = static_cast<FILESYSTEM::C_PhysicalFileANSI*>(C_RedeemerEngine::GetSingleton ().GetFileSystemManager ()->GetSingleton ().
				GetFileManager ().OpenPhysicalFile (L"GameData\\Consola.ttf", FILESYSTEM::FOM_Read | FILESYSTEM::FOM_Binary, FILESYSTEM::PFT_ANSI));

			font = C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->GetFontManager()->CreateFontDefinition(L"MyFont", guiFont, 100, false);
			font->SetPixelSize(17, 17);

			C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->GetFontManager()->SetFontMaterialTechnique(
				C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"GUIMaterial")->GetRenderTechnique(L"FontTechnique"));

			C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->SetGUIMaterial(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial (skin->GetMaterialName()));

			image = new GUI::C_GUIImage (100, C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem(), CORE::C_Rectangle<int>(0, 550, 800, 600));
			image->SetImage(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"SimpleMaterial")->GetRenderTechnique(L"main")->GetRenderPass(0)->GetTextureSampler(0).GetTexture());
			image->SetScaleImage(true);
			image->SetTabStop(true);
			image->SetTabOrder(0);

			staticText = new GUI::C_GUIStaticText (200, C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem(), CORE::C_Rectangle<int>(400, 20, 510, 210));
			staticText->SetFont(font);
			staticText->SetText(L"Redeemer Engine \n\n :)");
			staticText->SetHorizontalAlignment(GUI::GUICA_Center);
			staticText->SetVerticalAlignment(GUI::GUICA_UpperLeft);
			staticText->SetSkin(skin);
			staticText->SetWordWrapEnabled(true);
			staticText->SetTabStop(true);
			staticText->SetTabOrder(1);
		  
			panel = new GUI::C_GUIPanel (300, C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem(), CORE::C_Rectangle<int>(400, 20, 610, 230));
			panel->SetCaptionFont(font);
			panel->SetCaptionVerticalAlignment(GUI::GUICA_LowerRight);
			panel->SetText(L"Panel test :)");
			panel->SetTabStop(true);
			panel->SetTabOrder(2);

			RENDER::C_Color color = RENDER::C_Color::COLOR_YELLOW;

			color.SetAlpha(128);
			panel->SetColor(color);
			panel->SetCaptionColor(RENDER::C_Color::COLOR_RED);


			button = new GUI::C_GUIButton (400, C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem(), CORE::C_Rectangle<int>(490, 200, 610, 310));
			button->GetInternalPanelControl()->SetCaptionFont(font);
			button->SetText(L"Button test :)");
			button->SetTabStop(true);
			button->SetTabOrder(4);
			button->SetGlyphImage(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"SimpleMaterial")->GetRenderTechnique(L"main")->GetRenderPass(0)->GetTextureSampler(0).GetTexture());
			button->SetGlyphImageRendered(true);
			button->SetGlyphSpacing(5);
			button->SetGlyphAlignment(GUI::GA_Bottom);
			button->SetGlyphScalingFactor(79);

			RENDER::C_PostProcessStage* stage = static_cast<RENDER::C_PostProcessStage*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetRenderPipeline().GetRenderStage(POSTPROCESS_STAGE_NAME));
			
			stage->AddToRenderQueue (2,	C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"DiffuseMaterial"));

			posX = GetRect(false)->left;
			posY = GetRect(false)->top;

			LoadLevel ();

			timer.StartTimer();
			diff = (unsigned long)timer.GetTime();
			counter = 160;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameWindow::OnResize (unsigned width, unsigned height)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool S_GameWindow::OnMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_CREATE:
				{
					DisableIdle();

					REDEEMER_LOG << LOG_INFO << "Creating console window..." << LOG_ENDLINE << LOG_ENDMESSAGE;

					m_GameConsoleWindow = new S_GameConsoleWindow ();

					m_GameConsoleWindow->Initialize();
				}

				return false;

			case WM_DESTROY:
				{
					m_GameConsoleWindow->Finalize();
				}

				return false;

			case WM_CLOSE:
				PostQuitMessage(0);

				return false;
			};

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameWindow::OnActivate()
		{
			EnableIdle ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameWindow::OnDeactivate()
		{
			DisableIdle ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameWindow::OnIdle()
		{	
			if ((timer.GetTime() - diff) > 1000)
			{
				//font->ClearText();

				std::wstring s, s2;

				CORE::C_Convert::ConvertCharsToUnicode(&s, CORE::C_Convert::ConvertIntegerToString(counter));

				s2 = L"Czas: " + s;
				
				//font->AddText(s2, 600, 560, RENDER::C_Color::COLOR_RED);

				counter --;

				diff = (unsigned long)timer.GetTime();
			}

			INPUT::C_KeyboardDevice& keyboard =  C_RedeemerEngine::GetSingleton().GetInputManager()->GetKeyboard();

			if( keyboard.IsKeyDown(INPUT::KEY_ESCAPE ) || keyboard.IsKeyDown(INPUT::KEY_Q ) )
				PostQuitMessage(0);

			INPUT::C_MouseState state = C_RedeemerEngine::GetSingleton().GetInputManager()->GetMouse().GetMouseState();

			 C_RedeemerEngine::GetSingleton().GetInputManager()->GetMouse().SetPosition(
				 posX + C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetRenderVideoMode().ScreenResolution.Width / 2,
				 posY + C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetRenderVideoMode().ScreenResolution.Height / 2);

			 ShowMouseCursor (false);

			camera->RotateWithMouse(state.X.Rel, state.Y.Rel);

			CORE::C_Vector3D<float> position = camera->GetPositionLocal();

			int pozX = CORE::C_MathUtils::Floor(position.X + 4) / 8;
			int pozY = CORE::C_MathUtils::Floor(position.Z + 4) / 8;

			if (pozX >= 20 && pozY >= 20)
			{
				InfoMsg (L"Wygrales!!! :)", L"Koniec gry.");
				PostQuitMessage(0);
			}

			if (counter < 0)
			{
				InfoMsg (L"Przegrales!!! :'(", L"Koniec gry.");
				PostQuitMessage(0);
			}

			if( keyboard.IsKeyDown(INPUT::KEY_W) )
			{
				camera->Move (1);
			}

			if( keyboard.IsKeyDown(INPUT::KEY_S) )
			{
				camera->Move (-1);
			}

			if( keyboard.IsKeyDown(INPUT::KEY_A) )
			{
				camera->Strafe (-1);
			}

			if( keyboard.IsKeyDown(INPUT::KEY_D) )
			{
				camera->Strafe (1);
			}

			camera->SetVelocity(10.0f);

			if( keyboard.IsModifierDown(INPUT::C_KeyboardDevice::KM_Shift))
			{
				camera->SetVelocity(20.0f);
			}

			C_RedeemerEngine::GetSingleton().GetAudioManager()->Listener_Set3DPosition(camera->GetPositionLocal());
			C_RedeemerEngine::GetSingleton().GetAudioManager()->Listener_Set3DOrientation(camera->GetForwardVector(), camera->GetUpVector());

			std::wstring out;

			//CORE::C_Convert::ConvertCharsToUnicode(&out, C_RedeemerEngine::GetSingleton ().GetTimer ()->GetFPS ());
			GUI::C_GUIControl* control = C_RedeemerEngine::GetSingleton ().GetGUIManager()->GetGUISystem()->GetFocus();

//			if (control)
			CORE::C_Convert::ConvertCharsToUnicode(&out, CORE::C_Convert::ConvertIntegerToString(control->GetID()));

			SetCaption(GetCreateInfo()->caption + L" | FPS: [" + out + L"]");
		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameWindow::OnMonitorChange (const CORE::S_SystemMonitor* const monitor)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace SHAKE
}	//	namespace REDEEMER