#pragma once

#include "bot.hpp"
#include <unordered_map>
#include <unordered_set>
#include <optional>

/**
 * @brief Класс, реализующий базовый алгоритм для прохождения подземелья.
 */
class RegularBot : public IBot {
public:
    /**
     * @brief Конструктор базового бота.
     * @param target Указывает целевой ресурс.
     */
    explicit RegularBot(ResourceType target);

    /**
     * @brief Принимает решение о следующем действии, обновляя внутреннюю память.
     * @param current_room Данные текущей комнаты.
     * @param current_food Оставшееся количество еды.
     * @return Выбранное действие.
     */
    BotAction getNextAction(const Room& current_room, u8 current_food) override;

private:
    /**
     * @brief Возможные фазы работы алгоритма.
     */
    enum class Phase {
        EXPLORE, ///< Фаза исследования: бот ищет ресурсы и открывает новые комнаты.
        RETURN   ///< Фаза возвращения: бот кратчайшим путем идет в стартовую комнату.
    };

    Phase current_phase = Phase::EXPLORE; ///< Текущая активная фаза
    ResourceType target_resource;         ///< Целевой ресурс миссии

    std::unordered_map<u8, Room> memory;         ///< Память бота: сохраняет информацию обо всех увиденных комнатах
    std::unordered_set<u8> visited_rooms;        ///< ID комнат, в которые бот заходил физически
    std::unordered_map<u8, bool> free_collect;   ///< Флаг бесплатного сбора ресурса (при первом посещении)

    /**
     * @brief Выполняет логику фазы исследования.
     * @param current_room Текущая комната.
     * @param current_food Оставшаяся еда.
     * @return Действие для фазы EXPLORE.
     */
    BotAction handleExplorePhase(const Room& current_room, u8 current_food);

    /**
     * @brief Выполняет логику фазы возвращения домой.
     * @param current_room Текущая комната.
     * @param current_food Оставшаяся еда.
     * @return Действие для фазы RETURN.
     */
    BotAction handleReturnPhase(const Room& current_room, u8 current_food);
    
    /**
     * @brief Ищет первый шаг на кратчайшем пути к ближайшей непосещенной комнате.
     * @param start_id ID комнаты, в которой находится бот.
     * @return std::optional<u8> с ID комнаты, либо std::nullopt, если непосещенных комнат нет.
     */
    std::optional<u8> getNextStepToNearestUnvisited(u8 start_id);

    /**
     * @brief Ищет следующий шаг для возвращения домой (в нулевую комнату) по градиенту BFS.
     * @param current_id ID комнаты, в которой находится бот.
     * @return ID смежной комнаты для следующего шага.
     */
    u8 getNextStepToHome(u8 current_id);

    /**
     * @brief Рассчитывает точную стоимость пути до дома в единицах еды.
     * @param current_id ID комнаты, в которой находится бот.
     * @return Требуемое количество еды для возврата.
     */
    u8 calculateCostToHome(u8 current_id);

    /**
     * @brief Выбирает самый ценный ресурс в комнате с учетом х2 множителя.
     * @param res Доступные в комнате ресурсы.
     * @return Тип самого ценного ресурса или NONE, если комната пуста.
     */
    ResourceType getBestResourceInRoom(const Resources& res) const;
};