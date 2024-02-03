#include <CppUTest/TestHarness.h>

#include "ConfStorage.hpp"
#include "FileMock.hpp"
#include "FileSystemMock.hpp"
#include "RaiiFile.hpp"

TEST_GROUP(ConfStorageTest)  // NOLINT
{void teardown() override{mock().checkExpectations();
mock().clear();
}
}
;

TEST(ConfStorageTest, ShouldLoadCorrectConfiguration)  // NOLINT
{
    FileMock fileMock;
    ConfStorage confStorage;
    RaiiFile file(fileMock);

    std::string fileContent = R"({"user":"admin"})";
    mock().expectOneCall("FileMock::readString").andReturnValue(&fileContent);
    mock().ignoreOtherCalls();

    auto state = confStorage.load(file);
    CHECK_TRUE(ConfStorage::State::OK == state);
}

TEST(ConfStorageTest, ShouldntLoadIncorrectConfiguration)  // NOLINT
{
    FileMock fileMock;
    ConfStorage confStorage;
    RaiiFile file(fileMock);

    std::string fileContent = R"(brokenjsonfile:"admin"})";
    mock().expectOneCall("FileMock::readString").andReturnValue(&fileContent);
    mock().ignoreOtherCalls();

    auto state = confStorage.load(file);
    CHECK_TRUE(ConfStorage::State::FAIL == state);
}

TEST(ConfStorageTest, ShouldSaveDefaultConfiguration)
{
    FileMock fileMock;
    ConfStorage confStorage;
    RaiiFile file(fileMock);

    std::string expectedStringToSave
        = R"({"admin":{"passwd":"passwd","user":"admin"},"sensorUpdatePeriodMins":1,"sensors":null,"serverPort":80})";

    mock()
        .expectOneCall("FileMock::print")
        .withStringParameter("data", expectedStringToSave.c_str());
    mock().ignoreOtherCalls();

    confStorage.setDefault();
    auto state = confStorage.save(file);
    CHECK_TRUE(ConfStorage::State::OK == state);
}

TEST(ConfStorageTest, ShouldParseFileAndReadWifiConfig)
{
    FileMock fileMock;
    ConfStorage confStorage;
    RaiiFile file(fileMock);

    std::string fileContent = R"({"wifi":{"ssid": "test", "pass": "testPass"}})";
    mock().expectOneCall("FileMock::readString").andReturnValue(&fileContent);
    mock().ignoreOtherCalls();

    auto state = confStorage.load(file);
    CHECK_TRUE(ConfStorage::State::OK == state);

    auto wifiConfig = confStorage.getWifiConfig();
    CHECK_TRUE(wifiConfig.has_value());

    auto [ssid, pass] = wifiConfig.value();
    CHECK_EQUAL(ssid, "test");
    CHECK_EQUAL(pass, "testPass");
}

TEST(ConfStorageTest, ShouldReturnNulloptWhenWifiConfigIsIncompleteOrWrong)
{
    FileMock fileMock;
    ConfStorage confStorage;
    RaiiFile file(fileMock);

    std::string fileContent = R"({"wifi":{"pass": "testPass"}})";
    mock().expectOneCall("FileMock::readString").andReturnValue(&fileContent);
    mock().ignoreOtherCalls();

    auto state = confStorage.load(file);
    CHECK_TRUE(ConfStorage::State::OK == state);

    auto wifiConfig = confStorage.getWifiConfig();
    CHECK_FALSE(wifiConfig.has_value());
}

TEST(ConfStorageTest, ShouldSaveWifiConfiguration)
{
    FileMock fileMock;
    ConfStorage confStorage;
    RaiiFile file(fileMock);

    std::string expectedFileContent = R"({"wifi":{"pass":"thing","ssid":"some"}})";
    mock()
        .expectOneCall("FileMock::print")
        .withStringParameter("data", expectedFileContent.c_str());
    mock().ignoreOtherCalls();

    confStorage.setWifiConfig("some", "thing");
    auto state = confStorage.save(file);
    CHECK_TRUE(ConfStorage::State::OK == state);
}

TEST(ConfStorageTest, ShouldParseFileAndReadAdminCredentials)
{
    FileMock fileMock;
    ConfStorage confStorage;
    RaiiFile file(fileMock);

    std::string fileContent = R"({"admin": {"passwd":"dark","user":"tranquillity"}})";
    mock().expectOneCall("FileMock::readString").andReturnValue(&fileContent);
    mock().ignoreOtherCalls();

    auto state = confStorage.load(file);
    CHECK_TRUE(ConfStorage::State::OK == state);

    auto credentials = confStorage.getAdminCredentials();
    CHECK_TRUE(credentials.has_value());

    auto [user, pass] = credentials.value();
    CHECK_EQUAL(user, "tranquillity");
    CHECK_EQUAL(pass, "dark");
}

TEST(ConfStorageTest, ShouldSaveAdminCredentials)
{
    FileMock fileMock;
    ConfStorage confStorage;
    RaiiFile file(fileMock);

    std::string expectedFileContent = R"({"admin":{"pass":"in","user":"flames"}})";
    mock()
        .expectOneCall("FileMock::print")
        .withStringParameter("data", expectedFileContent.c_str());
    mock().ignoreOtherCalls();

    confStorage.setAdminCredentials("flames", "in");
    auto state = confStorage.save(file);
    CHECK_TRUE(ConfStorage::State::OK == state);
}

TEST(ConfStorageTest, ShouldAllowAddingSensorsIfThereIsEnoughSpace)
{
    ConfStorage confStorage;

    CHECK_TRUE(confStorage.isAvailableSpaceForNextSensor());

    CHECK_TRUE(confStorage.addSensor(1));
    CHECK_TRUE(confStorage.addSensor(2));
    CHECK_TRUE(confStorage.addSensor(3));
    CHECK_TRUE(confStorage.addSensor(4));
    CHECK_TRUE(confStorage.addSensor(5));
    CHECK_TRUE(confStorage.addSensor(6));
    CHECK_TRUE(confStorage.isAvailableSpaceForNextSensor());

    CHECK_TRUE(confStorage.addSensor(7));
    CHECK_FALSE(confStorage.isAvailableSpaceForNextSensor());

    CHECK_FALSE(confStorage.addSensor(8));
}

TEST(ConfStorageTest, ShouldReturnSensorsMapping)
{
    ConfStorage confStorage;

    confStorage.addSensor(1);
    confStorage.addSensor(2, "Aa");
    confStorage.addSensor(3, "Will be replaced");
    confStorage.addSensor(3, "replaced");
    confStorage.addSensor(4, "C name");
    confStorage.addSensor(5);
    confStorage.addSensor(6);
    confStorage.addSensor(7);
    confStorage.addSensor(8, "Will not be added as there is no enough space");

    auto expected = nlohmann::json({
        {"1", "Unnamed"},
        {"2", "Aa"},
        {"3", "replaced"},
        {"4", "C name"},
        {"5", "Unnamed"},
        {"6", "Unnamed"},
        {"7", "Unnamed"},
    });

    auto sensorsMapping = confStorage.getSensorsMapping();
    CHECK_TRUE(expected == sensorsMapping);
}

TEST(ConfStorageTest, ShouldBeAbleToRemoveSensor)
{
    ConfStorage confStorage;

    confStorage.addSensor(1);
    confStorage.addSensor(2, "Aa");

    auto isRemoved = confStorage.removeSensor(1);
    CHECK_TRUE(isRemoved);

    auto expected = nlohmann::json({
        {"2", "Aa"},
    });
    
    auto sensorsMapping = confStorage.getSensorsMapping();
    CHECK_TRUE(expected == sensorsMapping);
}

TEST(ConfStorageTest, ShouldNotRemoveSensorWithWrongIdentifier)
{
    ConfStorage confStorage;

    confStorage.addSensor(1);
    confStorage.addSensor(2, "Aa");

    auto isRemoved = confStorage.removeSensor(3);
    CHECK_FALSE(isRemoved);

    auto expected = nlohmann::json({
        {"1", "Unnamed"},
        {"2", "Aa"},
    });
    
    auto sensorsMapping = confStorage.getSensorsMapping();
    CHECK_TRUE(expected == sensorsMapping);
}

TEST(ConfStorageTest, CheckSimpleParameters)
{
    ConfStorage confStorage;

    confStorage.setSensorUpdatePeriodMins(123);
    CHECK_TRUE(confStorage.getSensorUpdatePeriodMins() == 123);
    confStorage.setServerPort(88);
    CHECK_TRUE(confStorage.getServerPort() == 88);
}

TEST(ConfStorageTest, ShouldReturnConfigWithoutCredentials)
{
    ConfStorage confStorage;
    confStorage.setDefault();

    confStorage.setWifiConfig("blabla", "test");
    confStorage.setAdminCredentials("asd", "fgh");

    auto expected = nlohmann::json{
        {"sensors", {}},
        {"serverPort", 80},
        {"sensorUpdatePeriodMins", 1},
    };

    auto configWithoutCred = confStorage.getConfigWithoutCredentials();
    CHECK_TRUE(expected == configWithoutCred);
}