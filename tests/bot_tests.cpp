#include "../src/bot/regular_bot.hpp"
#include "../src/utils/config.hpp"
#include "../src/utils/types.hpp"
#include <gtest/gtest.h>

// Проверка сбора приоритетного ресурса
TEST(RegularBotTest, CollectsBestResourceFirst) {
    RegularBot bot(ResourceType::IRON);

    Room start_room(0, {1});
    Room room1(1, {0});

    room1.resources.iron = 10;
    room1.resources.gold = 10;

    bot.getNextAction(start_room, 100);

    BotAction action = bot.getNextAction(room1, 99);

    EXPECT_EQ(action.type, ActionType::COLLECT);
    EXPECT_EQ(action.resource, ResourceType::IRON);
}

// Проверка выбора комнаты
TEST(RegularBotTest, MovesToSmallestUnvisitedAdjacent) {
    RegularBot bot(ResourceType::NONE);

    Room start_room(0, {1});
    Room room1(1, {0, 4, 2});

    bot.getNextAction(start_room, 100);
    BotAction action = bot.getNextAction(room1, 99);

    EXPECT_EQ(action.type, ActionType::MOVE);
    EXPECT_EQ(action.target_id, 2);
}

// Проверка переключения фазы при нехватке еды
TEST(RegularBotTest, SwitchesToReturnPhaseOnLowFood) {
    RegularBot bot(ResourceType::NONE);

    Room start_room(0, {1});
    Room room1(1, {0, 2});
    Room room2(2, {1, 3});

    bot.getNextAction(start_room, 10);
    bot.getNextAction(room1, 9);

    BotAction action = bot.getNextAction(room2, 3);

    EXPECT_EQ(action.type, ActionType::MOVE);
    EXPECT_EQ(action.target_id, 1);
}

// Проверка завершения исследования
TEST(RegularBotTest, ReturnsHomeWhenAllVisited) {
    RegularBot bot(ResourceType::NONE);

    Room start_room(0, {1});
    Room room1(1, {0});

    bot.getNextAction(start_room, 100);

    BotAction action = bot.getNextAction(room1, 99);
    EXPECT_EQ(action.type, ActionType::MOVE);
    EXPECT_EQ(action.target_id, 0);
}

// Проверка сбора ресурсов на обратном пути
TEST(RegularBotTest, SpendsExcessFoodOnReturn) {
    RegularBot bot(ResourceType::NONE);

    Room start_room(0, {1});
    Room room1(1, {0, 2});
    room1.resources.gems = 10;
    Room room2(2, {1});

    bot.getNextAction(start_room, 10);
    bot.getNextAction(room1, 9);

    bot.getNextAction(room2, 3);

    BotAction action = bot.getNextAction(room1, 10);

    EXPECT_EQ(action.type, ActionType::COLLECT);
    EXPECT_EQ(action.resource, ResourceType::GEMS);
}

// Проверка выбора наименьшего Id
TEST(RegularBotTest, ReturnPhaseChoosesSmallestIdOnFork) {
    RegularBot bot(ResourceType::NONE);

    Room start_room(0, {1, 2});
    Room room1(1, {0, 3});
    Room room2(2, {0, 3});
    Room room3(3, {1, 2});

    bot.getNextAction(start_room, 100);
    bot.getNextAction(room1, 99);
    bot.getNextAction(room2, 98);
    bot.getNextAction(room3, 97);

    BotAction action = bot.getNextAction(room3, 2);

    EXPECT_EQ(action.type, ActionType::MOVE);
    EXPECT_EQ(action.target_id, 1);
}

// Проверка успешного завершения
TEST(RegularBotTest, FinishesWhenHome) {
    RegularBot bot(ResourceType::NONE);
    Room start_room(0, {1});
    Room room1(1, {0});

    bot.getNextAction(start_room, 10);
    bot.getNextAction(room1, 2);

    BotAction action = bot.getNextAction(start_room, 1);

    EXPECT_EQ(action.type, ActionType::FINISH);
}

// Поиск пути к непосещенной комнате из тупика
TEST(RegularBotTest, FindsPathToNearestUnvisited) {
    RegularBot bot(ResourceType::NONE);

    Room start_room(0, {1});
    Room room1(1, {0, 2, 3});
    Room room2(2, {1});
    Room room3(3, {1});

    bot.getNextAction(start_room, 100);

    bot.getNextAction(room1, 99);

    BotAction action = bot.getNextAction(room2, 98);
    EXPECT_EQ(action.type, ActionType::MOVE);
    EXPECT_EQ(action.target_id, 1);

    action = bot.getNextAction(room1, 97);
    EXPECT_EQ(action.type, ActionType::MOVE);
    EXPECT_EQ(action.target_id, 3);
}