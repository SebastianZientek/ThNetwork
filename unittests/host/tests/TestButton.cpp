#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <memory>

#include "Button.hpp"
#include "mocks/Arduino32AdpMock.hpp"

// clang-format off
TEST_GROUP(ButtonTest)  // NOLINT
{   
    void teardown() override{
        mock().checkExpectations();
        mock().clear();
    }

    std::shared_ptr<Arduino32AdpMock> arduinoAdp{std::make_shared<Arduino32AdpMock>()};
    constexpr static auto buttonPin = 6;
};
// clang-format on
static IArduino32Adp::Lvl lvlHigh{IArduino32Adp::Lvl::High};
static IArduino32Adp::Lvl lvlLow{IArduino32Adp::Lvl::Low};

TEST(ButtonTest, DoNothingWhenCallbackIsNotSet)  // NOLINT
{
    mock("Arduino32Adp")
        .expectOneCall("pinMode")
        .withParameter("pin", buttonPin)
        .withParameter("mode", static_cast<int>(IArduino32Adp::Mode::PIN_INPUT_PULLUP));

    Button button(arduinoAdp, buttonPin);

    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlLow);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(1);
    button.update();

    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlHigh);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(2);
    button.update();
}

TEST(ButtonTest, CallOnClickCallbackOnceWhenClicked)  // NOLINT
{
    mock("Arduino32Adp")
        .expectOneCall("pinMode")
        .withParameter("pin", buttonPin)
        .withParameter("mode", static_cast<int>(IArduino32Adp::Mode::PIN_INPUT_PULLUP));

    Button button(arduinoAdp, buttonPin);
    button.onClick(
        []
        {
            mock("Lambda").actualCall("onClick");
        });

    // Button pressed
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlLow);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(1);
    button.update();

    // Button released -> call function
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlHigh);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(2);
    mock("Lambda").expectOneCall("onClick");
    button.update();

    // Button pressed -> don't do anything
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlLow);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(3);
    button.update();
}

TEST(ButtonTest, CallOnLongClickCallbackWhenClickIsEnoughLong)  // NOLINT
{
    mock("Arduino32Adp")
        .expectOneCall("pinMode")
        .withParameter("pin", buttonPin)
        .withParameter("mode", static_cast<int>(IArduino32Adp::Mode::PIN_INPUT_PULLUP));

    Button button(arduinoAdp, buttonPin);
    button.onLongClick(30,
                       []
                       {
                           mock("Lambda").actualCall("onLongClick");
                       });

    // Button pressed
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlLow);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(0);
    button.update();

    // Button released -> not call because it's too early
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlHigh);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(25);
    button.update();

    // Button pressed -> don't do anything yet
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlLow);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(30);
    button.update();

    // Button released after timeout, call long click callback
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlHigh);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(60);
    mock("Lambda").expectOneCall("onLongClick");
    button.update();
}

TEST(ButtonTest, SetAndUseOnClickAndOnLongClickAtOnce)  // NOLINT
{
    mock("Arduino32Adp")
        .expectOneCall("pinMode")
        .withParameter("pin", buttonPin)
        .withParameter("mode", static_cast<int>(IArduino32Adp::Mode::PIN_INPUT_PULLUP));

    Button button(arduinoAdp, buttonPin);
    button.onClick(
        []
        {
            mock("Lambda").actualCall("onClick");
        });

    button.onLongClick(30,
                       []
                       {
                           mock("Lambda").actualCall("onLongClick");
                       });

    // Button pressed
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlLow);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(0);
    button.update();

    // Button released -> too eraly for onLongClick, so onClick is called
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlHigh);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(25);
    mock("Lambda").expectOneCall("onClick");
    button.update();

    // Button pressed -> don't do anything yet
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlLow);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(30);
    button.update();

    // Button released after timeout, call onLongClick callback, but don't call onClick
    mock("Arduino32Adp")
        .expectOneCall("digitalRead")
        .withParameter("pin", buttonPin)
        .andReturnValue(&lvlHigh);
    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(60);
    mock("Lambda").expectOneCall("onLongClick");
    button.update();
}
