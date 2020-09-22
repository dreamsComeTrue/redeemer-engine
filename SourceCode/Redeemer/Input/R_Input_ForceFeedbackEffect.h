/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_ForceFeedbackEffect.h
**		Creating various effects on input devices supporting FF
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_FORCEFEEDBACKEFFECT_H_
#define _R_INPUT_FORCEFEEDBACKEFFECT_H_

#include "R_Input_InputDevice.h"

namespace REDEEMER
{
	namespace INPUT
	{
		//	Predeclare some Effect Property structs
		class C_ForceFeedback_ForceEffect;
		class C_ForceFeedback_ConstantEffect;
		class C_ForceFeedback_RampEffect;
		class C_ForceFeedback_PeriodicEffect;
		class C_ForceFeedback_ConditionalEffect;


		/*!
		**	\brief	Creating various effects on input devices supporting FF
		*/
		class C_ForceFeedbackEffect
		{
		public:
			/*! Type of force
			*/
			enum EForceType
			{
				UnknownForce = 0,
				ConstantForce,
				RampForce,
				PeriodicForce,
				ConditionalForce,
				CustomForce
			};

			/*! Type of effect
			*/
			enum EEffectType
			{
				//Type ----- Pairs with force:
				Unknown = 0, //UnknownForce
				Constant,    //ConstantForce
				Ramp,        //RampForce
				Square,      //PeriodicForce
				Triangle,    //PeriodicForce
				Sine,        //PeriodicForce
				SawToothUp,  //PeriodicForce
				SawToothDown,//PeriodicForce
				Friction,    //ConditionalForce
				Damper,      //ConditionalForce
				Inertia,     //ConditionalForce
				Spring,      //ConditionalForce
				Custom       //CustomForce
			};

			/*! Direction of the Force
			*/
			enum EForceDirection
			{
				NorthWest,
				North,
				NorthEast,
				East,
				SouthEast,
				South,
				SouthWest,
				West
			};

			/*!	This constructor allows you to set the force type and effect.
			*/
			C_ForceFeedbackEffect (EForceType ef, EEffectType et);

			/*!	Destructor
			*/
			virtual ~C_ForceFeedbackEffect();

			/*!	Get the specific Force Effect. This should be cast depending on the EForce
			*/
			C_ForceFeedback_ForceEffect* GetForceEffect() const;

			/*!	Set the number of Axes to use before the initial creation of the effect.
			**	Can only be done prior to creation! Use the FF interface to determine how many axes can be used (are available)
			*/
			void SetAxesCount (short nAxes);

			/*!	Returns the number of axes used in this effect
			*/
			short GetAxesCount () const;

			//-------------------------------------------------------------------//
			//--- Set these variables before uploading or modifying an effect ---//

			EForceDirection				m_Direction;		///<	Direction to apply to the force - affects two axes+ effects
			short						m_TriggerButton;	///<	Number of button triggering an effect (-1 means no trigger)
			unsigned int				m_TriggerInterval;	///<	Time to wait before an effect can be re-triggered (microseconds)
			unsigned int				m_ReplayLength;		///<	Duration of an effect (microseconds)
			unsigned int				m_ReplayDelay;		///<	Time to wait before to start playing an effect (microseconds)

			const EForceType			m_Force;			///<	Force type
			const EEffectType			m_Type;				///<	Effect type

			static const unsigned int	FORCEFEEDBACK_INFINITE = 0xFFFFFFFF;	///<	Infinite Time

			//------------- Library Internal -------------------------------------//
			/**
				set internally.. do not change or you will not be able to upload/stop
				this effect any more. It will become lost. It is mutable so even
				with const reference it can/will be changed by this lib
			*/
			mutable int m_Handle;

		private:
			/*!	Hidden constructor
			*/
			C_ForceFeedbackEffect ();

		protected:
			C_ForceFeedback_ForceEffect*	m_Effect;		///<	Properties depend on EForce
			short							m_AxesCount;    ///<	Number of axes to use in effect
		};

		//-----------------------------------------------------------------------------//
		/*!	Base class of all effect property classes
		*/
		class C_ForceFeedback_ForceEffect
		{
		public:
			/*!	Destructor
			*/
			virtual ~C_ForceFeedback_ForceEffect ();
		};

		//-----------------------------------------------------------------------------//
		/*!	An optional envelope to be applied to the start/end of an effect. If any of these values are nonzero, 
		**	then the envelope will be used in setting up the effect. Not currently utilized.. But, will be soon.
		*/
		class C_ForceFeedback_Envelope : public C_ForceFeedback_ForceEffect
		{
		public:
			/*!	Constructor
			*/
			C_ForceFeedback_Envelope();

			/*!	Checks, if is it used
			*/
			bool IsUsed ();

			unsigned short m_AttackLength;	///<	Attack length
			unsigned short m_AttackLevel;	///<	Attack level
			unsigned short m_FadeLength;	///<	Fade length
			unsigned short m_FadeLevel;		///<	Fade level
		};

		//-----------------------------------------------------------------------------//
		/*!	Use this class when dealing with Force type of Constant
		*/
		class C_ForceFeedback_ConstantEffect : public C_ForceFeedback_ForceEffect
		{
		public:
			/*!	Constructor
			*/
			C_ForceFeedback_ConstantEffect ();

			C_ForceFeedback_Envelope	m_Envelope;		///<	Optional envelope
			signed short				m_Level;		///<	-10K to +10k
		};

		//-----------------------------------------------------------------------------//
		/*!	Use this class when dealing with Force type of Ramp
		*/
		class C_ForceFeedback_RampEffect : public C_ForceFeedback_ForceEffect
		{
		public:
			/*!	Constructor
			*/
			C_ForceFeedback_RampEffect ();

			C_ForceFeedback_Envelope	m_Envelope;		///<	Optional envelope
			signed short				m_StartLevel;	///<	-10K to +10k
			signed short				m_EndLevel;		///<	-10K to +10k
		};

		//-----------------------------------------------------------------------------//
		/*!	Use this class when dealing with Force type of Periodic
		*/
		class C_ForceFeedback_PeriodicEffect : public C_ForceFeedback_ForceEffect
		{
		public:
			/*!	Constructor
			*/
			C_ForceFeedback_PeriodicEffect ();

			C_ForceFeedback_Envelope	m_Envelope;		///<	Optional envelope
			unsigned short				m_Magnitude;	///<	0 to 10,0000
			signed short				m_Offset;		///<	Offset
			unsigned short				m_Phase;		///<	Position at which playback begins 0 to 35,999
			unsigned int				m_Period;		///<	Period of effect (microseconds)
		};

		//-----------------------------------------------------------------------------//
		/**
			Use this class when dealing with Force type of Conditional
		*/
		class C_ForceFeedback_ConditionalEffect : public C_ForceFeedback_ForceEffect
		{
		public:
			/*!	Constructor
			*/
			C_ForceFeedback_ConditionalEffect ();

			signed short	m_RightCoeff;		///<	-10k to +10k (Positive Coeff)
			signed short	m_LeftCoeff;		///<	-10k to +10k (Negative Coeff)											
			unsigned short	m_RightSaturation;	///<	0 to 10k (Pos Saturation)
			unsigned short	m_LeftSaturation;	///<	0 to 10k (Neg Saturation)												
			unsigned short	m_DeadBand;			///<	Region around center in which the condition is not active, in the range from 0 through 10,000
			signed short	m_Center;			///<	(Offset in DX) -10k and 10k
		};																	 

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_FORCEFEEDBACKEFFECT_H_
