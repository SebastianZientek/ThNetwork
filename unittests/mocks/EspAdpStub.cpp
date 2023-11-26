#include "EspAdp.hpp"

void EspAdp::feedWatchdog()
{
}

void EspAdp::yield()
{
}

void EspAdp::wait(unsigned long timeout)
{
    if (auto callback = m_waitCbPtr.lock())
    {
        (*callback)();
    }
}

std::weak_ptr<EspAdp::OnWaitFunc> EspAdp::m_waitCbPtr;  // NOLINT

std::shared_ptr<EspAdp::OnWaitFunc> EspAdp::createAndInjectOnWaitCb(EspAdp::OnWaitFunc onWaitCb)
{
    auto onWaitCbShr = std::make_shared<EspAdp::OnWaitFunc>(onWaitCb);
    m_waitCbPtr = onWaitCbShr;
    return onWaitCbShr;
}
