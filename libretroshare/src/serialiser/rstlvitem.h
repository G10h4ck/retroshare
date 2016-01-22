#pragma once
/*
 * libretroshare/src/serialiser: rstlvitem.h
 *
 * RetroShare Serialiser.
 *
 * Copyright 2007-2008 by Robert Fernie, Chris Parker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

/**
 * @deprecated 2016/01/22 TLV [de]serialization is deprecated
 * @see RsAutoSerializable in @see rsautoserialize.h instead.
 *
 * @file rsautotlvitem.h
 * @brief RetroShare automatic TLV [de]serialization.
 * When an object is serialized all type information handled by the language is
 * lost, this cause that on deserialization the code have to simply trust the
 * data and try convert it to the expected object, if everything go as expected
 * we obtain the "same" object that the other RetroShare instance sent to us,
 * but the assumption that everything goes as expected depends on the fact that
 * the two istances uses the same code and that the software has no bugs which
 * is usually not true.
 *
 * A serialized TLV would look like this:
 * -----------------------------
 * |      (T) Type 2 bytes     |
 * |---------------------------|
 * |     (L) Lenght 4 bytes    |
 * |---------------------------|
 * |     (V) Value L bytes     |
 * |                           |
 * |                           |
 * -----------------------------
 *
 * A serialized RsItem containing two TLV (let's call them TLV1 and TLV2 for
 * simplicity) members would look like:
 * -----------------------------
 * |    RsItem type 4 bytes    |
 * |---------------------------|
 * |   RsItem Lenght 4 bytes   |
 * |---------------------------|
 * |        T1  2 bytes        |
 * |---------------------------|
 * |        L1  4 bytes        |
 * |---------------------------|
 * |                           |
 * |        V1 L1 bytes        |
 * |                           |
 * |---------------------------|
 * |        T2  2 bytes        |
 * |---------------------------|
 * |        L2  4 bytes1       |
 * |---------------------------|
 * |                           |
 * |        V2 L2 bytes        |
 * |                           |
 * -----------------------------
 */

#include <iosfwd>

#include "serialiser/rstlvbase.h"
#include "serialiser/rsautoserialize.h"

/**
 * @deprecated @see RsAutoSerializable instead
 *
 * @brief A base class for Type Length Value items
 * If you are about to implement a new serializable you should <b>not</b>
 * RsTlvItem as this class is kept just for retrocompatibility purpose and
 * should be not used anymore @see RsAutoSerializable instead.
 */
class RsTlvItem : public RsSerializable
{
public:
	RsTlvItem() {}
	virtual ~RsTlvItem() {}
	virtual uint32_t TlvSize() const = 0;

	/**
	 * @brief Clear members and delete them if dinamically allocated.
	 */
	virtual void TlvClear() = 0;

	/**
	 * @brief Clear members without deleting them if dinamically allocated.
	 * Should be overriden by derived classes.
	 * If not overridden call TlvClear().
	 */
	virtual	void TlvShallowClear() { TlvClear(); }

	virtual bool GetTlv(void *data, uint32_t size, uint32_t *offset) = 0;
	virtual bool SetTlv(void *data, uint32_t size, uint32_t *offset) const  = 0;
	virtual std::ostream &print(std::ostream &out, uint16_t indent) const = 0;
	std::ostream &printBase(std::ostream &out, std::string clsName, uint16_t indent) const;
	std::ostream &printEnd(std::ostream &out, std::string clsName, uint16_t indent) const;

	/// Forward compatibility with @see RsSerializable
	virtual uint32_t serialSize() const { return TlvSize(); }
	virtual bool serialize(uint8_t data[], uint32_t size, uint32_t &offset) const
	{ return SetTlv((void *)data, size, &offset); }
	virtual bool deserialize(const uint8_t data[], uint32_t size, uint32_t &offset)
	{ return GetTlv((void *) data, size, &offset); }
};

std::ostream &printIndent(std::ostream &out, uint16_t indent);
