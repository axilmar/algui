#ifndef ALGUI_TIMERTHREAD_HPP
#define ALGUI_TIMERTHREAD_HPP


#include <functional>
#include <chrono>
#include <map>
#include <memory>
#include "Thread.hpp"


namespace algui {


    /**
     * A thread that allows multiplexing of timed events.
     */
    class TimerThread : public Thread {
    public:
        /**
         * Timer function type.
         */
        using TimerFunction = std::function<void()>;

        /**
         * Duration type.
         */
        using Duration = std::chrono::steady_clock::duration;

    private:
        using _TimePoint = std::chrono::steady_clock::time_point;

        struct _Timer;

        using _TimerContainer = std::multimap<_TimePoint, std::shared_ptr<_Timer>>;
        using _Iterator = _TimerContainer::iterator;

        struct _Timer {
            TimerThread* owner;
            TimerFunction function;
            _TimePoint timePoint;
            Duration delay;
            bool once;
            _Iterator it;
            _Timer(TimerThread* o, const TimerFunction& f, const _TimePoint& tp, const Duration& d, bool onc);
        };

    public:
        /**
         * Timer id type.
         */
        struct TimerId {
        public:
            /**
             * The default constructor.
             * The timer id is empty.
             */
            TimerId() {}

        private:
            std::shared_ptr<_Timer> timer;
            TimerId(const std::shared_ptr<_Timer>& t) : timer(t) {}
            friend class TimerThread;
        };

        /**
         * Puts a function to be invoked after the given delay.
         * The function will be invoked in the context of the timer thread.
         * @param timerFunction timer function to execute.
         * @param delay amount of time to wait until the function is executed.
         * @param once if true, then the function is executed once, otherwise at regular intervals.
         * @return the timer id for this timer.
         */
        TimerId add(const TimerFunction& timerFunction, const Duration& delay, bool once = true);

        /**
         * Same as above, but duration is in milliseconds value.
         */
        TimerId add(const TimerFunction& timerFunction, long long msecs, bool once = true) {
            return add(timerFunction, std::chrono::duration_cast<Duration>(std::chrono::milliseconds(msecs)), once);
        }

        /**
         * Removes the given timer timer, if not already removed.
         * @param id of timer to remove.
         * @return true if the timer was removed, false otherwise.
         */
        bool remove(const TimerId& id);

    protected:
        /**
         * It should not wait if there are pending timer events and there is at least one timer event
         * that must be executed.
         * @return if it should wait, false otherwise.
         */
        bool shouldWait() const override;

        /**
         * Waits for the next timer.
         * @param cond the thread's condition variable.
         * @param mutex the thread's mutex.
         */
        void waitOnCond(ALLEGRO_COND* cond, ALLEGRO_MUTEX* mutex) override;

        /**
         * Executes the next available timer timer.
         */
        void onNotify() override;

    private:
        //fields
        _TimerContainer m_timers;
    };


    /**
     * Timer id.
     */
    using TimerId = TimerThread::TimerId;


    /**
     * Timer function.
     */
    using TimerFunction = TimerThread::TimerFunction;


} //namespace algui


#endif //ALGUI_TIMERTHREAD_HPP
