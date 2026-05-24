#pragma once

#include "../labyrinth/labyrinth.hpp"
#include "../bot/bot.hpp"
#include "../utils/types.hpp"
#include <vector>
#include <iostream>

/**
 * @brief Итоговый отчет о прохождении подземелья.
 */
struct SimulationResult {
    bool success = false;                 ///< Успешное возвращение в старт
    Resources collected_resources;        ///< Сумма всех собранных ресурсов
};

/**
 * @brief Менеджер игры, отвечающий за выполнение правил симуляции.
 */
class GameManager {
public:
    /**
     * @brief Инициализирует менеджер.
     * @param labyrinth Ссылка на граф лабиринта.
     * @param bot Ссылка на алгоритм бота.
     * @param initial_food Стартовое количество еды.
     */
    GameManager(Labyrinth& labyrinth, IBot& bot, u8 initial_food);

    /**
     * @brief Запускает цикл симуляции с логированием в поток.
     * @param out Поток для вывода логов (result.txt).
     * @param target Тип целевого ресурса (для расчета итоговой ценности).
     * @return SimulationResult с собранными ресурсами.
     */
    SimulationResult run(std::ostream& out, ResourceType target);

private:
    Labyrinth& _labyrinth;
    IBot& _bot;
    u8 _current_food;
    u8 _current_room_id;

    /**
     * @brief Вспомогательный метод для логирования статуса комнаты (state).
     */
    void logState(std::ostream& out, u8 room_id);
};