#ifndef ALGUI_THREAD_HPP
#define ALGUI_THREAD_HPP


#pragma warning (disable: 4309)
#include "allegro5/allegro.h"


namespace algui {


    /**
     * Allegro thread wrapper.
     */
    class Thread {
    public:
        /**
         * Creates a thread.
         * It also creates a mutex and a condition variable,
         * in order to allow a thread loop to run idle
         * @param start if true, then the thread starts immediately.
         */
        Thread(bool start = true);

        Thread(const Thread&) = delete;
        Thread(Thread&&) = delete;

        /**
         * Stops and destroys the thread.
         */
        ~Thread();

        Thread& operator =(const Thread&) = delete;
        Thread& operator =(Thread&&) = delete;

        /**
         * Starts the thread.
         */
        virtual void start();

        /**
         * Stops the thread.
         */
        virtual void stop();

        /**
         * Signals the condition variable, waking up the underlying thread.
         */
        virtual void notify();

    protected:
        /**
         * Returns the thread's mutex.
         * @return the thread's mutex.
         */
        ALLEGRO_MUTEX* getMutex() const {
            return m_mutex;
        }

        /**
         * Returns the thread's condition variable.
         * @return the thread's condition variable.
         */
        ALLEGRO_COND* getCond() const {
            return m_cond;
        }

        /**
         * Checks if the thread should wait.
         * The default implementation checks if the thread should stop or the thread is notified.
         * @return true if the thread should wait, false otherwise.
         */
        virtual bool shouldWait() const;

        /**
         * Interface for waiting on condition;
         * The default implementation waits for the thread to stop or for a notify signal.
         * @param cond the thread's condition variable.
         * @param mutex the thread's mutex.
         */
        virtual void waitOnCond(ALLEGRO_COND* cond, ALLEGRO_MUTEX* mutex);

        /**
         * Interface for doing some action, when notified.
         * The default implementation does nothing.
         */
        virtual void onNotify() {
        }

        /**
         * Interface for running the thread.
         * The default implementation makes a loop which waits on the condition variable,
         * calling the method `onNotify()` when it wakes up, but before unlocking the mutex.
         */
        virtual void run();

    private:
        ALLEGRO_MUTEX* m_mutex;
        ALLEGRO_COND* m_cond;
        ALLEGRO_THREAD* m_thread;

        //internal thread proc which invokes `run()`.
        static void* _threadProc(ALLEGRO_THREAD* thread, void* threadPtr);
    };


} //namespace algui


#endif //ALGUI_THREAD_HPP
