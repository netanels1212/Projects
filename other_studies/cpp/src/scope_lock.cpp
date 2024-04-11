#include <iostream>
#include <pthread.h>

using namespace std;
class Lock
{
public:
    Lock(pthread_mutex_t& m) : mutex(m)
    {
        pthread_mutex_lock(&mutex);
    }

    ~Lock()
    {
        pthread_mutex_unlock(&mutex);
    }

private:
    pthread_mutex_t& mutex;
};

void test()
{
    pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

    {
        Lock lock(m);
        // The mutex is now locked
        cout << "Mutex is locked.\n";
        // The mutex will be unlocked when 'lock' goes out of scope
    }

    cout << "Mutex is unlocked.\n";
}

int main()
{
    test();
    return 0;
}
