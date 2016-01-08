#pragma once
/*
 * libretroshare/src/serialiser: rstlvitem.h
 *
 * RetroShare Serialiser.
 *
 * Copyright 2007-2008 by Robert Fernie, Chris Parker
 * Copyright (C) 2016 Gioacchino Mazzurco <gio@eigenlab.org>
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

#include <iosfwd>
#include <string>
#include <inttypes.h>

/**
 * A base class for all Type Length Value items
 * This class is provided to allow the serialisation and deserialization of
 * compund TLV items that must be (un)packed.
 */
class RsTlvItem
{
public:
	RsTlvItem() {}
	virtual ~RsTlvItem() {}
	virtual uint32_t TlvSize() const = 0;
	virtual void TlvClear() = 0;

	/**
	 * @brief TlvShallowClear should to be overriden by derived classes is meant
	 * to clear content without deleting allocated raw memory chunk.
	 * If not overridden call TlvClear().
	 */
	virtual	void TlvShallowClear();

	/**
	 * Must be implemented by derived classes
	 * @brief GetTlv deserialize should obsolete the unsafe version in future
	 * @param data readonly data to read to generate the deserialized object
	 * @param size Size of memory chunk
	 * @param offset Readed to determine at witch offset start reading data,
	 *        written to inform caller were read ended, the updated value is
	 *        usually passed by the caller to GetTlv of another RsTlvItem so it
	 *        can read the same chunk of memory just after where the
	 *        deserialization this of this RsTlvItem has ended.
	 * @return true if deserialization succeded false otherwise
	 */
	virtual bool GetTlv(const uint8_t data[], uint32_t size, uint32_t &offset);

	/**
	 * Must be implemented by derived classes
	 * @brief SetTlv serialize
	 * @param data Chunk of memory were to dump the serialized data
	 * @param size Size of memory chunk
	 * @param offset Readed to determine at witch offset start writing data,
	 *        written to inform caller were written data ends, the updated value
	 *        is usually passed by the caller to SetTlv of another RsTlvItem so
	 *        it can write on the same chunk of memory just after where this
	 *        RsTlvItem has been serialized.
	 * @return true if serialization successed, false otherwise
	 */
	virtual bool SetTlv(uint8_t data[], uint32_t size, uint32_t &offset) const;

	/**
	 * @brief GetTlvType
	 * @param data
	 * @return
	 */
	static uint16_t GetTlvType(const uint8_t data[]);

	/**
	 * @brief GetTlvSize return size of a serialized TLV
	 * @param data @see GetTlv
	 * @return TLV size parsed from TLV length
	 */
	static uint32_t GetTlvSize(const uint8_t data[]);

	/**
	 * @brief SkipTlv skip TLV element without concerning about type or value
	 * @param data @see GetTlv
	 * @param size @see GetTlv
	 * @param offset @see GetTlv
	 * @return true if skipping success false otherwise
	 */
	static bool SkipTlv(const uint8_t data[], uint32_t size, uint32_t &offset);

	static bool GetTlvString(const uint8_t data[], uint32_t size,
	                         uint32_t &offset, uint16_t type, std::string &in);

	virtual std::ostream &print(std::ostream &out, uint16_t indent) const = 0;
	std::ostream &printBase(std::ostream &out, std::string clsName,
	                        uint16_t indent) const;
	std::ostream &printEnd(std::ostream &out, std::string clsName,
	                       uint16_t indent) const;

	/// DEPRECATED: Deserialization should operate on read only data
	bool GetTlv(void *data, uint32_t size, uint32_t *offset)
	{ return GetTlv((const uint8_t *) data, size, *offset); }
	/// DEPRECATED: Use the type safer version
	bool SetTlv(void *data, uint32_t size, uint32_t *offset) const
	{ return SetTlv((uint8_t *)data, size, offset); }
	/// DEPRECATED: Deserialization should operate on read only data
	bool GetTlvString(void *data, uint32_t size, uint32_t *offset,
	                  uint16_t type, std::string &in)
	{ return GetTlvString((const uint8_t *)data, size, *offset, type, in); }
};

std::ostream &printIndent(std::ostream &out, uint16_t indent);

/**
 * @def scaq
 * @brief Short Circuit ANDeQual macro
 * Around RS code you can find a lot of <i>ok &=</i> for [de]serialization.
 * Because it is bitwise &= doesn't shortcircuit even if both arguments are bool
 * Using this macro instead will improve RS performance in case some TLV fail to
 * [de]serialize.
 * @param acc boolean accumulator likely to be <i>ok</i> in RS code
 * @param expr expression to be evaluated as second && operand
 */
#define scaq(acc, expr) do { acc = acc && expr; } while (false)
