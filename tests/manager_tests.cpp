#include "../src/manager/game_manager.hpp"
#include "../src/utils/calculator.hpp"
#include "../src/utils/types.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <vector>

class ScriptedBot : public IBot {
  public:
    std::vector<BotAction> script;
    size_t step = 0;

    BotAction getNextAction(const Room &, u8) override {
        if (step < script.size()) {
            return script[step++];
        }
        return {ActionType::FINISH, 0, ResourceType::NONE};
    }
};

// Проверка успешного прохождения, сбора и правильного логирования
TEST(GameManagerTest, LogsMatchFormatPerfectly) {
    Labyrinth lab(1);
    lab.addRoom(0, {1}, 0, 0, 0, 0);
    lab.addRoom(1, {0}, 10, 0, 0, 0);

    ScriptedBot bot;
    bot.script = {{ActionType::MOVE, 1, ResourceType::NONE},
                  {ActionType::COLLECT, 0, ResourceType::IRON},
                  {ActionType::MOVE, 0, ResourceType::NONE},
                  {ActionType::FINISH, 0, ResourceType::NONE}};

    GameManager manager(lab, bot, 10);
    std::ostringstream out;
    manager.run(out, ResourceType::NONE);

    std::string expected_out = "go 1\n"
                               "state 1 10 0 0 0\n"
                               "collect iron\n"
                               "state 1 _ 0 0 0\n"
                               "go 0\n"
                               "result 10 0 0 0 70\n";

    EXPECT_EQ(out.str(), expected_out);
}

// Проверка защиты от читерства
TEST(GameManagerTest, FailsOnCheatingMove) {
    Labyrinth lab(2);
    lab.addRoom(0, {1, 2}, 0, 0, 0, 0);
    lab.addRoom(1, {0}, 0, 0, 0, 0);
    lab.addRoom(2, {0}, 0, 0, 0, 0);

    ScriptedBot bot;
    bot.script = {{ActionType::MOVE, 1, ResourceType::NONE},
                  {ActionType::MOVE, 2, ResourceType::NONE}};

    GameManager manager(lab, bot, 10);
    std::ostringstream out;
    SimulationResult res = manager.run(out, ResourceType::NONE);

    EXPECT_FALSE(res.success);
}

// Проверка смерти от голода при перемещении
TEST(GameManagerTest, DiesFromStarvationOnMove) {
    Labyrinth lab(2);
    lab.addRoom(0, {1}, 0, 0, 0, 0);
    lab.addRoom(1, {0, 2}, 0, 0, 0, 0);
    lab.addRoom(2, {1}, 0, 0, 0, 0);

    ScriptedBot bot;
    bot.script = {{ActionType::MOVE, 1, ResourceType::NONE},
                  {ActionType::MOVE, 2, ResourceType::NONE}};

    GameManager manager(lab, bot, 1);
    std::ostringstream out;
    SimulationResult res = manager.run(out, ResourceType::NONE);

    EXPECT_FALSE(res.success);
}

// Проверка смерти от голода при платном сборе ресурсов
TEST(GameManagerTest, DiesFromStarvationOnPaidCollect) {
    Labyrinth lab(1);
    lab.addRoom(0, {1}, 0, 0, 0, 0);
    lab.addRoom(1, {0}, 10, 10, 0, 0);

    ScriptedBot bot;
    bot.script = {{ActionType::MOVE, 1, ResourceType::NONE},
                  {ActionType::COLLECT, 0, ResourceType::IRON},
                  {ActionType::COLLECT, 0, ResourceType::GOLD}};

    GameManager manager(lab, bot, 1);
    std::ostringstream out;
    SimulationResult res = manager.run(out, ResourceType::NONE);

    EXPECT_FALSE(res.success);
}