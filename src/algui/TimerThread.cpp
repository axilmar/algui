#include "algui/TimerThread.hpp"


namespace algui {


    TimerThread::_Timer::_Timer(TimerThread* o, const TimerFunction& f, const _TimePoint& tp, const Duration& d, bool onc)
        : owner(o)
        , function(f)
        , timePoint(tp)
        , delay(d)
        , once(onc)
    {
    }


    //adds a timer event
    TimerThread::TimerId TimerThread::add(const TimerFunction& timerFunction, const Duration& delay, bool once) {
        const auto timePoint = std::chrono::high_resolution_clock::now() + delay;
        std::shared_ptr<_Timer> timer = std::make_shared<_Timer>(this, timerFunction, timePoint, delay, once);
        al_lock_mutex(getMutex());
        timer->it = _insertSorted(timer);
        al_unlock_mutex(getMutex());
        notify();
        return timer;
    }


    //Removes the given timer timer, if not already removed.
    bool TimerThread::remove(const TimerId& id) {
        al_lock_mutex(getMutex());
        if (id.timer->owner == this) {
            id.timer->owner = nullptr;
            m_timers.erase(id.timer->it);
            al_unlock_mutex(getMutex());
            notify();
            return true;
        }
        al_unlock_mutex(getMutex());
        return false;
    }


    //check if it should wait
    bool TimerThread::shouldWait() const {
        //if thread should stop, do not wait
        if (!Thread::shouldWait()) {
            return false;
        }

        //if there are no timers, wait
        if (m_timers.empty()) {
            return true;
        }

        //wait if the time for the first timer has not yet arrived
        const std::shared_ptr<_Timer>& firstTimer = m_timers.front();
        return firstTimer->timePoint > std::chrono::high_resolution_clock::now();
    }


    //Waits for the next timer.
    void TimerThread::waitOnCond(ALLEGRO_COND* cond, ALLEGRO_MUTEX* mutex) {
        //no events; wait for one
        if (m_timers.empty()) {
            al_wait_cond(cond, mutex);
            return;
        }

        //if time for the first timer has come, don't wait
        const std::shared_ptr<_Timer>& firstTimer = m_timers.front();
        const auto now = std::chrono::high_resolution_clock::now();
        if (firstTimer->timePoint <= now) {
            return;
        }

        //wait for the remaining time for the first timer
        const double remainingSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(firstTimer->timePoint - now).count();
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, remainingSeconds);
        al_wait_cond_until(cond, mutex, &timeout);
    }


    //Executes the next available timer timer.
    void TimerThread::onNotify() {
        const auto now = std::chrono::high_resolution_clock::now();

        //iterate timers
        while (!m_timers.empty()) {
            //get the first timer
            std::shared_ptr<_Timer> timer = m_timers.front();

            //found future timer; stop
            if (timer->timePoint > now) {
                return;
            }

            //execute timer
            timer->function();

            //if timer was only for one time, erase it
            if (timer->once) {
                timer->owner = nullptr;
                m_timers.pop_front();
            }

            //else reoccuring timer; reinsert it 
            else {
                timer->timePoint = now + timer->delay;
                m_timers.pop_front();
                timer->it = _insertSorted(timer);
            }
        }
    } 

    //insert timer sorted
    TimerThread::_Iterator TimerThread::_insertSorted(const std::shared_ptr<_Timer>& timer) {
        //2 or more timers
        if (m_timers.size() >= 2) {

            //iterate the list from both sides
            auto it = m_timers.begin();
            auto itRev = m_timers.rbegin();

            //the loop will always result in an insertion
            for (;;) {
                //insert before timer at the front
                const std::shared_ptr<_Timer>& other = *it;
                if (timer->timePoint < other->timePoint) {
                    return m_timers.insert(it, timer);
                }

                //insert after timer at the back
                const std::shared_ptr<_Timer>& otherRev = *itRev;
                if (timer->timePoint >= otherRev->timePoint) {
                    return m_timers.insert(itRev.base(), timer);
                }

                //next positions
                ++it;
                ++itRev;
            }

        }

        //only one timer; compare new timer with it
        else if (m_timers.size() == 1) {
            const std::shared_ptr<_Timer>& other = m_timers.front();
            if (timer->timePoint < other->timePoint) {
                return m_timers.insert(m_timers.begin(), timer);
            }
        }

        //no timers or new timer must be placed at the end
        return m_timers.insert(m_timers.end(), timer);
    }


} //namespace algui
