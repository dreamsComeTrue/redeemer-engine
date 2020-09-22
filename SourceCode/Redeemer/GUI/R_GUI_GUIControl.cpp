/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIControl.cpp
**		GUIControl is a base class for all GUI widgtes
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUIControl.h"
#include "..\\Core\\\Math\\R_Core_MathUtils.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GUIControl::C_GUIControl (unsigned int ID, EGUIControlType type, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions) :
			m_ID (ID),
			m_Type (type),
			m_ParentControl (parent),
			m_RelativeRectangle (dimensions),
			m_AbsoluteRectangle (dimensions),
			m_ClippingRectangle (dimensions),
			m_IsNoClip (false),
			m_DesiredRectangle (dimensions),
			m_MinSize (CORE::C_Vector2D<int>(1,1)),
			m_MaxSize (CORE::C_Vector2D<int>(0,0)),
			m_TopAlignment (GUICA_UpperLeft),
			m_BottomAlignment (GUICA_UpperLeft),
			m_LeftAlignment (GUICA_UpperLeft),
			m_RightAlignment (GUICA_UpperLeft),
			m_IsVisible (true),
			m_IsEnabled (true),
			m_IsTabStop (false),
			m_IsTabGroup (false),
			m_TabOrder (-1),
			m_Skin (NULL)
		{
			//	Try to add this control to it's parent
			if (parent)
			{
				parent->AddChildControl (this);
				UpdateAbsolutePosition (true);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIControl::~C_GUIControl ()
		{
			//	Delete all children
			for (std::list<C_GUIControl*>::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
			{
				REDEEMER_SAFE_DELETE ((*it));
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetParent (C_GUIControl* parent)
		{
			//	Detach from previous parent
			if (m_ParentControl)
			{
				DetachFromParent();

				//	And attach to the new one
				parent->AddChildControl (this);
				UpdateAbsolutePosition (true);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIControl* C_GUIControl::GetParent ()
		{
			return m_ParentControl;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetRelativeDimensions (const CORE::C_Rectangle<int>& dimensions)
		{
			m_DesiredRectangle = dimensions;

			//	Sets stretching
			ComputeStretchingFactor(dimensions);

			UpdateAbsolutePosition(true);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Rectangle<int> C_GUIControl::GetRelativeDimensions () const
		{
			return m_RelativeRectangle;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetRelativePosition (CORE::C_Vector2D<int>& position)
		{
			const CORE::C_Rectangle<int> rectangle (position.X, position.Y,	position.X + m_RelativeRectangle.GetWidth(), position.Y + m_RelativeRectangle.GetHeight ());

			SetRelativeDimensions (rectangle);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector2D<int> C_GUIControl::GetRelativePosition () const
		{
			return m_RelativeRectangle.UpperLeftCorner;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetWidth (int width)
		{
			CORE::C_Vector2D<int> position = GetRelativePosition ();
			const CORE::C_Rectangle<int> rectangle (position.X, position.Y,	position.X + m_RelativeRectangle.GetWidth() + width, position.Y + m_RelativeRectangle.GetHeight ());

			SetRelativeDimensions (rectangle);
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIControl::GetWidth () const
		{
			return m_RelativeRectangle.GetWidth();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetHeight (int height)
		{
			CORE::C_Vector2D<int> position = GetRelativePosition ();
			const CORE::C_Rectangle<int> rectangle (position.X, position.Y,	position.X + m_RelativeRectangle.GetWidth(), position.Y + m_RelativeRectangle.GetHeight () + height);

			SetRelativeDimensions (rectangle);
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIControl::GetHeight () const
		{
			return m_RelativeRectangle.GetHeight();
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Rectangle<int> C_GUIControl::GetAbsoluteDimensions () const
		{
			return m_AbsoluteRectangle;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector2D<int> C_GUIControl::GetAbsolutePosition () const
		{
			return m_AbsoluteRectangle.UpperLeftCorner;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Rectangle<int> C_GUIControl::GetClippingRectangle () const
		{
			return m_ClippingRectangle;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetNotClipped (bool notClipped)
		{
			m_IsNoClip = notClipped;

			UpdateAbsolutePosition(true);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIControl::IsNotClipped () const
		{
			return m_IsNoClip;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetMaxSize (CORE::C_Vector2D<int> maxSize)
		{
			m_MaxSize = maxSize;

			UpdateAbsolutePosition (true);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector2D<int> C_GUIControl::GetMaxSize () const
		{
			return m_MaxSize;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetMinSize (CORE::C_Vector2D<int> minSize)
		{
			m_MinSize = minSize;

			if (m_MinSize.Width < 1)
				m_MinSize.Width = 1;

			if (m_MinSize.Height < 1)
				m_MinSize.Height = 1;

			UpdateAbsolutePosition (true);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector2D<int> C_GUIControl::GetMinSize () const
		{
			return m_MinSize;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetTopAlignment (EGUIControlAlignment alignment)
		{
			m_TopAlignment = alignment;

			ComputeStretchingFactor(m_DesiredRectangle);
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGUIControlAlignment C_GUIControl::GetTopAlignment () const
		{
			return m_TopAlignment;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetBottomAlignment (EGUIControlAlignment alignment)
		{
			m_BottomAlignment = alignment;

			ComputeStretchingFactor(m_DesiredRectangle);
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGUIControlAlignment C_GUIControl::GetBottomAlignment () const
		{
			return m_BottomAlignment;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetLeftAlignment (EGUIControlAlignment alignment)
		{
			m_LeftAlignment = alignment;

			ComputeStretchingFactor(m_DesiredRectangle);
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGUIControlAlignment C_GUIControl::GetLeftAlignment () const
		{
			 return m_LeftAlignment;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetRightAlignment (EGUIControlAlignment alignment)
		{
			m_RightAlignment = alignment;

			ComputeStretchingFactor(m_DesiredRectangle);
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGUIControlAlignment C_GUIControl::GetRightAlignment () const
		{
			return m_RightAlignment;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::AddChildControl (C_GUIControl* control)
		{
			if (control)
			{
				// remove from old parent
				control->DetachFromParent (); 
				control->m_ParentControl = this;
				control->m_LastParentRectangle = GetAbsoluteDimensions();

				m_Children.push_back (control);

				control->UpdateAbsolutePosition (true);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::RemoveChildControl (C_GUIControl* control)
		{
			for (std::list<C_GUIControl*>::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
			{
				if ((*it) == control)
				{
					(*it)->m_ParentControl = NULL;					
					m_Children.erase (it);

					return;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::list<C_GUIControl*>& C_GUIControl::GetChildrenControls () const
		{
			return m_Children;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::DetachFromParent ()
		{
			if (m_ParentControl)
				m_ParentControl->RemoveChildControl (this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetVisible (bool visible)
		{
			m_IsVisible = visible;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIControl::IsVisible () const
		{
			return m_IsVisible;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetTabStop (bool enabled)
		{
			m_IsTabStop = enabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIControl::IsTabStop () const
		{
			return m_IsTabStop;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetTabOrder (int tabOrder)
		{
			// negative = autonumber
			if (tabOrder < 0)
			{
				m_TabOrder = 0;

				C_GUIControl *el = GetTabGroup ();

				while (m_IsTabGroup && el && el->m_ParentControl)
					el = el->m_ParentControl;

				C_GUIControl *first = 0, *closest = 0;

				if (el)
				{
					// find the highest element number
					el->GetNextElement (-1, true, m_IsTabGroup, first, closest, true);
					if (first)
					{
						m_TabOrder = first->GetTabOrder() + 1;
					}
				}

			}
			else
				m_TabOrder = tabOrder;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIControl::GetTabOrder () const
		{
			return m_TabOrder;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetTabGroup (bool isGroup)
		{
			m_IsTabGroup = isGroup;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIControl::IsTabGroup () const
		{
			return m_IsTabGroup;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIControl* C_GUIControl::GetTabGroup ()
		{
			C_GUIControl *ret = this;

			while (ret && !ret->IsTabGroup())
				ret = ret->GetParent ();

			return ret;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetEnabled (bool enabled)
		{
			m_IsEnabled = enabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIControl::IsEnabled () const
		{
			return m_IsEnabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetText (const wchar_t* caption)
		{
			m_Text = caption;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring C_GUIControl::GetText () const
		{
			return m_Text;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetToolTipText (const wchar_t* caption)
		{
			m_ToolTipText = caption;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring C_GUIControl::GetToolTipText () const
		{
			return m_ToolTipText;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetID (unsigned int ID)
		{
			m_ID = ID;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_GUIControl::GetID () const
		{
			return m_ID;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGUIControlType C_GUIControl::GetType () const
		{
			return m_Type;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const wchar_t* C_GUIControl::GetTypeName () const
		{
			return GUIControlTypeNames[m_Type];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::UpdateAbsolutePosition (bool recursive)
		{
			CORE::C_Rectangle<int> parentAbsolute (0, 0, 0, 0);
			CORE::C_Rectangle<int> parentAbsoluteClip;
			float fw = 0.0f, fh = 0.0f;

			if (m_ParentControl)
			{	
				parentAbsolute = m_ParentControl->m_AbsoluteRectangle;

				if (m_IsNoClip)
				{
					C_GUIControl* p = this;

					//	Get the topmost parent
					while (p && p->m_ParentControl)
						p = p->m_ParentControl;

					parentAbsoluteClip = p->m_ClippingRectangle;
				}
				else
					parentAbsoluteClip = m_ParentControl->m_ClippingRectangle;
			}

			const int diffx = parentAbsolute.GetWidth() - m_LastParentRectangle.GetWidth();
			const int diffy = parentAbsolute.GetHeight() - m_LastParentRectangle.GetHeight();

			if (m_LeftAlignment == GUICA_Stretch || m_RightAlignment == GUICA_Stretch)
				fw = (float)parentAbsolute.GetWidth();

			if (m_TopAlignment == GUICA_Stretch || m_BottomAlignment == GUICA_Stretch)
				fh = (float)parentAbsolute.GetHeight();

			switch (m_LeftAlignment)
			{
			case GUICA_UpperLeft:
				break;

			case GUICA_LowerRight:
				m_DesiredRectangle.UpperLeftCorner.X += diffx;
				break;

			case GUICA_Center:
				m_DesiredRectangle.UpperLeftCorner.X += diffx / 2;
				break;

			case GUICA_Stretch:
				m_DesiredRectangle.UpperLeftCorner.X = CORE::C_MathUtils::Round (m_ScaleRectangle.UpperLeftCorner.X * fw);
				break;
			}

			switch (m_RightAlignment)
			{
			case GUICA_UpperLeft:
				break;

			case GUICA_LowerRight:
				m_DesiredRectangle.LowerRightCorner.X += diffx;
				break;

			case GUICA_Center:
				m_DesiredRectangle.LowerRightCorner.X += diffx / 2;
				break;

			case GUICA_Stretch:
				m_DesiredRectangle.LowerRightCorner.X = CORE::C_MathUtils::Round (m_ScaleRectangle.LowerRightCorner.X * fw);
				break;
			}

			switch (m_TopAlignment)
			{
			case GUICA_UpperLeft:
				break;

			case GUICA_LowerRight:
				m_DesiredRectangle.UpperLeftCorner.Y += diffy;
				break;

			case GUICA_Center:
				m_DesiredRectangle.UpperLeftCorner.Y += diffy / 2;
				break;

			case GUICA_Stretch:
				m_DesiredRectangle.UpperLeftCorner.Y = CORE::C_MathUtils::Round(m_ScaleRectangle.UpperLeftCorner.Y * fh);
				break;
			}

			switch (m_BottomAlignment)
			{
			case GUICA_UpperLeft:
				break;

			case GUICA_LowerRight:
				m_DesiredRectangle.LowerRightCorner.Y += diffy;
				break;

			case GUICA_Center:
				m_DesiredRectangle.LowerRightCorner.Y += diffy / 2;
				break;

			case GUICA_Stretch:
				m_DesiredRectangle.LowerRightCorner.Y = CORE::C_MathUtils::Round(m_ScaleRectangle.LowerRightCorner.Y * fh);
				break;
			}

			m_RelativeRectangle = m_DesiredRectangle;

			const int w = m_RelativeRectangle.GetWidth();
			const int h = m_RelativeRectangle.GetHeight();

			// make sure the desired rectangle is allowed
			if (w < (int)m_MinSize.Width)
				m_RelativeRectangle.LowerRightCorner.X = m_RelativeRectangle.UpperLeftCorner.X + m_MinSize.Width;

			if (h < (int)m_MinSize.Height)
				m_RelativeRectangle.LowerRightCorner.Y = m_RelativeRectangle.UpperLeftCorner.Y + m_MinSize.Height;

			if (m_MaxSize.Width && w > (int)m_MaxSize.Width)
				m_RelativeRectangle.LowerRightCorner.X = m_RelativeRectangle.UpperLeftCorner.X + m_MaxSize.Width;

			if (m_MaxSize.Height && h > (int)m_MaxSize.Height)
				m_RelativeRectangle.LowerRightCorner.Y = m_RelativeRectangle.UpperLeftCorner.Y + m_MaxSize.Height;

			m_RelativeRectangle.Repair();

			m_AbsoluteRectangle = m_RelativeRectangle + parentAbsolute.UpperLeftCorner;

			if (!m_ParentControl)
				parentAbsoluteClip = m_AbsoluteRectangle;

			m_ClippingRectangle = m_AbsoluteRectangle;
			m_ClippingRectangle.ClipAgainst (parentAbsoluteClip);

			m_LastParentRectangle = parentAbsolute;

			// update all children
			if (recursive)
			{					
				for (std::list<C_GUIControl*>::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
				{
					(*it)->UpdateAbsolutePosition (recursive);
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::ComputeStretchingFactor (const CORE::C_Rectangle<int>& dimensions)
		{
			if (m_ParentControl)
			{
				float parentWidth = (float)m_ParentControl->GetAbsoluteDimensions().GetWidth();
				float parentHeight = (float)m_ParentControl->GetAbsoluteDimensions ().GetHeight();

				if (m_TopAlignment == GUICA_Stretch)
					m_ScaleRectangle.UpperLeftCorner.Y = (float)dimensions.UpperLeftCorner.Y / parentHeight;

				if (m_BottomAlignment == GUICA_Stretch)
					m_ScaleRectangle.LowerRightCorner.Y = (float)dimensions.LowerRightCorner.Y / parentHeight;

				if (m_LeftAlignment == GUICA_Stretch)
					m_ScaleRectangle.UpperLeftCorner.X = (float)dimensions.UpperLeftCorner.X / parentWidth;

				if (m_RightAlignment == GUICA_Stretch)
					m_ScaleRectangle.LowerRightCorner.X = (float)dimensions.LowerRightCorner.X / parentWidth;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::Render ()
		{
			//	This method actually renders nothing, it should be overriden in more specialized controls
			if (IsVisible())
			{
				for (std::list<C_GUIControl*>::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
					(*it)->Render();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIControl::GetNextElement (int startOrder, bool reverse, bool group, C_GUIControl*& first, C_GUIControl*& closest, bool includeInvisible) const
		{
			// we'll stop searching if we find this number
			int wanted = startOrder + ( reverse ? -1 : 1 );

			if (wanted == -2)
				wanted = 1073741824; // maximum s32

			std::list<C_GUIControl*>::const_iterator it = m_Children.begin();

			int closestOrder, currentOrder;

			while(it != m_Children.end())
			{
				// ignore invisible elements and their children
				if (((*it)->IsVisible() || includeInvisible ) && (group == true || (*it)->IsTabGroup() == false))
				{
					// only check tab stops and those with the same group status
					if ((*it)->IsTabStop() && ((*it)->IsTabGroup() == group))
					{
						currentOrder = (*it)->GetTabOrder();

						// is this what we're looking for?
						if (currentOrder == wanted)
						{
							closest = *it;
							return true;
						}

						// is it closer than the current closest?
						if (closest)
						{
							closestOrder = closest->GetTabOrder();

							if ((reverse && currentOrder > closestOrder && currentOrder < startOrder) || 
								(!reverse && currentOrder < closestOrder && currentOrder > startOrder))
							{
								closest = *it;
							}
						}
						else
						if ((reverse && currentOrder < startOrder) || (!reverse && currentOrder > startOrder))
						{
							closest = *it;
						}

						// is it before the current first?
						if (first)
						{
							closestOrder = first->GetTabOrder();

							if ((reverse && closestOrder < currentOrder) || (!reverse && closestOrder > currentOrder))
							{
								first = *it;
							}
						}
						else
						{
							first = *it;
						}
					}
					// search within children
					if ((*it)->GetNextElement (startOrder, reverse, group, first, closest))
					{
						return true;
					}
				}
				++it;
			}

			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIControl::SetSkin (C_GUISkin* skin)
		{
			m_Skin = skin;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUISkin* C_GUIControl::GetSkin()
		{
			return m_Skin;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIControl::OnEvent (const S_GUIEvent& event)
		{
			return m_ParentControl ? m_ParentControl->OnEvent (event) : false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIControl* C_GUIControl::GetControlAtPosition (const CORE::C_Vector2D<int>& position)
		{
			C_GUIControl* control = 0;

			// we have to search from back to front, because later children
			// might be drawn over the top of earlier ones.

			std::list<C_GUIControl*>::reverse_iterator it = m_Children.rbegin();

			if (IsVisible())
			{
				while (it != m_Children.rend ())
				{
					control = (*it)->GetControlAtPosition(position);

					if (control)
						return control;

					++it;
				}
			}

			if (IsVisible() && IsPointInside (position))
				control = this;

			return control;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIControl::IsPointInside (const CORE::C_Vector2D<int>& point) const
		{	
			return m_AbsoluteRectangle.IsPointInside (point);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
