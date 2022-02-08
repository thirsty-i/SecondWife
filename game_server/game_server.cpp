#include "platform.h"

#include "game_server.h"
//#include <memory>
//#include "epoll/acceptor.h"
#include "ip/address.h"
//#include <thread>
#include "logger/log.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

game_server::game_server()
	: service_()
{
	net::endpoint endpoint(net::ip::address::from_string("127.0.0.1"), 8001);
	acceptor_ = new net::acceptor(&service_, endpoint);
	
	start();
}

bool game_server::start()
{
	_start_nets();
	return true;
}

bool game_server::_start_nets()
{
	service_.start();
	return true;
}

int test(int a)
{
	return a;
}

struct A
{
	int a;
	int b;
	int c;

	A()
	{
	}

	virtual void func()
	{
		LOG(ERROR) << "A::func, a:" << a;
	}
};


INITIALIZE_EASYLOGGINGPP
int main()
{
	//g_init_log(game_server::instance().server_name());
	//while (1);

	
	key_t key;
	key = ftok("......", 1);

	int shmid = shmget(key, sizeof(A), IPC_CREAT | 0666);
	LOG_PROCESS_ERROR_RET(shmid != -1, 0);

	void* shmaddr = shmat(shmid, 0, 0);
	//::new (shmaddr) B(10);

	A* pA = static_cast<A*>(shmaddr);
	pA->a = 10;

	// 假设这里是重启后的代码
	::new (shmaddr) A();
	A* pA_1 = static_cast<A*>(shmaddr);
	pA_1->func();

	return 0;
}