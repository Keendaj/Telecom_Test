#pragma once

#include "../labyrinth/labyrinth_types.hpp"

/**
 * @brief Глобальные настройки и константы симуляции.
 */
namespace Config {

constexpr u8 IRON_BASE_VALUE = 7;
constexpr u8 GOLD_BASE_VALUE = 11;
constexpr u8 GEMS_BASE_VALUE = 23;
constexpr u8 EXP_BASE_VALUE = 1;

/**
 * @brief Возвращает базовую ценность для указанного типа ресурса.
 * @param type Тип ресурса.
 * @return Ценность ресурса
 */
constexpr u8 getResourceBaseValue(ResourceType type) {
    switch (type) {
    case ResourceType::IRON:
        return IRON_BASE_VALUE;
    case ResourceType::GOLD:
        return GOLD_BASE_VALUE;
    case ResourceType::GEMS:
        return GEMS_BASE_VALUE;
    case ResourceType::EXP:
        return EXP_BASE_VALUE;
    default:
        return 0;
    }
}

} // namespace Config