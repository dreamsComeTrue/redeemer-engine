/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Random.h
**		Random number generator class
*/

#ifndef _R_CORE_RANDOM_H_
#define _R_CORE_RANDOM_H_

#include <ctime>

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Random number generator class
		*/
		class C_RandomGenerator
		{
		public:
			//------------------------------------------------------------------------------------------------------------------------
			///	Constructor
			C_RandomGenerator () :
			  m_seed (0)
			{
				//	EMPTY
			}

			//------------------------------------------------------------------------------------------------------------------------

			///	Constructor for float-types
			C_RandomGenerator (float seed)
			{
				SetSeed (seed);
			}

			//------------------------------------------------------------------------------------------------------------------------

			///	Constructor for integer-types
			C_RandomGenerator (int seed) :
				m_seed (seed)
			{
				//	EMPTY
			}

			//------------------------------------------------------------------------------------------------------------------------

			///	Destructor
			~C_RandomGenerator ()
			{
				//	EMPTY
			}

			//------------------------------------------------------------------------------------------------------------------------

			/*!	Sets seed (for integer types)
			**	\param seed new seed
			*/
			void SetSeed (int seed)
			{
				m_seed = seed;
			}

			//------------------------------------------------------------------------------------------------------------------------

			/*!	Sets seed (for float types)
			**	\param seed new seed
			*/
			void SetSeed (float seed)
			{
				m_seed = *((int *)&seed);
			}

			//------------------------------------------------------------------------------------------------------------------------

			/*!	Generate random float value in 0..1 range
			**	\return new random value
			*/
			float GenerateOneToZero ()
			{
				m_seed = 1664525L * m_seed + 1013904223L;

				int iSixteenSixteen = (((m_seed >> 10)^(m_seed << 10))& 0x7fff) << 1;
				float fResult = (((float)(iSixteenSixteen & 0xffff))/65536.0f);

				fResult += (float)( iSixteenSixteen >> 16 );

				return fResult;
			}

			//------------------------------------------------------------------------------------------------------------------------

			/*!	Generate random number from specific range (float output)
			**	\return new random value
			*/
			float GenerateFromRange (float min, float max)
			{
				return (min + ((max - min) * GenerateOneToZero ()));
			}

			//------------------------------------------------------------------------------------------------------------------------

			/*!	Generate random number from specific range (integer output)
			**	\return new random value
			*/
			int GenerateFromRange (int min, int max)
			{
				return (int(GenerateFromRange ((float)min, (float)max)));
			}

			//------------------------------------------------------------------------------------------------------------------------

		private:
			int m_seed;
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_RANDOM_H_
