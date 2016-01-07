
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


bool RsTlvItem::GetTlv(void *data, uint32_t size, uint32_t *offset)
{ return GetTlv((const uint8_t *) data, size, *offset); }


bool RsTlvItem::SetTlv(void *data, uint32_t size, uint32_t *offset) const
{ return SetTlv((uint8_t *)data, size, offset); }

