#include <CppUTest/TestHarness.h>

#include "ConfStorage.hpp"
#include "mocks/FileSystem32AdpMock.hpp"
#include "mocks/RaiiFileMock.hpp"

// clang-format off
TEST_GROUP(ConfStorageTest)  // NOLINT
{
    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }
};
// clang-format on

TEST(ConfStorageTest, ShouldLoadCorrectConfiguration)  // NOLINT
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "/config.json");
    auto file = std::make_unique<RaiiFileMock>();

    std::string fileContent = R"({"user":"admin"})";
    mock("RaiiFileMock").expectOneCall("readString").andReturnValue(fileContent.c_str());
    mock("FileSystem32AdpMock")
        .expectOneCall("open")
        .withStringParameter("path", "/config.json")
        .withParameter("mode", static_cast<int>(IFileSystem32Adp::Mode::F_READ))
        .andReturnValue(file.release());

    auto state = confStorage.load();
    CHECK_TRUE(ConfStorage::State::OK == state);
}

TEST(ConfStorageTest, ShouldntLoadIncorrectConfiguration)  // NOLINT
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "/config.json");
    auto file = std::make_unique<RaiiFileMock>();

    std::string fileContent = R"(brokenjsonfile:"admin"})";
    mock("RaiiFileMock").expectOneCall("readString").andReturnValue(fileContent.c_str());
    mock("FileSystem32AdpMock")
        .expectOneCall("open")
        .ignoreOtherParameters()
        .andReturnValue(file.release());

    auto state = confStorage.load();
    CHECK_TRUE(ConfStorage::State::FAIL == state);
}

TEST(ConfStorageTest, ShouldSaveDefaultConfiguration)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "/config.json");
    auto file = std::make_unique<RaiiFileMock>();

    std::string expectedStringToSave
        = R"({"admin":{"pass":"admin","user":"admin"},"sensorUpdatePeriodMins":1,"sensors":null,"serverPort":80})";

    mock("RaiiFileMock").expectOneCall("print").withParameter("str", expectedStringToSave.c_str());
    mock("FileSystem32AdpMock")
        .expectOneCall("open")
        .withStringParameter("path", "/config.json")
        .withParameter("mode", static_cast<int>(IFileSystem32Adp::Mode::F_WRITE))
        .andReturnValue(file.release());

    confStorage.setDefault();
    auto state = confStorage.save();
    CHECK_TRUE(ConfStorage::State::OK == state);
}

TEST(ConfStorageTest, ShouldParseFileAndReadWifiConfig)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "/config.json");
    auto file = std::make_unique<RaiiFileMock>();

    std::string fileContent = R"({"wifi":{"ssid": "test", "pass": "testPass"}})";
    mock("RaiiFileMock").expectOneCall("readString").andReturnValue(fileContent.c_str());
    mock("FileSystem32AdpMock")
        .expectOneCall("open")
        .ignoreOtherParameters()
        .andReturnValue(file.release());

    auto state = confStorage.load();
    CHECK_TRUE(ConfStorage::State::OK == state);

    auto wifiConfig = confStorage.getWifiConfig();
    CHECK_TRUE(wifiConfig.has_value());

    auto [ssid, pass] = wifiConfig.value();
    CHECK_EQUAL(ssid, "test");
    CHECK_EQUAL(pass, "testPass");
}

TEST(ConfStorageTest, ShouldReturnNulloptWhenWifiConfigIsIncompleteOrWrong)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "/config.json");
    auto file = std::make_unique<RaiiFileMock>();

    std::string fileContent = R"({"wifi":{"pass": "testPass"}})";
    mock("RaiiFileMock").expectOneCall("readString").andReturnValue(fileContent.c_str());
    mock("FileSystem32AdpMock")
        .expectOneCall("open")
        .ignoreOtherParameters()
        .andReturnValue(file.release());

    auto state = confStorage.load();
    CHECK_TRUE(ConfStorage::State::OK == state);

    auto wifiConfig = confStorage.getWifiConfig();
    CHECK_FALSE(wifiConfig.has_value());
}

TEST(ConfStorageTest, ShouldSaveWifiConfiguration)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "/config.json");
    auto file = std::make_unique<RaiiFileMock>();

    std::string expectedFileContent
        = R"({"admin":{"pass":"admin","user":"admin"},"sensorUpdatePeriodMins":1,"sensors":null,"serverPort":80,"wifi":{"pass":"thing","ssid":"some"}})";
    auto expectedWifiJsonContent = nlohmann::json::parse(expectedFileContent)["wifi"];

    mock("RaiiFileMock").expectOneCall("print").withParameter("str", expectedFileContent.c_str());
    mock("FileSystem32AdpMock")
        .expectOneCall("open")
        .ignoreOtherParameters()
        .andReturnValue(file.release());

    confStorage.setWifiConfig("some", "thing");
    auto state = confStorage.save();

    CHECK_TRUE(ConfStorage::State::OK == state);
}

TEST(ConfStorageTest, ShouldParseFileAndReadAdminCredentials)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "/config.json");
    auto file = std::make_unique<RaiiFileMock>();

    std::string fileContent = R"({"admin": {"pass":"dark","user":"tranquillity"}})";
    mock("RaiiFileMock").expectOneCall("readString").andReturnValue(fileContent.c_str());
    mock("FileSystem32AdpMock")
        .expectOneCall("open")
        .ignoreOtherParameters()
        .andReturnValue(file.release());

    auto state = confStorage.load();
    CHECK_TRUE(ConfStorage::State::OK == state);

    auto credentials = confStorage.getAdminCredentials();
    CHECK_TRUE(credentials.has_value());

    auto [user, pass] = credentials.value();
    CHECK_EQUAL(user, "tranquillity");
    CHECK_EQUAL(pass, "dark");
}

TEST(ConfStorageTest, ShouldSaveAdminCredentials)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "/config.json");
    auto file = std::make_unique<RaiiFileMock>();

    std::string expectedFileContent
        = R"({"admin":{"pass":"in","user":"flames"},"sensorUpdatePeriodMins":1,"sensors":null,"serverPort":80})";

    mock("RaiiFileMock").expectOneCall("print").withParameter("str", expectedFileContent.c_str());
    mock("FileSystem32AdpMock")
        .expectOneCall("open")
        .ignoreOtherParameters()
        .andReturnValue(file.release());

    confStorage.setAdminCredentials("flames", "in");
    auto state = confStorage.save();
    CHECK_TRUE(ConfStorage::State::OK == state);
}

TEST(ConfStorageTest, ShouldAllowAddingSensorsIfThereIsEnoughSpace)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "NotImportantInThisTest");

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
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "NotImportantInThisTest");

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
        {"1", "Unnamed 1"},
        {"2", "Aa"},
        {"3", "replaced"},
        {"4", "C name"},
        {"5", "Unnamed 2"},
        {"6", "Unnamed 3"},
        {"7", "Unnamed 4"},
    });

    auto sensorsMapping = confStorage.getSensorsMapping();
    CHECK_TRUE(expected.dump() == sensorsMapping);
}

TEST(ConfStorageTest, ShouldBeAbleToRemoveSensor)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "NotImportantInThisTest");

    confStorage.addSensor(1);
    confStorage.addSensor(2, "Aa");

    auto isRemoved = confStorage.removeSensor(1);
    CHECK_TRUE(isRemoved);

    auto expected = nlohmann::json({
        {"2", "Aa"},
    });

    auto sensorsMapping = confStorage.getSensorsMapping();
    CHECK_TRUE(expected.dump() == sensorsMapping);
}

TEST(ConfStorageTest, ShouldNotRemoveSensorWithWrongIdentifier)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "NotImportantInThisTest");

    confStorage.addSensor(1);
    confStorage.addSensor(2, "Aa");

    auto isRemoved = confStorage.removeSensor(3);
    CHECK_FALSE(isRemoved);

    auto expected = nlohmann::json({
        {"1", "Unnamed 1"},
        {"2", "Aa"},
    });

    auto sensorsMapping = confStorage.getSensorsMapping();
    CHECK_TRUE(expected.dump() == sensorsMapping);
}

TEST(ConfStorageTest, CheckSimpleParameters)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "NotImportantInThisTest");

    confStorage.setSensorUpdatePeriodMins(123);
    CHECK_TRUE(confStorage.getSensorUpdatePeriodMins() == 123);
    confStorage.setServerPort(88);
    CHECK_TRUE(confStorage.getServerPort() == 88);
}

TEST(ConfStorageTest, ShouldReturnConfigWithoutCredentials)
{
    auto fileSystemMock = std::make_shared<FileSystem32AdpMock>();
    ConfStorage confStorage(fileSystemMock, "NotImportantInThisTest");

    confStorage.setDefault();
    confStorage.setWifiConfig("blabla", "test");
    confStorage.setAdminCredentials("asd", "fgh");

    auto expected = nlohmann::json{
        {"sensors", {}},
        {"serverPort", 80},
        {"sensorUpdatePeriodMins", 1},
    };

    auto configWithoutCred = confStorage.getConfigWithoutCredentials();
    CHECK_TRUE(expected.dump() == configWithoutCred);
}
