#include <event2/event.h>
#include <iostream>
#ifdef _WIN32
#else
    #include <signal.h>
#endif

using namespace std;

int main(){

#ifdef _WIN32
    //初始化socket库
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#else
    //忽略管道信号， 发送数据给已关闭的socket
    if (signal(SIGPIPE,SIG_IGN) == SIG_ERR){
        return 0;
    }
#endif

    //初始话libevent上下文

    //配置上下文

   
#ifdef _WIN32
    WSACleanup();
#endif

}



