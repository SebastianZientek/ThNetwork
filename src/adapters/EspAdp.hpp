#pragma once

#ifdef UNIT_TESTS
#include <functional>
#include <memory>
#endif

struct EspAdp
{
    static void feedWatchdog();
    static void yield();
    static void wait(unsigned long timeout);
    static bool isResetReasonDeepSleepAwake();

#ifdef UNIT_TESTS
    using OnWaitFunc = std::function<void()>;
    static std::shared_ptr<OnWaitFunc> createAndInjectOnWaitCb(OnWaitFunc onWaitCb);

private:
    static std::weak_ptr<OnWaitFunc> m_waitCbPtr;
#endif
};
