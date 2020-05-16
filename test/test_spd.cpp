#include <chrono>
#include <iostream>
#include <string>

using namespace std;

template<typename Func, typename... Args>
void exec_spd(const string& funcName, Func func, Args&... args){
    auto start_time = std::chrono::steady_clock::now();
    func(args...);
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    std::cout << "The exec time of " << funcName << " is : "<< duration.count() << "ms" << std::endl;
}



int main(int argc, char **argv){
	char buf[1024];
	snprintf(buf, 1024, "mysql -uroot -h127.0.0.1 -P%s -p123 < %s > log", argv[1], argv[2]);
	exec_spd("system", system, buf);
}