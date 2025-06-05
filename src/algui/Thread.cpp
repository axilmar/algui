#include <iostream>
#include "algui/Thread.hpp"


namespace algui {


    //Creates a thread.
    Thread::Thread()
        : m_mutex(al_create_mutex())
        , m_cond(al_create_cond())
        , m_thread(al_create_thread(&Thread::_threadProc, this))
    {
    }


    //Stops and destroys the thread.
    Thread::~Thread() {
        al_destroy_thread(m_thread);
        al_destroy_cond(m_cond);
        al_destroy_mutex(m_mutex);
    }


    //Starts the thread.
    void Thread::start() {
        al_start_thread(m_thread);
    }


    //Stops the thread.
    void Thread::stop() {
        al_join_thread(m_thread, nullptr);
    }


    //Signals the condition variable, waking up the underlying thread.
    void Thread::notify() {
        al_signal_cond(m_cond);
    }


    //check if the thread should wait.
    bool Thread::shouldWait() const {
        return !al_get_thread_should_stop(m_thread);
    }


    //wait on condition
    void Thread::waitOnCond(ALLEGRO_COND* cond, ALLEGRO_MUTEX* mutex) {
        return al_wait_cond(cond, mutex);
    }


    //Interface for running the thread.
    void Thread::run() {
        for (;;) {
            al_lock_mutex(m_mutex);

            try {
                //wait for either thread stop or a notification
                while (shouldWait()) {
                    waitOnCond(m_cond, m_mutex);
                }

                //if thread should stop, return
                if (al_get_thread_should_stop(m_thread)) {
                    al_unlock_mutex(m_mutex);
                    break;
                }

                //execute notification
                onNotify();

                al_unlock_mutex(m_mutex);
            }

            catch (...) {
                al_unlock_mutex(m_mutex);
                std::cerr << "Thread: run(): exit due to exception.\n";
                return;
            }
        }
    }


    //internal thread proc which invokes `run()`.
    void* Thread::_threadProc(ALLEGRO_THREAD* thread, void* threadPtr) {
        reinterpret_cast<Thread*>(threadPtr)->run();
        return nullptr;
    }


} //namespace algui
