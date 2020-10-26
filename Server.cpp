#include<iostream>
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<string>
#include<algorithm>
#include<ctime>
#include<fstream>
#pragma warning(disable: 4996)
#pragma comment(linker, "/STACK:60000")


void InfoOut(std::ofstream& out, std::string info, std::string path)
{
	out.open(path, std::ios::app);
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);

	out << asctime(timeinfo) <<" - "<< info << "\n\n";
	out.close();
}

int main(int argc, char* argv[])
{
	WSAData wsaData;
	if (WSAStartup(0x0202, &wsaData) != 00)
	{
		std::cout << "error lib don`t download\n";
		return 1;
	}
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1048);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout << "Error at socket()";
		return 1;
	}

	if (bind(ListenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << "Error, bind() failed";
		return 1;
	}

	if (listen(ListenSocket, 1) == SOCKET_ERROR)
	{
		std::cout << "Error, listen() failed";
		return 1;
	}

	SOCKET AcceptSocket;


	std::ofstream out;

	while (1) {
		AcceptSocket = SOCKET_ERROR;
		while (AcceptSocket == SOCKET_ERROR) {
			AcceptSocket = accept(ListenSocket, NULL, NULL);
		}
		std::cout << "Client connected.\n";
		InfoOut(out, "Server: Client connected.", "D:\\Info_file");
		InfoOut(out, "________________________server :START NEW CONNECTION________________________", "D:\\Info_file");
		ListenSocket = AcceptSocket;
		break;
	}

	/// ///////////////////////////////////////////////////////////////////////////////////////
	
	

	setlocale(LC_ALL, "rus");
	std::string info = "Commands: start - start network test.\n\t_quit - close connection with server.\n\t_who_ - info about author.\n";
	send(ListenSocket, &info[0],  info.length(), NULL);

	InfoOut(out, "Server: send(): info about command to client : " + info, "D:\\Info_file");

	srand(time(NULL));
	while (true)
	{

	char command[5] = "";
	int bytesRecv = SOCKET_ERROR;
	while (bytesRecv == SOCKET_ERROR) {
		bytesRecv = recv(ListenSocket, command, sizeof(command), NULL);
		}

	InfoOut(out, "Server: recv(): command", "D:\\Info_file");

	    if (strstr(command,"_who_") != nullptr)
		{
			char info[61] = "Автор Бубка Михайло, В-23 Тестування продуктивностi мережi.\n";
			send(ListenSocket, info, sizeof(info), 0);
			InfoOut(out, "Server: command - \"_who_\": send(): " + std::string(info), "D:\\Info_file");
		}
		else if (strstr(command, "_quit") != nullptr)
		{
			if (closesocket(ListenSocket) != 0)
			{
				std::cout << "Error. close() failed\n";
				InfoOut(out, "Server: command - \"_quit\": Error. close() failed", "D:\\Info_file");
				out.close();
				exit(1);
			}
			else
			{
				std::cout << "connection completed\n";
				InfoOut(out, "Server: command - \"_quit\": connection completed", "D:\\Info_file");
				out.close();
				exit(0);
			}
		}
		else if(strstr(command, "start") != nullptr)
		{
			char sendbuf[50000];
			int min = 50000;
			int max = 0;
			int Sum = 0;

			int num;
			recv(ListenSocket, (char*)&num, sizeof(num), NULL);
			InfoOut(out, "Server: command - \"start\": recv(): num of message:" + std::to_string(num), "D:\\Info_file");
			if(num > 0)
			{
				InfoOut(out, "Server: command - \"start\": start testing", "D:\\Info_file");
				double start_time = clock();
				for (int i = 0; i < num; ++i)
				{
					int Size = rand() % (50000 - 1 + 1) + 1;
					//std::cout << Size << "\n";
					send(ListenSocket, (char*)&Size, sizeof(Size), NULL);

					if (min > Size) min = Size;
					if (max < Size) max = Size;
					Sum += Size;
					std::cout << "Msg: " + std::to_string(i) + "\n";
					send(ListenSocket, &sendbuf[50000 - Size], Size, NULL);
				    recv(ListenSocket, sendbuf, Size, NULL);
					

				}
				double end_time = clock();
				Sum /= num;
				double ResTime = end_time - start_time;
				double AverageTime = ResTime / num;
				int NumOfMessage = num * 2;

				InfoOut(out, "Server: command - \"start\": end of testing", "D:\\Info_file");

				std::string NOM = "Кiлькiсть повiдомленнь: " + std::to_string(NumOfMessage) + '\n';
				std::string Time = "Витрачений час: " + std::to_string(ResTime) + " ms.\n";
				std::string SUM = "Середнiй розмiр повiдомлення: " + std::to_string(Sum) + " bytes.\n";
				std::string ATime = "Середнiй час на повiдомлення: " + std::to_string(AverageTime) + " ms.\n";
				std::string MIN = "Мiнiмальний розмiр повiдомлення: " + std::to_string(min) + " bytes.\n";
			    std::string MAX = "Максимальний розмiр повiдомлення: " + std::to_string(max) + " bytes.\n";
				std::string result = NOM + Time + SUM + ATime + MIN + MAX;
				std::cout << result << "\n";
				send(ListenSocket, &result[0], result.length(), NULL);
				InfoOut(out, "Server: command - \"start\": send(): result:" + result, "D:\\Info_file");
				

			}

		}
		else
		{
			char error[17] = "Unknown command\n";
			send(ListenSocket, error, sizeof(error), NULL);
			InfoOut(out, "Server: Unknown command: send(): \"unknown command\"", "D:\\Info_file");
		}
	}

	system("pause");
	return 0;
}