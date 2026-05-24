#include "../src/labyrinth/labyrinth.hpp"
#include "../src/utils/types.hpp"
#include <gtest/gtest.h>

// Успешное добавление и получение стандартной комнаты
TEST(LabyrinthTest, AddAndGetRoomSuccess) {
    Labyrinth lab(1);
    lab.addRoom(1, {0}, 10, 5, 2, 100);

    const auto &room = lab.getRoom(1);
    EXPECT_EQ(room.id, 1);
    EXPECT_EQ(room.adjacent_rooms.size(), 1);
    EXPECT_EQ(room.adjacent_rooms[0], 0);
    EXPECT_EQ(room.resources.iron, 10);
    EXPECT_EQ(room.resources.gold, 5);
}

// Игнорирование ресурсов в нулевой (стартовой) комнате
TEST(LabyrinthTest, AddRoomZeroIgnoresResources) {
    Labyrinth lab(1);
    lab.addRoom(0, {1}, 50, 50, 50, 50);

    const auto &room0 = lab.getRoom(0);
    EXPECT_EQ(room0.id, 0);
    EXPECT_EQ(room0.resources.iron, 0);
    EXPECT_EQ(room0.resources.gold, 0);
    EXPECT_EQ(room0.resources.gems, 0);
    EXPECT_EQ(room0.resources.exp, 0);
}

// Выброс исключения при попытке получить несуществующую комнату
TEST(LabyrinthTest, GetNonExistentRoomThrows) {
    Labyrinth lab(1);
    lab.addRoom(0, {}, 0, 0, 0, 0);

    EXPECT_THROW(lab.getRoom(99), std::out_of_range);
}

// Успешная проверка правильного двунаправленного графа
TEST(LabyrinthTest, ValidationSuccessOnBidirectionalGraph) {
    Labyrinth lab(2);
    lab.addRoom(0, {1, 2}, 0, 0, 0, 0);
    lab.addRoom(1, {0}, 10, 0, 0, 0);
    lab.addRoom(2, {0}, 0, 10, 0, 0);

    EXPECT_TRUE(lab.validate());
}

// Ошибка при односторонней связи
TEST(LabyrinthTest, ValidationFailsOnUnidirectionalLink) {
    Labyrinth lab(2);
    lab.addRoom(0, {1}, 0, 0, 0, 0);
    lab.addRoom(1, {}, 10, 0, 0, 0);

    EXPECT_FALSE(lab.validate());
}

// Ошибка , если комната ссылается на несуществующий ID
TEST(LabyrinthTest, ValidationFailsOnNonExistentAdjacentRoom) {
    Labyrinth lab(1);
    lab.addRoom(0, {99}, 0, 0, 0, 0);

    EXPECT_FALSE(lab.validate());
}

// Подтверждение существующего перехода
TEST(LabyrinthTest, HasConnectionReturnsTrueForValidLink) {
    Labyrinth lab(2);
    lab.addRoom(0, {1, 2}, 0, 0, 0, 0);

    EXPECT_TRUE(lab.hasConnection(0, 1));
    EXPECT_TRUE(lab.hasConnection(0, 2));
}

// Отказ для отсутствующего перехода или несуществующей комнаты
TEST(LabyrinthTest, HasConnectionReturnsFalseForInvalidLink) {
    Labyrinth lab(3);
    lab.addRoom(0, {1}, 0, 0, 0, 0);
    lab.addRoom(1, {0}, 0, 0, 0, 0);

    EXPECT_FALSE(lab.hasConnection(0, 2));
    EXPECT_FALSE(lab.hasConnection(99, 1));
}

// Успешный сбор разных типов ресурсов
TEST(LabyrinthTest, CollectDifferentResourceTypes) {
    Labyrinth lab(1);
    lab.addRoom(1, {0}, 10, 20, 30, 40);

    EXPECT_EQ(lab.collectResource(1, ResourceType::GOLD), 20);
    EXPECT_EQ(lab.collectResource(1, ResourceType::GEMS), 30);
    EXPECT_EQ(lab.collectResource(1, ResourceType::EXP), 40);
}

// Повторный сбор возвращает 0 (ресурс исчерпан)
TEST(LabyrinthTest, CollectResourceDepletesIt) {
    Labyrinth lab(1);
    lab.addRoom(1, {0}, 50, 0, 0, 0);

    u8 first_collect = lab.collectResource(1, ResourceType::IRON);
    EXPECT_EQ(first_collect, 50);

    u8 second_collect = lab.collectResource(1, ResourceType::IRON);
    EXPECT_EQ(second_collect, 0);
}

// Сбор из несуществующей комнаты безопасно возвращает 0
TEST(LabyrinthTest, CollectFromNonExistentRoomReturnsZero) {
    Labyrinth lab(1);
    EXPECT_EQ(lab.collectResource(99, ResourceType::IRON), 0);
}

// Проверка правильного сохранения N
TEST(LabyrinthTest, GetRoomsCountReturnsCorrectValue) {
    Labyrinth lab1(5);
    EXPECT_EQ(lab1.getRoomsCount(), 5);

    Labyrinth lab2(255);
    EXPECT_EQ(lab2.getRoomsCount(), 255);
}