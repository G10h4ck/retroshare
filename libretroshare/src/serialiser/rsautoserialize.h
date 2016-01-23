#ifndef RSAUTOSERIALIZE_H
#define RSAUTOSERIALIZE_H
/*
 * This file is part of libretroshare.
 *
 * Copyright (C) 2016 Gioacchino Mazzurco <gio@eigenlab.org>
 *
 * libretroshare is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libretroshare is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with libretroshare.  If not, see <http://www.gnu.org/licenses/>.
 */

/* DISCLAIMER!
 *
 * This file has exceptionally long lines that are ugly, this may seems a
 * violation of libretroshare coding style but is not because those lines became
 * even uglier if breaked.
 *
 * For more information about libretroshare coding style see:
 * https://github.com/RetroShare/RetroShare/wiki/Coding
 */

/**
 * @file rsautoserialize.h
 * @brief RetroShare automatic [de]serialization.
 * In RetroShare data is trasmitted between nodes in form of @see RsItem,
 * historically each discendant type of RsItem and his members had his
 * [de]serialization code written by hand, this has generated code and work
 * duplication, subtle bugs and other undesired side effects, the code in this
 * file is aimed to radically change that situation altought some compromise has
 * been necessary (@see RsSerializer) to avoid breaking retrocompatibility.
 *
 * Make sure to read documentation on this file and in @see rstlvitem.h before
 * taking any action about RetroShare [de]serialization.
 */

#include <stdint.h>
#include <string>
#include <vector>
#include <ctime>

#include "util/rsnet.h"

/**
 * @brief The RsSerializable Interface
 * This interface is supposed to be implemented by serializable classes.
 * To inherit from RsSerializable doesn't imply that the serialzation will
 * follow a specific format.
 * If you are about to implement a new serializable class you should not inherit
 * direclty from RsSerializable as more functionalities you are probably
 * interested in are already provided by more specialize classes such as
 * @see RsAutoSerializable and @see RsAutoTlvSerializable.
 */
class RsSerializable
{
public:
	/**
	 * @brief serialize this object to the given buffer
	 * @param data Chunk of memory were to dump the serialized data
	 * @param size Size of memory chunk
	 * @param offset Readed to determine at witch offset start writing data,
	 *        written to inform caller were written data ends, the updated value
	 *        is usually passed by the caller to serialize of another
	 *        RsSerializable so it can write on the same chunk of memory just
	 *        after where this RsSerializable has been serialized.
	 * @return true if serialization successed, false otherwise
	 */
	virtual bool serialize(uint8_t data[], uint32_t size,
	                       uint32_t &offset) const = 0;

	/**
	 * @brief Populate members by deserializing the given buffer
	 * @param data Readonly data to read to generate the deserialized object
	 * @param size Size of memory chunk
	 * @param offset Readed to determine at witch offset start reading data,
	 *        written to inform caller were read ended, the updated value is
	 *        usually passed by the caller to deserialize of another
	 *        RsSerializable so it can read the same chunk of memory just after
	 *        where the deserialization of this RsSerializable has ended.
	 * @return true if deserialization succeded false otherwise
	 */
	virtual bool deserialize(const uint8_t data[], uint32_t size,
	                         uint32_t &offset) = 0;

	/**
	 * @brief serializedSize calculate the size in bytes of minimum buffer
	 * needed to serialize this element
	 * @return the calculated size
	 */
	virtual uint32_t serialSize() const = 0;
};

class RsAutoSerializable;

/**
 * After long discussion we have consensuated to provide retrocompatible
 * [de]serialization of basic types also if not wrapped into an RsSerializable
 * mainly to reduce porting impact for pre-existent code and make it easier to
 * port it to the new serialization system (based on @see RsAutoSerializable),
 * for this only purpose we provide commodity non wrapping serializers.
 * This has been written for RsAutoSerializable internal usage you should not
 * implement more RsSerializer, neither you should inherit direclty from
 * RsSerializer, or use it inside your own RsSerializable.
 * If you have got here you are probably interested in what already provided by
 * @see RsAutoSerializable.
 * Only in case your new @see RsAutoSerializable has a member to [de]serialize
 * of a basic type not yet implemented you may consider to implement an
 * RsSerializer for it in this file and modify @see RsAutoSerializable
 * accordingly, but make sure before to talk extensively with other devs about
 * not it implementing as a @see RsAutoSerializable child.
 */
class RsSerializerBase
{
public:
	/**
	 * @brief Similar to @see RsSerializable::serialize(), but operate only on
	 * the single given member instead of operating on the whole object.
	 * @param data @see RsSerializable::serialize()
	 * @param size @see RsSerializable::serialize()
	 * @param offset @see RsSerializable::serialize()
	 * @param autoObj object to which the member pertain
	 * @param member member pointer to member to serialize
	 * @return @see RsSerializable::serialize()
	 */
	virtual bool serialize(uint8_t data[], uint32_t size, uint32_t &offset,
	                       const RsAutoSerializable * autoObj,
	                       const int RsAutoSerializable::* member) = 0;
	/**
	 * @brief Similar to @see RsSerializable::deserialize(), but operate only on
	 * the single given member instead of operating on the whole object.
	 * @param data @see RsSerializable::deserialize()
	 * @param size @see RsSerializable::deserialize()
	 * @param offset @see RsSerializable::deserialize()
	 * @param autoObj object to which the member pertain
	 * @param member member pointer to member to serialize
	 * @return @see RsSerializable::deserialize()
	 */
	virtual bool deserialize(const uint8_t data[], uint32_t size,
	                         uint32_t &offset, RsAutoSerializable * autoObj,
	                         int RsAutoSerializable::* member) = 0;
	/**
	 * @brief Similar to @see RsSerializable::serialSize(), but operate only on
	 * the single given member instead of operating on the whole object.
	 * @param autoObj object to which the member pertain
	 * @param member member pointer to member to serialize
	 * @return @see RsSerializable::serialSize()
	 */
	virtual uint32_t serialSize(const RsAutoSerializable * autoObj,
	                            const int RsAutoSerializable::* member) = 0;
};

/// RsSerializer base template
template<typename T> class RsSerializer : public RsSerializerBase {};

/// Templates to generate RsSerializer for standard integral types
template<typename N, uint32_t SIZE> class t_SerializerNType : public RsSerializerBase
{
public:
	bool serialize(uint8_t data[], uint32_t size, uint32_t &offset,
	               const RsAutoSerializable * autoObj,
	               const int RsAutoSerializable::* member)
	{
		if (size <= offset || size - offset < SIZE) return false;
		const N * absPtr = (const N *) &(autoObj->*member);
		N tmp = hton<N>(*absPtr);
		memcpy(data+offset, &tmp, SIZE);
		offset += SIZE;
		return true;
	}
	bool deserialize(const uint8_t data[], uint32_t size,
	                 uint32_t &offset, RsAutoSerializable * autoObj,
	                 int RsAutoSerializable::* member)
	{
		if (size <= offset || size - offset < SIZE) return false;
		N * absPtr = (N *) &(autoObj->*member);
		memcpy(absPtr, data+offset, SIZE);
		*absPtr = ntoh<N>(*absPtr);
		offset += SIZE;
		return true;
	}
	inline uint32_t serialSize(const RsAutoSerializable * /*autoObj*/,
	                           const int RsAutoSerializable::* /*member*/)
	{ return SIZE; }
};
template<> class RsSerializer<uint8_t> : public t_SerializerNType<uint8_t, 1>{};
template<> class RsSerializer<uint16_t> : public t_SerializerNType<uint16_t, 2>{};
template<> class RsSerializer<uint32_t> : public t_SerializerNType<uint32_t, 4>{};
template<> class RsSerializer<uint64_t> : public t_SerializerNType<uint64_t, 8>{};
template<> class RsSerializer<time_t> : public RsSerializer<uint64_t> {};

/// Serializer for <b>non negative</b> float
template<> class RsSerializer<float> : public RsSerializerBase
{
public:
	bool serialize(uint8_t data[], uint32_t size, uint32_t &offset,
	               const RsAutoSerializable * autoObj,
	               const int RsAutoSerializable::* member)
	{
		uint32_t sz = serialSize(autoObj, member);
		if ( !data || size <= offset ||
		     size - offset < sz )
			return false;

		const float * absPtr = (const float *) &(autoObj->*member);
		if(*absPtr < 0.0f)
		{
			std::cerr << "(EE) Cannot serialise invalid negative float value "
			          << *absPtr << " in " << __PRETTY_FUNCTION__ << std::endl;
			return false;
		}

		/* This serialisation is quite accurate. The max relative error is
		 * approximately 0.01% and most of the time less than 1e-05%, The error
		 * is well distributed over numbers also. */
		uint32_t n;
		if(*absPtr < 1e-7) n = (~(uint32_t)0);
		else n = ((uint32_t)( (1.0f/(1.0f+*absPtr) * (~(uint32_t)0))));
		n = hton<uint32_t>(n);
		memcpy(data+offset, &n, sz);
		offset += sz;
		return true;
	}
	bool deserialize(const uint8_t data[], uint32_t size,
	                 uint32_t &offset, RsAutoSerializable * autoObj,
	                 int RsAutoSerializable::* member)
	{
		uint32_t sz = serialSize(autoObj, member);
		if ( !data || size <= offset ||
		     size - offset < sz )
			return false;

		uint32_t n;
		memcpy(&n, data+offset, sz);
		n = ntoh<uint32_t>(n);
		float * absPtr = (float *) &(autoObj->*member);
		*absPtr = 1.0f/ ( n/(float)(~(uint32_t)0)) - 1.0f;
		return true;
	}
	inline uint32_t serialSize(const RsAutoSerializable * /*autoObj*/,
	                           const int RsAutoSerializable::* /*member*/)
	{ return 4; }
};

/// Serializer for std::string
template<> class RsSerializer<std::string> : public RsSerializerBase
{
public:
	bool serialize(uint8_t data[], uint32_t size, uint32_t &offset,
	               const RsAutoSerializable * autoObj,
	               const int RsAutoSerializable::* member)
	{
		if ( !data || size <= offset ||
		     size - offset < serialSize(autoObj, member) )
			return false;

		const std::string * absPtr = (const std::string *) &(autoObj->*member);
		uint32_t charsLen = absPtr->length();
		uint32_t netLen = hton<uint32_t>(charsLen);
		memcpy(data+offset, &netLen, 4); offset += 4;
		memcpy(data+offset, absPtr->c_str(), charsLen); offset += charsLen;
		return true;
	}
	bool deserialize(const uint8_t data[], uint32_t size,
	                 uint32_t &offset, RsAutoSerializable * autoObj,
	                 int RsAutoSerializable::* member)
	{
		if ( !data || size <= offset || size - offset < 4 ) return false;
		uint32_t charsLen;
		memcpy(&charsLen, data+offset, 4); offset += 4;
		charsLen = ntoh<uint32_t>(charsLen);

		if ( size <= offset || size - offset < charsLen ) return false;
		std::string * absPtr = (std::string *) &(autoObj->*member);
		absPtr->clear();
		absPtr->insert(0, (char*)data+offset, charsLen);
		offset += charsLen;
		return true;
	}
	inline uint32_t serialSize(const RsAutoSerializable * autoObj,
	                           const int RsAutoSerializable::* member)
	{
		const std::string * absPtr = (const std::string *) &(autoObj->*member);
		return absPtr->length() + 4;
	}
};

/// RsSerializer <i>wrapper</i> for RsSerializable
template<> class RsSerializer<RsSerializable> : public RsSerializerBase
{
public:
	inline bool serialize(uint8_t data[], uint32_t size, uint32_t &offset,
	               const RsAutoSerializable * autoObj,
	               const int RsAutoSerializable::* member)
	{ return reinterpret_cast<const RsSerializable * >(&(autoObj->*member))->serialize(data, size, offset); }
	inline bool deserialize(const uint8_t data[], uint32_t size,
	                 uint32_t &offset, RsAutoSerializable * autoObj,
	                 int RsAutoSerializable::* member)
	{ return reinterpret_cast<RsSerializable * >(&(autoObj->*member))->deserialize(data, size, offset); }
	inline uint32_t serialSize(const RsAutoSerializable * autoObj,
	                           const int RsAutoSerializable::* member)
	{ return reinterpret_cast<const RsSerializable * >(&(autoObj->*member))->serialSize(); }
};


/**
 * @brief Base class for RetroShare automatics [de]serializable classes
 * Provide auto-[de]serialization for members of RsSerializable discendant types
 * and for some standard C++ types.
 * Inherit from this class if you are about to implement a new serializable or
 * are renewing an existing one, retrocompatibility should not be a problem as
 * the serialized format isn't affected.
 * Use class members as building blocks of your derived RsAutoSerializable
 * discendant then register them for automatic [de]serialization in
 * serialDescription() with the help of registerSerializable(...), this way your
 * class will benefit of automatic [de]serialization without writing a single
 * line of boring code.
 * The usage of automatic [de]serialization will make the code safer as raw
 * serialized material manipulation is done just in one place, and duplication
 * on potencial security critical code is elegantly avoided making everything
 * easier to check audit.
 */
class RsAutoSerializable : public RsSerializable
{
public:
	/**
	 * @copydoc RsSerializable::serialize()
	 * This method is to be considered final, you should not override it.
	 */
	bool serialize(uint8_t data[], uint32_t size, uint32_t &offset) const
	{
		if ( !data || size <= offset || size - offset < serialSize() )
			return false;

		bool ok = true;
		const SerialLayout &sd = serialLayout();
		SerialLayout::const_iterator it;
		for(it = sd.begin(); ok && it != sd.end(); ++it)
			ok &= objectSerializer((*it).first)->serialize(data, size, offset, this, (*it).second);
		return ok;
	}
	/**
	 * @copydoc RsSerializable::deserialize()
	 * This method is to be considered final, you should not override it.
	 */
	bool deserialize(const uint8_t data[], uint32_t size, uint32_t &offset)
	{
		if ( !data || size <= offset ) return false;
		/* Can't check if there is enough space because serialSize() may return
		 * an unvalid value here as empty variable size members can be presents
		 * this is not unsafe as sub member::deserialize(...) will check it. */
		bool ok = true;
		const SerialLayout &sd = serialLayout();
		SerialLayout::const_iterator it;
		for(it = sd.begin(); ok && it != sd.end(); ++it)
			ok &= objectSerializer((*it).first)->deserialize(data, size, offset, this, (*it).second);
		return ok;
	}
	/** @copydoc RsSerializable::serialSize()
	 * This method is to be considered final, you should not override it.
	 */
	uint32_t serialSize() const
	{
		uint32_t acc = 0;
		const SerialLayout &sd = serialLayout();
		SerialLayout::const_iterator it;
		for(it = sd.begin(); it != sd.end(); ++it)
			acc += objectSerializer((*it).first)->serialSize(this, (*it).second);
		return acc;
	}

private:
	/**
	 * Used internally to keep track of type of registered for [de]serialization
	 * members.
	 * If you edit this take care of updating objectType() and
	 * objectSerializer(...).
	 */
	enum ObjectType
	{
		OBJECT_TYPE_UINT8    = 0x01, // Integral C++ base types
		OBJECT_TYPE_UINT16   = 0x02,
		OBJECT_TYPE_UINT32   = 0x03,
		OBJECT_TYPE_UINT64   = 0x04,
		OBJECT_TYPE_UFLOAT32 = 0x05, // Unsigned Float
		OBJECT_TYPE_TIME     = 0x06, // time_t
		OBJECT_TYPE_STR      = 0x20, // C++ std::string
		OBJECT_TYPE_SRZ      = 0x30  // RsSerializable and derivatives
	};

	/// Map C++ types into corresponding ObjectType
	template<class T> static ObjectType objectType();

	/// Map ObjectType into corresponding RsSerializer
	static RsSerializerBase * objectSerializer(ObjectType type)
	{
		switch(type)
		{
		case(OBJECT_TYPE_UINT8): { static RsSerializer<uint8_t> rs; return &rs; }
		case(OBJECT_TYPE_UINT16): { static RsSerializer<uint16_t> rs; return &rs; }
		case(OBJECT_TYPE_UINT32): { static RsSerializer<uint32_t> rs; return &rs; }
		case(OBJECT_TYPE_UINT64): { static RsSerializer<uint64_t> rs; return &rs; }
		case(OBJECT_TYPE_UFLOAT32): { static RsSerializer<float> rs; return &rs; }
		case(OBJECT_TYPE_TIME): { static RsSerializer<time_t> rs; return &rs; }
		case(OBJECT_TYPE_STR): { static RsSerializer<std::string> rs; return &rs; }
		case(OBJECT_TYPE_SRZ): { static RsSerializer<RsSerializable> rs; return &rs; }
		}
	}

protected:
	typedef std::pair<ObjectType, int RsAutoSerializable::*> TMPair;
	typedef std::vector<TMPair> SerialLayout;

	/**
	 * Derivative classes must use this function to register members for
	 * automatic [de]serialization in serialLayout().
	 */
	template<class A, class T> static void registerSerializable
	( T A::* member, SerialLayout &reg )
	{
		reg.push_back(
		            std::make_pair(
		                objectType<T>(),
		                reinterpret_cast<int RsAutoSerializable::*>(member)
		                )
		            );
	}

	/**
	 * @brief Get the serial structure description of this RsAutoSerializable.
	 * @return A reference to a cost vector of pairs representing the
	 *         serialization structure.
	 *
	 * @details
	 * This is expected to be implemented in an efficent manner, also by child
	 * classes. The serial structure so the description is the same for all the
	 * object of the same type so the description is calculated and stored just
	 * one time for each class and not duplicated accross all objects.
	 * Child classes should implement this function like this:
@code{.cpp}
	virtual const SerialLayout & serialLayout() const
	{
		// Use local variables for calculation so your internal operations are
		// guaranted to be thread safe, this initialization will be done every
		// time but it's cost is neglegible
		SerialLayout lReg;

		// Calculate the layout only one time, there is a remote possibility
		// that two or more threads enter this at same time but it is not a
		// problem because lReg is local so calculation would be duplicated but
		// thread safe
		static bool initialize = true;
		if (initialize)
		{
			// Make sure program don't enter this if again
			initialize = false;

			// If your class is a discendant of an autoserializable that may
			// already have a layout, inherit his layout first
			lReg = ParentRsAutoSerializable::serialLayout();

			// Register your serializable members for automatic serialization
			registerSerializable(&ThisRsAutoSerializable::member1_name, lReg);
			registerSerializable(&ThisRsAutoSerializable::member2_name, lReg);
			registerSerializable(&ThisRsAutoSerializable::member3_name, lReg);
		}

		// As this call to SerialLayout copy constructor is thread safe this
		// static initialization guarantee that this is executed really once
		static SerialLayout reg(lReg);
		return reg;
	}
@endcode
	 */
	virtual const SerialLayout & serialLayout() const = 0;
};

/* TODO: How can we move all this inside the class?
 * It woulf stay next to ObjectType declaration and it would be easier to check
 * if is defined for all enum type */
template<> inline RsAutoSerializable::ObjectType RsAutoSerializable::objectType<uint8_t>()        { return OBJECT_TYPE_UINT8; }
template<> inline RsAutoSerializable::ObjectType RsAutoSerializable::objectType<uint16_t>()       { return OBJECT_TYPE_UINT16; }
template<> inline RsAutoSerializable::ObjectType RsAutoSerializable::objectType<uint32_t>()       { return OBJECT_TYPE_UINT32; }
template<> inline RsAutoSerializable::ObjectType RsAutoSerializable::objectType<uint64_t>()       { return OBJECT_TYPE_UINT64; }
template<> inline RsAutoSerializable::ObjectType RsAutoSerializable::objectType<float>()          { return OBJECT_TYPE_UFLOAT32; }
template<> inline RsAutoSerializable::ObjectType RsAutoSerializable::objectType<time_t>()         { return OBJECT_TYPE_TIME; }
template<> inline RsAutoSerializable::ObjectType RsAutoSerializable::objectType<std::string>()    { return OBJECT_TYPE_STR; }
template<> inline RsAutoSerializable::ObjectType RsAutoSerializable::objectType<RsSerializable>() { return OBJECT_TYPE_SRZ; }

#endif // RSAUTOSERIALIZE_H
