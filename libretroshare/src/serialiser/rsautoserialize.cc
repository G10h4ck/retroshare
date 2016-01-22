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

#include "serialiser/rsautoserialize.h"
#include "serialiser/rsbaseserial.h"

template<> void processMember<SerializeBuffer, const uint8_t>(SerializeBuffer&op, const uint8_t&value, bool opt)
{
	std::cout << "processMember<SerializeBuffer, const uint8_t>" << std::endl;
	scae(op.ok, setRawUInt8(&(op.data), op.size, &(op.offset), value) || opt);
}
template<> void processMember<DeserializeBuffer, uint8_t>(DeserializeBuffer&op, uint8_t&value, bool opt)
{
	std::cout << "processMember<DeserializeBuffer, uint8_t>" << std::endl;
	scae(op.ok, getRawUInt8(&(op.data), op.size, &(op.offset), &value) || opt);
}
template<> void processMember<SerializationSize, const uint8_t>( SerializationSize&op, const uint8_t&, bool)
{
	std::cout << "processMember<SerializationSize, const uint8_t>" << std::endl;
	scae(op.ok, op.size += 1);
}

template<> void processMember<SerializeBuffer, const uint16_t>(SerializeBuffer&op, const uint16_t&value, bool opt)
{
	std::cout << "processMember<SerializeBuffer, const uint16_t>" << std::endl;
	scae(op.ok, setRawUInt16(&(op.data), op.size, &(op.offset), value) || opt);
}
template<> void processMember<DeserializeBuffer, uint16_t>(DeserializeBuffer&op, uint16_t&value, bool opt)
{
	std::cout << "processMember<DeserializeBuffer, uint16_t>" << std::endl;
	scae(op.ok, getRawUInt16(&(op.data), op.size, &(op.offset), &value) || opt);
}
template<> void processMember<SerializationSize, const uint16_t>(SerializationSize&op, const uint16_t&, bool)
{
	std::cout << "processMember<SerializationSize, const uint16_t>" << std::endl;
	scae(op.ok, op.size += 2);
}


class ExampleSerializable : public RsSerializable
{
public:
	uint8_t flip;
	uint8_t flop;
	uint16_t blip;
	uint16_t blop;

protected:
	bool process(SerializationOp &op)
	{
		/* At compile time op specific type is not known here altought it is
		 * known by the caller, this seems some kind of type erasure gone wrong.
		 * As is this cannot work.
		 */
		processMember(op, flip, false);
		processMember(op, flop, false);
		processMember(op, blip, true);
		processMember(op, blop, true);
		return op.ok;
	}
};

void test_autoserial2()
{
	ExampleSerializable e;
	e.flip = 10;
	e.blop = 33;

	uint8_t buf[100];

	SerializeBuffer sb;
	sb.data = buf;
	sb.size = 100;
	sb.offset = 0;
	sb.ok = true;

	e.serialize(sb);

	DeserializeBuffer db;
	db.data = buf;
	db.size = 100;
	db.offset = 0;
	db.ok = true;

	ExampleSerializable d;
	d.deserialize(db);

	exit(0);
}
