#include <iostream>
#include <event2/event.h>
#include <signal.h>

using namespace std;

//sock 文件描述符， which 事件类型 arg传递的参数
static void Ctrl_C(int sock, short which, void *arg){
    cout << "Ctrl_C" << endl;
}

static void Kill(int sock, short which, void *arg){
    cout << "Kill" << endl;
    event *ev = (event *)arg;
    //如果处于非待决
    if(!evsignal_pending(ev, NULL)){
        event_add(ev, 0);
        event_add(ev, NULL);
    }
}

int main(int argc, char * argv[]){

    event_base * base = event_base_new();
    //添加ctrl + c 信号事件，处于no_pending
    //evsignal_new 隐藏的状态 EV__SIGNAL|EV_PERSIST
    event *csig = evsignal_new(base, SIGINT, Ctrl_C, base);
    if(!csig){
        cerr << "SIGINT evsignal_new failed!" << endl;
        return -1;
    }

    //添加事件到pending
    if (event_add(csig, 0) != 0) {
        cerr << "SIGINT evsignal_new failed!" << endl;
        return -1;
    }

    //添加kill 的信号
    //非持久事件， 只进入一次
    event *ksig = event_new(base, SIGTERM, EV_SIGNAL,Kill,
    event_self_cbarg());
    if(!ksig){
         cerr << "SIGTERM evsignal_new failed!" << endl;
        return -1;
    }

    if(event_add(ksig, 0) != 0){
         cerr << "SIGTERM evsignal_new failed!" << endl;
        return -1;
    }



    //进入事件主循环
    event_base_dispatch(base);
    event_free(csig);
    event_base_free(base);

    return 0;


}