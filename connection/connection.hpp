#pragma once

namespace putils
{
	namespace connection
	{
		template<typename Int>
		void add(Int connection)
		{
#ifdef __unix__
			(void)connection;
#endif
#ifdef _WIN32
			static int openConnections = 0;

			if (openConnections == 0)
			{
				WSADATA _;
				WSAStartup(0x202, &_);
			}
			openConnections += connection;

			if (openConnections == 0)
				WSACleanup();
#endif
		}

		template<typename Int>
		static void closeSocket(Int &&fd)
		{
#ifdef __unix__
			close(fd);
#elif defined(_WIN32)
			closesocket(fd);
#endif
		}
	}
}