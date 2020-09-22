/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIControl.h
**		GUIControl is a base class for all GUI widgtes
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUICONTROL_H_
#define _R_GUI_GUICONTROL_H_

#include "R_GUI_GUIEventHandler.h"
#include "..\\Core\\Math\\R_Core_Rectangle.h"
#include "..\\Core\\Math\\R_Core_Vector2D.h"

#include <list>
#include <string>

namespace REDEEMER
{
	namespace GUI
	{
		class C_GUISkin;

		/*!	Various types of alignment for control
		*/
		enum EGUIControlAlignment
		{
			GUICA_UpperLeft,		///<	Aligned to parent's top or left side (default)
			GUICA_LowerRight,		///<	Aligned to parent's bottom or right side
			GUICA_Center,			///<	Aligned to the center of parent
			GUICA_Stretch			///<	Stretched to fit parent
		};

		/*!	Names for alignments
		*/
		const wchar_t* const GUIAlignmentNames[] =
		{
			L"UpperLeft",
			L"LowerRight",
			L"Center",
			L"Stretch",
		};

		/*!	Valid control types
		*/
		enum EGUIControlType
		{		
			GUICT_Control,			///<	Generic control
			GUICT_Button,			///<	A button (C_GUIButton)
			GUICT_StaticText,		///<	Static text, label (C_GUIStaticText)
			GUICT_Panel,			///<	Panel (C_GUIPanel)
			GUICT_CheckBox,			///<	CheckBox (C_GUICheckBox)
			GUICT_Image,			///<	An image (C_GUIImage)
			GUICT_System,			///<	GUI System (C_GUISystem)
			GUICT_Count				///<	Number of supported controls	
		};

		/*! Names for built-in element types
		*/
		const wchar_t* const GUIControlTypeNames[] =
		{
			L"Control",
			L"Button",
			L"StaticText",
			L"Panel",
			L"CheckBox",
			L"Image",
			L"GUISystem",
			L""
		};

		/*!
		**	\brief GUIControl is a base class for all GUI widgtes
		*/
		class C_GUIControl : public C_GUIEventHandler
		{
		public:
			/*!	Constructor
			*/
			C_GUIControl (unsigned int ID, EGUIControlType type, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions);

			/*!	Destructor
			*/
			virtual ~C_GUIControl ();

			/*!	Sets parent of this control
			*/
			void SetParent (C_GUIControl* parent);

			/*!	Returns control's parent
			*/
			C_GUIControl* GetParent ();

			/*!	Sets relative dimensions of this control
			*/
			void SetRelativeDimensions (const CORE::C_Rectangle<int>& dimensions);

			/*!	Gets relative dimensions of this control
			*/
			CORE::C_Rectangle<int> GetRelativeDimensions () const;

			/*!	Sets control's relative position, not altering width and height
			*/
			void SetRelativePosition (CORE::C_Vector2D<int>& position);

			/*!	Gets control's relative position
			*/
			CORE::C_Vector2D<int> GetRelativePosition () const;

			/*!	Sets control's width
			*/
			void SetWidth (int width);

			/*!	Gets control's width
			*/
			int GetWidth () const;

			/*!	Sets control's height
			*/
			void SetHeight (int height);

			/*!	Gets control's height
			*/
			int GetHeight () const;

			/*!	Gets control's absolute (screen space) dimensions
			*/
			CORE::C_Rectangle<int> GetAbsoluteDimensions () const;

			/*!	Gets control's absolute position
			*/
			CORE::C_Vector2D<int> GetAbsolutePosition () const;

			/*!	Returns control's clipping rectangle
			*/
			CORE::C_Rectangle<int> GetClippingRectangle () const;

			/*!	Sets, if this control should NOT be clipped to it's parent rectangle
			*/
			void SetNotClipped (bool notClipped);

			/*!	Checks, if this control should NOT be clipped to it's parent rectangle
			*/
			bool IsNotClipped () const;

			/*!	Sets maximum size allowed for this control
			*/
			void SetMaxSize (CORE::C_Vector2D<int> maxSize);

			/*!	Gets maximum size allowed for this control
			*/
			CORE::C_Vector2D<int> GetMaxSize () const;

			/*!	Sets minimum size allowed for this control
			*/
			void SetMinSize (CORE::C_Vector2D<int> minSize);

			/*!	Gets minimum size allowed for this control
			*/
			CORE::C_Vector2D<int> GetMinSize () const;

			/*!	Sets control's top alignment
			*/
			void SetTopAlignment (EGUIControlAlignment alignment);

			/*!	Gets control's top alignment
			*/
			EGUIControlAlignment GetTopAlignment () const;

			/*!	Sets control's bottom alignment
			*/
			void SetBottomAlignment (EGUIControlAlignment alignment);

			/*!	Gets control's bottom alignment
			*/
			EGUIControlAlignment GetBottomAlignment () const;

			/*!	Sets control's left alignment
			*/
			void SetLeftAlignment (EGUIControlAlignment alignment);

			/*!	Gets control's left alignment
			*/
			EGUIControlAlignment GetLeftAlignment () const;

			/*!	Sets control's right alignment
			*/
			void SetRightAlignment (EGUIControlAlignment alignment);

			/*!	Gets control's right alignment
			*/
			EGUIControlAlignment GetRightAlignment () const;

			/*!	Adds new child element
			*/
			virtual void AddChildControl (C_GUIControl* control);

			/*!	Removes child element
			*/
			virtual void RemoveChildControl (C_GUIControl* control);

			/*!	Returns all children belonging to this control
			*/
			virtual const std::list<C_GUIControl*>& GetChildrenControls () const;

			/*!	Detach element from its parent
			*/
			virtual void DetachFromParent ();

			/*!	Sets control's visibility
			*/
			void SetVisible (bool visible = true);

			/*!	Checks if control is visible
			*/
			bool IsVisible () const;

			/*!	Set receiving input focus using tab key
			*/
			void SetTabStop (bool enabled = true);

			/*!	Should control receive input when using TAB key
			*/
			bool IsTabStop () const;

			/*!	Sets control tab order, which means it's priority on getting focus when using TAB key
			*/
			void SetTabOrder (int tabOrder);

			/*!	Gets control's tab order
			*/
			int GetTabOrder () const;

			/*! Sets whether this element is a container for a group of elements which can be navigated using the tab key.
			**	Groups can be navigated using ctrl+tab, providing isTabStop is true. 
			*/
			void SetTabGroup (bool isGroup = true);

			/*! Returns true if this element is a tab group.
			*/
			bool IsTabGroup () const;

			/*! Returns the container element which holds all elements in this element's tab group.
			*/
			C_GUIControl* GetTabGroup ();

			/*!	Sets receiving focus and events
			*/
			void SetEnabled (bool enabled = true);

			/*!	Checks, if control can receive focus and events
			*/
			bool IsEnabled () const;

			/*!	Sets control's caption text
			*/
			virtual void SetText (const wchar_t* caption);

			/*!	Returns control's caption text
			*/
			virtual const std::wstring GetText () const;

			/*!	Sets control's tooltip text
			*/
			virtual void SetToolTipText (const wchar_t* caption);

			/*!	Returns control's tooltip text
			*/
			virtual const std::wstring GetToolTipText () const;

			/*!	Sets control Identification Number
			*/
			virtual void SetID (unsigned int ID);

			/*!	Gets control Identification Number
			*/
			virtual unsigned int GetID () const;

			/*!	Returns type of GUI control
			*/
			EGUIControlType GetType () const;

			/*!	Returns type name of GUI control
			*/
			virtual const wchar_t* GetTypeName () const;		

			/*!	Draws control and its children on the screen
			*/
			virtual void Render ();

			/*!	Sets skin used for drawing this control
			*/
			void SetSkin (C_GUISkin* skin);

			/*!	Returns control's skin
			*/
			C_GUISkin* GetSkin ();

			/*! Method called when the event happened
			*/
			virtual bool OnEvent (const S_GUIEvent& event);

			/*! Returns the topmost GUI element at the specific position.
			**
			**	\param point: The point at which to find a GUI control.
			**	\return The topmost GUI control at that point, or 0 if there are no candidate elements at this point.
			*/
			C_GUIControl* GetControlAtPosition (const CORE::C_Vector2D<int>& position);

			/*! Returns true if a point is within this control.
			** Controls with a shape other than a rectangle should override this method 
			*/
			virtual bool IsPointInside (const CORE::C_Vector2D<int>& point) const;

			/*! Searches elements to find the closest next element to tab to
			**	\param startOrder: The TabOrder of the current element, -1 if none
			**	\param reverse: true if searching for a lower number
			**	\param group: true if searching for a higher one
			**	\param first: element with the highest/lowest known tab order depending on search direction
			**	\param closest: the closest match, depending on tab order and direction
			**	\param includeInvisible: includes invisible elements in the search (default=false)
			**	\return true if successfully found an element, false to continue searching/fail
			*/
			bool GetNextElement (int startOrder, bool reverse, bool group, C_GUIControl*& first, C_GUIControl*& closest, bool includeInvisible = false) const;

		protected:
			/*!	Compute control's position based on parent control
			**	\param recursive If true, all children are being updated too
			*/
			void UpdateAbsolutePosition (bool recursive);

			/*!	Compute stretching factor
			*/
			void ComputeStretchingFactor (const CORE::C_Rectangle<int>& dimensions);

		protected:
			C_GUIControl*				m_ParentControl;		///<	Parent control for setting hierarchies
			std::list<C_GUIControl*>	m_Children;				///<	Children controls
			CORE::C_Rectangle<int>		m_RelativeRectangle;	///<	Rectangle holding relative dimensions
			CORE::C_Rectangle<int>		m_AbsoluteRectangle;	///<	Rectangle holding absolute dimensions
			CORE::C_Rectangle<int>		m_ClippingRectangle;	///<	Control's clipping rectangle			
			bool						m_IsNoClip;				///<	Does this element ignore its parent's clipping rectangle?
			CORE::C_Rectangle<int>		m_DesiredRectangle;		///<	Control's dimensions before constraining and clipping			
			CORE::C_Rectangle<float>	m_ScaleRectangle;		///<	Relative scale of the element inside its parent
			CORE::C_Rectangle<int>		m_LastParentRectangle;	///<	For calculating the difference when resizing parent
			CORE::C_Vector2D<int>		m_MaxSize;				///<	Maximum size allowed for this element
			CORE::C_Vector2D<int>		m_MinSize;				///<	Minimum size allowed for this element
			EGUIControlAlignment		m_TopAlignment;			///<	Control's top alignment
			EGUIControlAlignment		m_BottomAlignment;		///<	Control's bottom alignment
			EGUIControlAlignment		m_LeftAlignment;		///<	Control's left alignment
			EGUIControlAlignment		m_RightAlignment;		///<	Control's right alignment
			bool						m_IsVisible;			///<	Should this control be rendered?
			bool						m_IsTabStop;			///<	Should control receive focus when using TAB key?
			int							m_TabOrder;				///<	Order to get focus using TAB key			
			bool						m_IsTabGroup;			///<	Tab groups are containers like windows, use ctrl+tab to navigate
			bool						m_IsEnabled;			///<	Should control receive focus?
			std::wstring				m_Text;					///<	Control's caption
			std::wstring				m_ToolTipText;			///<	Text shown as tooltip
			unsigned int				m_ID;					///<	Identification number
			EGUIControlType				m_Type;					///<	Control type, like button, window, etc.
			C_GUISkin*					m_Skin;					///<	Skin used for rendering process
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUICONTROL_H_
