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

#include "pqissl6.h"
#include "util/rsnet.h"

int pqissl6::Initiate_Connection()
{
	if(waiting != WAITING_DELAY)
	{
		std::cerr << "pqissl6::Initiate_Connection() Attempt Already  in Progress!" << std::endl;
		return -1;
	}

	if(sockaddr_storage_isnull(remote_addr))
	{
		waiting = WAITING_FAIL_INTERFACE;
		return -1;
	}

	int osock = unix_socket(PF_INET6, SOCK_STREAM, 0);
	if( osock < 0 )
	{
		waiting = WAITING_FAIL_INTERFACE;
		unix_close(osock);
		return -1;
	}

	if( unix_fcntl_nonblock(osock) < 0 )
	{
		waiting = WAITING_FAIL_INTERFACE;
		unix_close(osock);
		return -1;
	}

	mTimeoutTS = time(NULL) + mConnectTimeout;

	if( 0 != unix_connect(osock, (sockaddr *) &remote_addr, sizeof(remote_addr)) )
	{
		std::cerr << "pqissl6::Initiate_Connection() connect error: "
				  << errno << " " << socket_errorType(errno) << std::endl;

		switch(errno)
		{
		case EINPROGRESS:
			waiting = WAITING_SOCK_CONNECT;
			sockfd = osock;
			return 0;
		default:
			waiting = WAITING_FAIL_INTERFACE;
			unix_close(osock);
			osock = -1;

			std::cerr << "pqissl6::Initiate_Connection Connection Failed: "
					  << errno << " " << socket_errorType(errno) << std::endl;

			return -1;
		}
	}

#ifdef PQISSL_LOG_DEBUG
	std::cerr << "pqissl::Initiate_Connection() Waiting for Socket Connect" << std::endl;
#endif
	waiting = WAITING_SOCK_CONNECT;
	sockfd = osock;
	return 1;
}
