
/*
 * libretroshare/src/serialiser: rstlvtypes.cc
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

#include "rstlvitem.h"
#include "rstlvbase.h"
#include <iostream>

void RsTlvItem::TlvShallowClear() { TlvClear(); } // unless overloaded!

std::ostream &RsTlvItem::printBase(std::ostream &out, std::string clsName,
                                   uint16_t indent) const
{
	printIndent(out, indent);
	out << "RsTlvItem: " << clsName << " Size: " << TlvSize()
	    << "  ***********************" << std::endl;
	return out;
}

std::ostream &RsTlvItem::printEnd(std::ostream &out, std::string clsName,
                                  uint16_t indent) const
{
	printIndent(out, indent);
	out << "***************** " << clsName << " ****************" << std::endl;
	return out;
}

std::ostream &printIndent(std::ostream &out, uint16_t indent)
{
	for(int i = 0; i < indent; i++) out << " ";
	return out;
}

bool RsTlvItem::GetTlv(const uint8_t data[], uint32_t size, uint32_t &offset)
{
	std::cerr << "RsTlvItem::GetTlv you should implement type safe GetTlv"
	          << "for your TLV instead of relying on deprecated non type "
	          << "safe one" << std::endl;
	return GetTlv((void *) data, size, &offset);
}

bool RsTlvItem::SetTlv(uint8_t data[], uint32_t size, uint32_t &offset) const
{
	std::cerr << "RsTlvItem::SetTlv you should implement type safe SetTlv"
	          << "for your TLV instead of relying on deprecated less type "
	          << "safe one" << std::endl;
	return SetTlv((void *)data, size, &offset);
}

uint32_t RsTlvItem::GetTlvSize(const uint8_t data[])
{
	if (!data) return 0;
	uint32_t len;
	const void * from = data + TLV_HEADER_TYPE_SIZE;
	memcpy((void *)&len, from , TLV_HEADER_LEN_SIZE);
	len = ntohl(len);
	return len;
}

uint16_t RsTlvItem::GetTlvType(const uint8_t data[])
{
	if (!data) return 0;
	uint16_t type;
	memcpy((void*)&type, data, TLV_HEADER_TYPE_SIZE);
	type = ntohs(type);
	return type;
}

bool RsTlvItem::SkipTlv(const uint8_t data[], uint32_t size,
                               uint32_t &offset)
{
	if (!data) return false;
	if (size < offset + TLV_HEADER_SIZE) return false;

	const uint8_t * tlvstart = data + offset;
	uint32_t tlvsize = GetTlvSize(tlvstart);
	uint32_t tlvend = offset + tlvsize;
	if (size < tlvend)
	{
#ifdef TLV_BASE_DEBUG
		std::cerr << "SkipUnknownTlv() FAILED - not enough space. Size: "
		          << size << " tlvsize: " << tlvsize << " tlvend: " << tlvend
		          << std::endl;
#endif
		return false;
	}

	offset = tlvend;
	return true;
}

bool RsTlvItem::GetTlvString(const uint8_t data[], uint32_t size,
                             uint32_t &offset, uint16_t type, std::string &in)
{
	if(!data && (size < offset)) return false;

	const uint8_t * tlvstart = data + offset;
	uint16_t tlvtype = GetTlvType(tlvstart);
	uint32_t tlvsize = GetTlvSize(tlvstart);
	uint32_t tlvend = offset + tlvsize;
	if ( (size < tlvend) || (size < offset) || (type != tlvtype) ) return false;

	//bool ok = data;
	//scaq(ok, )

	char *strdata = (char *) tlvstart + TLV_HEADER_SIZE;
	uint32_t strsize = tlvsize - TLV_HEADER_SIZE;
	if (strsize <= 0) in = "";
	else in = std::string(strdata, strsize);

#ifdef TLV_BASE_DEBUG
	if(type == TLV_TYPE_STR_MSG)
		std::cerr << "Checking string \"" << in << "\"" << std::endl;
#endif

	// Check for string content. We want to avoid possible lol bombs as soon as possible.

	static const int number_of_suspiscious_strings = 4 ;
	static const std::string err_in = "**** String removed (SVG bomb?) ****";
	static std::string suspiscious_strings[number_of_suspiscious_strings] =
	{ "<!e",     // base ingredient of xml bombs
	  "<!E",     // base ingredient of xml bombs
	  "PD94bW",  // this is the base64 encoding of "<?xml*"
	  "PHN2Zy"   // this is the base64 encoding of "<svg*"
	};

#ifdef TLV_BASE_DEBUG
	std::cerr << "Checking wide string \"" << in << std::endl;
#endif
	// Drop any string with "<!" or "<!"...
	// TODO: 2016/01/08 check what happens with partial messages
	for(int i=0;i<number_of_suspiscious_strings;++i)
		if (find_decoded_string(in,suspiscious_strings[i]))
		{
			std::cerr << "**** suspiscious wstring contains \""
			          << suspiscious_strings[i] << "\" (SVG bomb suspected). "
			          << "========== Original string =========" << std::endl
			          << in << std::endl
			          << "=============== END ================" << std::endl;

			/* It's important to keep the same size for in than the size it
			 * should have, otherwise the deserialization of derived items that
			 * use it might fail */
			for(uint32_t k=0;k<in.length();++k)
				if(k < err_in.length()) in[k] = err_in[k];
				else in[k] = ' ';
			break;
		}

	/* Also check that the string does not contain invalid characters.
	 * Every character less than 0x20 will be turned into a space (0x20).
	 * This is compatible with utf8, as all utf8 chars are > 0x7f.
	 * We do this for strings that should not contain some
	 * special characters by design. */

	if( type == 0 || // this is used for mGroupName and mMsgName
	        type == TLV_TYPE_STR_PEERID || type == TLV_TYPE_STR_NAME ||
	        type == TLV_TYPE_STR_PATH || type == TLV_TYPE_STR_TITLE ||
	        type == TLV_TYPE_STR_SUBJECT || type == TLV_TYPE_STR_LOCATION ||
	        type == TLV_TYPE_STR_VERSION )
	{
		bool modified = false;
		for(uint32_t i=0;i<in.length();++i)
			if(in[i] < 0x20 && in[i] > 0)
			{
				modified = true;
				in[i] = 0x20 ;
			}

		if(modified)
			std::cerr << "RsTlvItem::GetTlvString(...) Deserialised string of "
			          << "type: " << type << " contains forbidden characters. "
			          << "They have been replaced by spaces. New string: \""
			          << in << "\"" << std::endl;
	}

	offset += tlvsize;
	return true;
}

// DEPRECATED
//bool RsTlvItem::GetTlv(void *data, uint32_t size, uint32_t *offset)
//{ return GetTlv((const uint8_t *) data, size, *offset); }

// DEPRECATED
//bool RsTlvItem::SetTlv(void *data, uint32_t size, uint32_t *offset) const
//{ return SetTlv((uint8_t *)data, size, offset); }

