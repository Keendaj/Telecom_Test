#include "game_manager.hpp"
#include "../utils/calculator.hpp"
#include <unordered_set>

GameManager::GameManager(Labyrinth& labyrinth, IBot& bot, u8 initial_food) 
    : _labyrinth(labyrinth), _bot(bot), _current_food(initial_food), _current_room_id(0) {}

void GameManager::logState(std::ostream& out, u8 room_id) {
    const Room& current_room = _labyrinth.getRoom(room_id);
    const Resources& res = current_room.resources;
    
    out << "state " << static_cast<int>(room_id)
        << " " << (res.iron_collected ? "_" : std::to_string(static_cast<int>(res.iron)))
        << " " << (res.gold_collected ? "_" : std::to_string(static_cast<int>(res.gold)))
        << " " << (res.gems_collected ? "_" : std::to_string(static_cast<int>(res.gems)))
        << " " << (res.exp_collected  ? "_" : std::to_string(static_cast<int>(res.exp)))
        << std::endl;
}

SimulationResult GameManager::run(std::ostream& out, ResourceType target) {
    SimulationResult result;
    
    std::unordered_set<u8> free_collect_used;

    while (true) {
        Room current_room = _labyrinth.getRoom(_current_room_id);
        BotAction action = _bot.getNextAction(current_room, _current_food);

        if (action.type == ActionType::FINISH) {
            result.success = (_current_room_id == 0);

            if (result.success) {
                u32 total_value = ScoreCalculator::calculateTotalValue(result.collected_resources, target);
                out << "result " 
                    << static_cast<int>(result.collected_resources.iron) << " "
                    << static_cast<int>(result.collected_resources.gold) << " "
                    << static_cast<int>(result.collected_resources.gems) << " "
                    << static_cast<int>(result.collected_resources.exp) << " "
                    << total_value << std::endl;
            }

            break;
        }

        if (action.type == ActionType::MOVE) {
            if (!_labyrinth.hasConnection(_current_room_id, action.target_id)) {
                result.success = false; 
                break;
            }
            
            if (_current_food == 0) {
                result.success = false;
                break;
            }
            
            _current_food--;
            _current_room_id = action.target_id;
            
            out << "go " << static_cast<int>(_current_room_id) << std::endl;
            
            if (_current_room_id != 0) {
                logState(out, _current_room_id);
            }
            
        } 
        else if (action.type == ActionType::COLLECT) {
            bool is_free = (free_collect_used.find(_current_room_id) == free_collect_used.end());
            
            if (!is_free) {
                if (_current_food == 0) {
                    result.success = false;
                    break; 
                }
                _current_food--;
            } 
            else {
                free_collect_used.insert(_current_room_id);
            }

            u8 collected_amount = _labyrinth.collectResource(_current_room_id, action.resource);
            
            std::string res_name;
            switch (action.resource) {
                case ResourceType::IRON: 
                    result.collected_resources.iron += collected_amount; 
                    res_name = "iron";
                    break;
                case ResourceType::GOLD: 
                    result.collected_resources.gold += collected_amount; 
                    res_name = "gold";
                    break;
                case ResourceType::GEMS: 
                    result.collected_resources.gems += collected_amount; 
                    res_name = "gems";
                    break;
                case ResourceType::EXP:  
                    result.collected_resources.exp  += collected_amount; 
                    res_name = "exp";
                    break;
                default: 
                    break;
            }

            out << "collect " << res_name << std::endl;
            
            if (_current_room_id != 0) {
                logState(out, _current_room_id);
            }
        }
    }

    return result;
}