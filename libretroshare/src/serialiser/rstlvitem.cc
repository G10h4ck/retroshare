/*
 * libretroshare/src/serialiser: rstlvitem.cc
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
