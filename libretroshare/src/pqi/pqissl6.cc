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
