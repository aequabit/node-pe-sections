/**
* @file    cBinaryUtil.h.
*
* Declares the binary utility class.
**/

#pragma once
#include <Windows.h>
#include <string>
#include "PEFile.h"
#include <sstream>
#include <vector>

class BinaryUtil
{
public:

	/**
	* Loads binary from disk.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @param   strPath Full pathname of the file.
	*
	* @return  Null if it fails, else the binary from disk.
	**/

	static PEFile* LoadBinaryFromDisk(
		const std::string& strPath);

	/**
	* Splits.
	*
	* @author  ToxicOverflow
	* @date    21.04.2017
	*
	* @param   target  Target for the.
	* @param   delim   The delimiter.
	*
	* @return  A std::vector&lt;std::string&gt;
	**/

	template<typename Out>
	static void split(const std::string &s, char delim, Out result)
	{
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	/**
	* Determines if this collection contains a given object.
	*
	* @author  ToxicOverflow
	* @date    21.04.2017
	*
	* @tparam  T   Generic type parameter.
	* @param [in,out]  Vec     The vector.
	* @param           Element The element.
	*
	* @return  A const bool.
	**/

	template <typename T>
	static bool Contains(std::vector<T>& Vec, const T& Element)
	{
		if (std::find(Vec.cbegin(), Vec.cend(), Element) != Vec.cend())
			return true;

		return false;
	}

	/**
	* Makes a pointer.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @tparam  T   Generic type parameter.
	* @tparam  U   Generic type parameter.
	* @tparam  V   Generic type parameter.
	* @param   pBase   The base.
	* @param   Offset  The offset.
	*
	* @return  Null if it fails, else a pointer to a T.
	**/

	template < typename T, typename U, typename V >
	static T* MakePointer(
		U pBase,
		V Offset)
	{
		return reinterpret_cast< T* >(unsigned long(pBase) + static_cast< unsigned long >(Offset));
	}

	/**
	* Gets a delta.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @tparam  T   Generic type parameter.
	* @tparam  U   Generic type parameter.
	* @tparam  V   Generic type parameter.
	* @param   first   The first.
	* @param   second  The second.
	*
	* @return  The delta.
	**/

	template < typename T, typename U, typename V >
	static T GetDelta(
		U first,
		V second)
	{
		return T(unsigned long(first) - unsigned long(second));
	}
};