// Tools.h
// Andre Berthiaume, June 2012
//
// a list of convenient functions used all over

#ifndef _Tools
#define _Tools

#include <iostream>
#include <sstream>
#include <string>
#include "SFML\Graphics.hpp"

class Tools
{
public:
	template <class T>
	static std::string ToString(T v)
	{
		std::ostringstream o;
		if (!(o << v))
			exit(1);
		return o.str();
	};

	template <class T>
	static std::string ToString( sf::Vector2<T> v)
	{
		return "< " + ToString(v.x) + ", " + ToString(v.y) + " >";
	};

	template <class T>
	static std::string ToString( sf::Rect<T> r)
	{
		return "[ (" + ToString(r.left) + ", " + ToString(r.top) + "), <" + ToString(r.width) + ", " + ToString(r.height) + "> ]";
	};

	template <class T>
	struct NonClearingVector
	{
		std::vector<T> vector;
		unsigned int count;						// rather than using clear() (and triggering the GC) we will simply reuse the cells.
		NonClearingVector(){count = 0;}
		void insert( T t){ vector[count] = t; count++; }
	};

	// ***********************************************************************
	// Varia
	
	// Clamps a value to within min/max inclusively.
	template <class T>
	static void Clamp(T& val, T min, T max)
	{
		if ( val < min ) val = min;
		else if ( val > max ) val = max;
	}


	// ***********************************************************************
	// Vector stuff

	static float Length( sf::Vector2f a){	return sqrt(a.x*a.x + a.y*a.y); };
	static float LengthSquare( sf::Vector2f a){ return a.x*a.x + a.y*a.y; };
	static sf::Vector2f Normalize( sf::Vector2f a){ return (1/Length(a) ) * a; };
	static float Dot( sf::Vector2f a, sf::Vector2f b){ return a.x * b.x + a.y*b.y; };
	static float Cross( sf::Vector2f a, sf::Vector2f b){ return a.x*b.y - a.y*b.x; };

	// *****************************************************************************
	// The following methods we lifted from http://www.rixcode.com/Completed/RixMath.h
	
	static const float PI;
	static float RADTODEG(const float Radians) { return Radians * 180.0f / PI; }
	static float DEGTORAD(const float Degrees) { return Degrees * PI / 180.0f; }


	//Rotate a vector ( available for 2D only ) 
	template <class T>
	static void Rotate(sf::Vector2<T> &vec, float angleInDegrees){
		float temp = DEGTORAD(angleInDegrees);
		float sin1 = sin(temp);
		float cos1 = cos(temp);
		float tempx = (vec.x * cos1) - (vec.y * sin1);
		vec.y = (vec.y * cos1) + (vec.x * sin1);
		vec.x = tempx;
	}

	//Rotate a vector ( available for 2D only ) 
	template <class T>
	static sf::Vector2<T> RotateVector(sf::Vector2<T> &vec, float angleInDegrees){
		sf::Vector2f temp = vec;
		Rotate(temp, angleInDegrees);
		return temp;
	}
	
	//Calculate angle between two vectors and returns it in Radians
	template <class T>
	static float AngleBetweenR(const sf::Vector3<T> &v1, const sf::Vector3<T> &v2){
		float len1 = Length(v1);
		float len2 = Length(v2);
		if (!len1 || !len2)
			return 0;
		float angle = acosf(Dot(v1, v2) / (len1 * len2));
		if ( std::numeric_limits(angle))
			return 0;
		return angle;
	}

	template <class T>
	static float AngleBetweenR(const sf::Vector2<T> &v1, const sf::Vector2<T> &v2){
		return acos( Dot(v1, v2) );
	}
	//Calculate angle between two vectors and returns it in degrees. 
	template <class T>
	static float AngleBetweenD(const sf::Vector3<T> &v1, const sf::Vector3<T> &v2){
		return RADTODEG(AngleBetweenR(v1, v2));
	}

	template <class T>
	static float AngleBetweenD(const sf::Vector2<T> &v1, const sf::Vector2<T> &v2){
		return RADTODEG(AngleBetweenR(v1, v2));
	}

	//Calculate dot product of two 3D vectors
	template <class T>
	static float Dot(const sf::Vector3<T> &v1, const sf::Vector3<T> &v2){
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template <class T>
	static float Dot(const sf::Vector2<T> &v1, const sf::Vector2<T> &v2){
		return v1.x * v2.x + v1.y * v2.y;
	}


	// Sorts (see http://create.stephan-brumme.com/stl-sort/)
	template <typename iterator>
	static void insertSort(iterator begin, iterator end)
	{
	  iterator current = begin;
	  ++current;
	  while (current != end)
	  {
		typename iterator::value_type compare = *current;
		// shift bigger value to the right
		iterator pos = current;
		--pos;

		while (pos != begin && compare < *pos)
		{
		  iterator next = pos;
		  ++next;
		  *next = *pos;
		  pos--;
		}
		// special case: first element
		if (pos != begin || compare >= *begin)
		  ++pos;
		*pos = compare;

		++current;
	  }
	}

	// Force-wait until the key k is released
	static void WaitForRelease(sf::Keyboard::Key k);
};

#endif _Tools