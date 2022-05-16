// IMServer.cpp: 定义应用程序的入口点。
//

#include "main.h"
#include "IMServer.h"
#include "muduo/base/Singleton.h"
#include <exception>
#include <signal.h>
#include <fcntl.h>
using std::cout, std::endl, std::invalid_argument;


//void test(EventLoop& loop) {
//	muduo::net::InetAddress addr(9527); //获取本地 IP
//	muduo::net::TcpServer server(&loop, addr, "echo IM");  //
//	server.setConnectionCallback(onConect);
//	server.setMessageCallback(onMessage1);
//	server.start();
//}

void signal_exit(int signum)
{
	// TODO: 退出清理
	cout << "cacth " << signum << endl;
	exit(1);
}

// 让子进程 后台运行（因为此时父进程已经前台运行），并且让主进程退出
void daemon()
{
	signal(SIGCHLD, SIG_IGN); //忽略子进程 状态的改变
	pid_t pid = fork();
	if (0 > pid)
	{
		cout << "fork failed ,ret=" << pid << " errornum" << errno << endl;
	}
	if (pid > 0)
	{
		//主进程退出
		exit(0);
	}

	/*不管我们是 bash还是 ssh 创建进程，主进程都会创建一个会话，
	 * 而子进程会继承 主进程的会话，那主进程退出时，该会话会回收掉全部的子进程，
	 * 所以需要设置一个新的会话
	 */
	setsid();

	//不让后台影响前台,屏蔽 输入输出错误信息( 将其都关闭再 复制为指向null）
	int fd = open("/dev/null", O_RDWR);
	cout << "fd=" << fd << endl;
	if (-1 != fd)
	{
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
	}
	if (STDERR_FILENO < fd) //如果是标准输入输出流，系统会处理
	{
		close(fd);
	}
}

//// 消息回调函数   (buf：对端发送的消息）
//void  onMessage1(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time) {
//	conn->send(buf);
//	cout << time.toFormattedString() << endl;
//}
//// 连接回调函数
//void onConect(const muduo::net::TcpConnectionPtr& conn)
//{
//	cout << conn->name() << endl;
//}

int main(int argc, char* argv[], char* env[])
{
	// 信号处理
	signal(SIGCHLD, SIG_DFL);	 // 子进程状态改变
	signal(SIGPIPE, SIG_IGN);	 // 客户端断开链接时会发送管道信号，忽略掉
	signal(SIGINT, signal_exit); // ctrl+c
	// signal(SIGKILL, signal_exit);  SIGKILL 不可捕捉
	signal(SIGTERM, signal_exit); // kill 缺省
	signal(SIGILL, signal_exit);  // 非法指令
	signal(SIGTRAP, signal_exit); // ctrl + break
	signal(SIGABRT, signal_exit); //调用 abort
	signal(SIGILL, signal_exit);  //段错误

	int ch = 0;
	bool isDaemon = false;						 // 是否后台运行
	while ((ch = getopt(argc, argv, "d")) != -1) // :: 表示参数可选,  : 表示
	{
		switch (ch)
		{
		case 'd':
			cout << "found d" << endl;
			isDaemon = true;
			break;
		default:
			throw invalid_argument("use -d ");
			break;
		}
	}

	// 开启守护进程
	if (isDaemon)
	{
		daemon();
	}
	// 开启服务器逻辑
	// 创建一个事件循环对象（必须在线程中开启循环）
	muduo::net::EventLoop loop;

	if (Singleton<IMServer>::instance().init("0.0.0.0", 9527, &loop) == false)
	{
		cout << "server init failed" << endl;
		return -1;
	}
	
	loop.loop();
	return 0;
}
