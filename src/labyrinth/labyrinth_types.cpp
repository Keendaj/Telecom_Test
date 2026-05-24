#include "labyrinth_types.hpp"

u8 Resources::collect(ResourceType type) {
    u8 amount = 0;
    switch (type) {
    case ResourceType::IRON:
        amount = iron;
        iron = 0;
        iron_collected = true;
        break;
    case ResourceType::GOLD:
        amount = gold;
        gold = 0;
        gold_collected = true;
        break;
    case ResourceType::GEMS:
        amount = gems;
        gems = 0;
        gems_collected = true;
        break;
    case ResourceType::EXP:
        amount = exp;
        exp = 0;
        exp_collected = true;
        break;
    default:
        break;
    }
    return amount;
}