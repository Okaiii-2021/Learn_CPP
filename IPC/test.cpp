#include <thread>
#include <iostream>
#include <unistd.h>

void getinfo()
{
	while(1)
	{
		int i = 0;
		std::cout << "getinfor function... " << std::endl;
		std::cout << "i: ";
		std::cin >> i;

		std::cout << "I: " << i << std::endl;
	}

}

void eraseText(int cnt)
{
	char back_space=8;
	for(int i=0; i<cnt; i++)
	{
		std::cout<<back_space;
	}
}

void printtt()
{
	for (int i = 0; i < 10; i++)
	{
		sleep(2);
		eraseText(12);
		std::cout << "U: " << i << std::endl;
		std::cout << "i-: ";
		fflush(stdout);
	}
}



int main()
{
	// std::thread t1(getinfo);
	std::thread t2(printtt);
	getinfo();

	// t1.join();
	t2.join();

	return 0;
}


