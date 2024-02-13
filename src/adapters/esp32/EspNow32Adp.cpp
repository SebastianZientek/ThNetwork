#include "EspNow32Adp.hpp"
#include <esp_now.h>

EspNow32Adp::OnSendCb EspNow32Adp::m_onSendCb;  // NOLINT
EspNow32Adp::OnRecvCb EspNow32Adp::m_onRecvCb;  // NOLINT