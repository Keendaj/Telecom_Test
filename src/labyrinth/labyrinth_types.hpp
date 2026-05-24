#pragma once
#include "../utils/types.hpp"
#include <vector>


/**
 * @brief Перечисление доступных в подземелье типов ресурсов.
 */
enum class ResourceType {
    NONE,
    IRON,
    GOLD,
    GEMS,
    EXP
};

/**
 * @brief Структура для хранения количества различных ресурсов в комнате.
 */
struct Resources {
    u8 iron = 0; ///< Количество железа
    u8 gold = 0; ///< Количество золота
    u8 gems = 0; ///< Количество кристаллов
    u8 exp = 0;  ///< Количество опыта

    bool iron_collected = false; ///< Было ли собрано железо (для корректного вывода)
    bool gold_collected = false; ///< Было ли собрано золото (для корректного вывода)
    bool gems_collected = false; ///< Были ли собраны кристаллы (для корректного вывода)
    bool exp_collected  = false; ///< Был ли собран опыт (для корректного вывода)

    /**
     * @brief Изымает весь ресурс указанного типа.
     * Значение выбранного ресурса внутри структуры обнуляется.
     * @param type Тип собираемого ресурса.
     * @return Количество успешно собранного ресурса.
     */
    u8 collect(ResourceType type);
};

/**
 * @brief Структура, описывающая отдельную комнату в подземелье.
 */
struct Room {
    u8 id;                          ///< Уникальный идентификатор комнаты
    std::vector<u8> adjacent_rooms; ///< Список ID смежных комнат
    Resources resources;            ///< Запасы ресурсов в данной комнате

    /**
     * @brief Конструктор по умолчанию.
     */
    Room() = default;

    /**
     * @brief Конструктор комнаты с инициализацией базовых параметров.
     * @param id_ Уникальный идентификатор комнаты.
     * @param adjacent_rooms_ Список ID смежных комнат.
     */
    explicit Room(u8 id_, std::vector<u8> adjacent_rooms_) : id(id_), adjacent_rooms(adjacent_rooms_) {}
};
