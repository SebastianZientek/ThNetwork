#include "EspNowAdp.hpp"
#include <esp_now.h>

EspNowAdp::OnSendCb EspNowAdp::m_onSendCb;  // NOLINT
EspNowAdp::OnRecvCb EspNowAdp::m_onRecvCb;  // NOLINT