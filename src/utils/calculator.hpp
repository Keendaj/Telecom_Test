#pragma once

#include "types.hpp"
#include "config.hpp"

/**
 * @brief Класс для расчета итоговой ценности ресурсов.
 * Инкапсулирует логику подсчета очков, учитывая базовые стоимости 
 * из Config и правило удвоения ценности для целевого ресурса.
 */
class ScoreCalculator {
public:
    /**
     * @brief Вычисляет общую ценность собранных ресурсов.
     * @param res Структура с общим количеством собранных ресурсов.
     * @param target Тип целевого ресурса (его ценность удваивается).
     * @return Общее количество очков (сумма).
     */
    static u32 calculateTotalValue(const Resources& res, ResourceType target) {
        u32 total = 0;
        
        auto getValue = [target](ResourceType type, u8 count) -> u32 {
            u8 base = Config::getResourceBaseValue(type);
            u8 multiplier = (type == target) ? 2 : 1;
            return static_cast<u32>(count) * base * multiplier;
        };

        total += getValue(ResourceType::IRON, res.iron);
        total += getValue(ResourceType::GOLD, res.gold);
        total += getValue(ResourceType::GEMS, res.gems);
        total += getValue(ResourceType::EXP,  res.exp);
        
        return total;
    }
};