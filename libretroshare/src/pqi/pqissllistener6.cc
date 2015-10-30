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

#include "pqissllistener6.h"
#include "pqi/pqinetwork.h"
#include "util/rsnet.h"


int	pqissllistener6::setuplisten()
{
	if(active) return -1;

	lsock = socket(PF_INET6, SOCK_STREAM, 0);

	if(lsock < 0)
	{
		unix_close(lsock);
		lsock = -1;
		std::cout << "pqissllisten6::setuplisten() Cannot Open Socket: "
		          << errno << " " << socket_errorType(errno) << std::endl;
		return -1;
	}

	if(unix_fcntl_nonblock(lsock) < 0)
	{
		unix_close(lsock);
		lsock = -1;
		std::cout << "pqissllisten6::setuplisten() Cannot make socket nonblocking: "
		          << errno << " " << socket_errorType(errno) << std::endl;
		return -1;
	}

	/* added a call to REUSEADDR, so that we can re-open an existing socket
	 * when we restart_listener.
	 */
	int on = 1;
	if (0 > unix_setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
	{
		std::cout << "pqissllisten6::setuplisten() cannot set SO_REUSEADDR: "
		          << errno << " " << socket_errorType(errno) << std::endl;
		unix_close(lsock);
		lsock = -1;
		exit(1);
	}

	struct sockaddr_storage tmpaddr;
	sockaddr_storage_clear(tmpaddr);
	tmpaddr.ss_family = PF_INET6;
	sockaddr_storage_setport(tmpaddr, sockaddr_storage_port(laddr));
	if (0 != universal_bind(lsock, (struct sockaddr *) &tmpaddr, sizeof(tmpaddr)))
	{
		std::cout << "pqissllistenbase6::setuplisten() Cannot Bind to Local Address: "
		          << sockaddr_storage_tostring(tmpaddr) << std::endl
		          << socket_errorType(errno) << std::endl;
		exit(1);
	}

	if (0 != listen(lsock, 100))
	{
		std::cout << "pqissllisten6::setuplisten() Error: Cannot Listen to Socket: "
				  << errno << " " << socket_errorType(errno) << std::endl;
		exit(1);
	}

	active = true;

	// Update active listen addresses set
	pqiUrlAddress paddr;
	paddr.value.clear();
	paddr.value += "ipv6+tcp://[::]:";
	paddr.value += sockaddr_storage_port(laddr);
	paddr.value += "/";
	listenAddresses.clear();
	listenAddresses.push_back(paddr);

	std::list<sockaddr_storage> hostAddrs;
	std::list<sockaddr_storage>::const_iterator it;
	if(getLocalAddresses(hostAddrs))
		for(it = hostAddrs.begin(); it != hostAddrs.end(); ++it)
			if((*it).ss_family == AF_INET6)
			{
				paddr.value.clear();
				paddr.value += "ipv6+tcp://[:";
				paddr.value += sockaddr_storage_iptostring(*it);
				paddr.value += ":]:";
				paddr.value += sockaddr_storage_port(laddr);
				paddr.value += "/";
				listenAddresses.push_back(paddr);
			}

	return 1;
}

bool pqissllistener6::exportListenAddresses(std::list<const pqiaddress*>& out)
{
	out.clear();
	std::list<pqiUrlAddress>::const_iterator it = listenAddresses.begin();
	for(; it != listenAddresses.end(); ++it)
		out.push_back(&*it);
	return true;
}
