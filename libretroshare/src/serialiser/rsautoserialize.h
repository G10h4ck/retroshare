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
 * Make sure to read documentation on this file before taking any action about
 * RetroShare [de]serialization.
 */

#include "serialiser/rsbaseserial.h"

/// @def Short Circuit And Equals
#define scae(acc, expr) do { acc = acc && (expr);} while (false)

class SerializationOp
{
public:
	bool ok;
};

class SerializeBuffer : public SerializationOp
{
public:
	uint8_t * data;
	uint32_t size;
	uint32_t offset;
};

class DeserializeBuffer : public SerializationOp
{
public:
	const uint8_t * data;
	uint32_t size;
	uint32_t offset;
};

class SerializationSize : public SerializationOp
{
public:
	uint32_t size;
};

template<class OP, typename T> bool processMember(OP&, T&, bool);

template<> bool processMember<SerializeBuffer, const uint8_t>(SerializeBuffer&op, const uint8_t&value, bool opt);
template<> bool processMember<DeserializeBuffer, uint8_t>(DeserializeBuffer&op, uint8_t&value, bool opt);
template<> bool processMember<SerializationSize, const uint8_t>(SerializationSize&op, const uint8_t&, bool);

template<> bool processMember<SerializeBuffer, const uint16_t>(SerializeBuffer&op, const uint16_t&value, bool opt);
template<> bool processMember<DeserializeBuffer, uint16_t>(DeserializeBuffer&op, uint16_t&value, bool opt);
template<> bool processMember<SerializationSize, const uint16_t>(SerializationSize&op, const uint16_t&, bool);


class RsSerializable
{
	bool serialize(SerializeBuffer& buf) { return process(buf); }
	bool deserialize(DeserializeBuffer& buf) { return process(buf); }
	bool serialSize(SerializationSize& buf) { return process(buf); }

	virtual bool process(SerializationOp &op) = 0;
};

class ExampleSerializable : public RsSerializable
{
	bool process(SerializationOp &op)
	{
		bool ok = true;
		scae(ok, processMember(op, flip, false));
		scae(ok, processMember(op, flop, false));
		return ok;
	}

	uint8_t flip;
	uint8_t flop;
};

#endif // RSAUTOSERIALIZE_H
