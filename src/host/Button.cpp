#include <utility>

#include "Button.hpp"

Button::Button(const std::shared_ptr<IArduino32Adp> &arduinoAdp, uint8_t buttonPin)
    : m_arduinoAdp(arduinoAdp)
    , m_buttonPin(buttonPin)
{
    m_arduinoAdp->pinMode(buttonPin, IArduino32Adp::Mode::PIN_INPUT_PULLUP);
}

void Button::onClick(const BtnClbk &clbk)
{
    m_onClick = clbk;
}

// onLongClick have higher priority than onClick
void Button::onLongClick(size_t timeMs, const BtnClbk &clbk)
{
    m_onLongClickTime = timeMs;
    m_onLongClick = clbk;
}

void Button::update()
{
    auto pressed = m_arduinoAdp->digitalRead(m_buttonPin);
    m_lastButtonPressed = std::exchange(m_buttonPressed, pressed);

    // Button pressed
    if (!m_lastButtonPressed && m_buttonPressed)
    {
        m_startBtnTimer = m_arduinoAdp->millis();
    }

    // Button released (was pressed, but now is not)
    if (m_lastButtonPressed && !m_buttonPressed)
    {
        auto currentTime = m_arduinoAdp->millis();
        if (m_onLongClick && currentTime - m_startBtnTimer >= m_onLongClickTime)
        {
            m_onLongClick();
            return;
        }

        if (m_onClick)
        {
            m_onClick();
        }
    }
}
