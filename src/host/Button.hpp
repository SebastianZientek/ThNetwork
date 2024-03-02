#pragma once

#include <functional>
#include <memory>

#include "adapters/IArduino32Adp.hpp"

class Button
{
    using BtnClbk = std::function<void()>;

public:
    Button(const std::shared_ptr<IArduino32Adp> &arduinoAdp, uint8_t buttonPin);

    void onClick(const BtnClbk &clbk);
    void onLongClick(size_t timeMs, const BtnClbk &clbk);
    void update();

private:
    std::shared_ptr<IArduino32Adp> m_arduinoAdp;

    constexpr static auto DOWN = IArduino32Adp::Lvl::Low;
    constexpr static auto UP = IArduino32Adp::Lvl::High;

    BtnClbk m_onClick{};
    BtnClbk m_onLongClick{};
    size_t m_onLongClickTime{};
    uint8_t m_buttonPin{};

    IArduino32Adp::Lvl m_currentButtonState{UP};
    IArduino32Adp::Lvl m_lastButtonState{UP};
    size_t m_startBtnTimer{};
};
