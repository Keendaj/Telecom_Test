#pragma once

#include "../labyrinth/labyrinth_types.hpp"
#include "../utils/types.hpp"

/**
 * @brief Типы действий, которые бот может вернуть симулятору.
 */
enum class ActionType {
    MOVE,    ///< Перемещение в смежную комнату.
    COLLECT, ///< Сбор ресурса в текущей комнате.
    FINISH   ///< Завершение симуляции (бот успешно вернулся на старт).
};

/**
 * @brief Структура, описывающая выбранное ботом действие на текущем шаге.
 */
struct BotAction {
    ActionType type; ///< Тип выполняемого действия.
    u8 target_id =
        0; ///< ID целевой комнаты (используется только при type == MOVE).
    ResourceType resource =
        ResourceType::NONE; ///< Тип собираемого ресурса (используется только
                            ///< при type == COLLECT).
};

/**
 * @brief Абстрактный интерфейс для любого алгоритма бота.
 */
class IBot {
  public:
    virtual ~IBot() = default;

    /**
     * @brief Запрашивает у бота следующее действие.
     * @param current_room Данные о комнате, в которой бот находится прямо
     * сейчас.
     * @param current_food Текущий запас еды .
     * @return Действие (BotAction), которое бот хочет совершить.
     */
    virtual BotAction getNextAction(const Room &current_room,
                                    u8 current_food) = 0;
};