#include<iostream>
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<string>
#include<ctime>
#include<fstream>
#pragma warning(disable: 4996)
#pragma comment(linker, "/STACK:60000")

void InfoOut(std::ofstream& out, std::string info, std::string path)
{
	out.open(path, std::ios::app);
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);

	out << asctime(timeinfo) << " - " << info << "\n\n";
	out.close();
}

int main(int argc, char* argv[])
{
	WSAData wsaData;
	if (WSAStartup(0x0202, &wsaData) != 0)
	{
		std::cout << "error lib dont download\n";
		return 1;
	}
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1048);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	SOCKET connection =  socket(AF_INET,SOCK_STREAM,NULL);

	std::ofstream out;

	if (connect(connection, (sockaddr*)&addr, sizeof(addr))!=0)
	{
		std::cout << "Error, failed connection to server!";
		return 1;
	}

	else
	{
		std::cout << "connection succesfull!\n";
	}
	setlocale(LC_ALL, "rus");

	char recvbuf[106] = "";
	recv(connection, recvbuf, sizeof(recvbuf), 0);
	std::cout << recvbuf;
	InfoOut(out, "Client: recv(): " + std::string(recvbuf), "D:\\Info_file");

	while (true)
	{
		int n = 1;
		char command[5] = "";
		std::cout << "Enter command: \n";
		std::cin >> command;
		send(connection, command, sizeof(command), 0);
		InfoOut(out, "Client: send(): command", "D:\\Info_file");

		if (command == std::string("_who_"))
		{
			char info[61] = "";
			recv(connection, info, sizeof(info), NULL);
			std::cout << info;
			InfoOut(out, "Client: command - \"_who_\": recv(): " + std::string(info), "D:\\Info_file");
		}
		else if (command == std::string("_quit"))
		{
			out.close();
			exit(0);
		}
		else if(command == std::string("start"))
		{
			std::cout << "Enter num of message: \n";
			int num;
		    std::cin >> num;

			
			send(connection, (char*)&num, sizeof(num), NULL);
			InfoOut(out, "Client: command - \"start\": send(): num of message:" + std::to_string(num), "D:\\Info_file");
			char msg[50000];
			if (num > 0)
			{
				char Size[10] = "1";
				InfoOut(out, "Client: command - \"start\": start receiving", "D:\\Info_file");
				for (int i = 0; i < num; ++i)
				{
					//std::cout << "Msg: " + std::to_string(i) + "\n";
					int size;
				    recv(connection, (char*)&size, sizeof(size), NULL);
					
					
					recv(connection, msg, size, NULL);


					send(connection, &msg[50000 - size], size, NULL);

				}
				InfoOut(out, "Client: command - \"start\": end of receiving ", "D:\Info_file");
				char Result[280];
				recv(connection, Result, sizeof(Result), NULL);
				InfoOut(out, "Client: command - \"start\": recv(): Result - " + std::string(Result), "D:\\Info_file");
				std::string res(Result);
				std::cout << res << '\n';
				/*for (int i = 0; i < 280; ++i)
				{
					if (Result[i] != 'M')std::cout << Result[i];
				}*/


			}
		}
		else
		{
			char info[17];
			recv(connection, info, sizeof(info), NULL);
			InfoOut(out, "Client: Unknown command: recv(): " + std::string(info), "D:\\Info_file");
			std::cout << info;
		}
	}


	system("pause");
	return 0;
}