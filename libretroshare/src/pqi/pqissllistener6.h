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
 * along with libretroshare. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PQISSLLISTENER6_H
#define PQISSLLISTENER6_H

#include "pqi/pqiaddress.h"
#include "pqi/pqissllistener.h"

#include <list>

class pqissllistener6 : public pqissllistener
{
public:
	pqissllistener6(const sockaddr_storage &addr, p3PeerMgr *pm) :
	    pqissllistener(addr, pm) {}

	int setuplisten();
	bool exportListenAddresses(std::list<const pqiaddress*>& out);

private:
	std::list<pqiUrlAddress> listenAddresses;
};

#endif // PQISSLLISTENER6_H
