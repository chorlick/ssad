#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "config.h"

using namespace std;

class ConfigTest : public ::testing::Test {
protected:
    string temp_file;

    void SetUp() override {
        temp_file = "test_config.json";
    }

    void TearDown() override {
        if (std::filesystem::exists(temp_file)) {
            std::filesystem::remove(temp_file);
        }
    }

    void write_config(const string& content) {
        ofstream out(temp_file);
        out << content;
        out.close();
    }
};

TEST_F(ConfigTest, ValidConfigLoadsSuccessfully) {
    string config_json = R"({
        "trigger_ports": [1001, 1002, 1003],
        "sequence_timeout_ms": 5000,
        "inter_knock_timeout_ms": 1000,
        "activation_log_file": "/tmp/ssad.log"
    })";
    write_config(config_json);

    Config config;
    EXPECT_TRUE(config.load(temp_file));
    EXPECT_EQ(config.trigger_ports.size(), 3);
    EXPECT_EQ(config.sequence_timeout_ms, 5000);
    EXPECT_EQ(config.inter_knock_timeout_ms, 1000);
    EXPECT_EQ(config.log_file, "/tmp/ssad.log");
}

TEST_F(ConfigTest, MissingFileReturnsFalse) {
    Config config;
    EXPECT_FALSE(config.load("non_existent.json"));
}

TEST_F(ConfigTest, MissingTriggerPortsReturnsFalse) {
    string config_json = R"({
        "sequence_timeout_ms": 5000,
        "inter_knock_timeout_ms": 1000,
        "activation_log_file": "/tmp/ssad.log"
    })";
    write_config(config_json);

    Config config;
    EXPECT_FALSE(config.load(temp_file));
}

TEST_F(ConfigTest, InvalidPortCountReturnsFalse) {
    string config_json = R"({
        "trigger_ports": [1001],
        "sequence_timeout_ms": 5000,
        "inter_knock_timeout_ms": 1000,
        "activation_log_file": "/tmp/ssad.log"
    })";
    write_config(config_json);

    Config config;
    EXPECT_FALSE(config.load(temp_file));
}

TEST_F(ConfigTest, InvalidPortRangeReturnsFalse) {
    string config_json = R"({
        "trigger_ports": [0, 65536, 80],
        "sequence_timeout_ms": 5000,
        "inter_knock_timeout_ms": 1000,
        "activation_log_file": "/tmp/ssad.log"
    })";
    write_config(config_json);

    Config config;
    EXPECT_FALSE(config.load(temp_file));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}