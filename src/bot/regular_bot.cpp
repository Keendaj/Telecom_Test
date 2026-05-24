#include "../utils/config.hpp"
#include "regular_bot.hpp"

#include <queue>
#include <algorithm>
#include <vector>

RegularBot::RegularBot(ResourceType target) : target_resource(target) {}

BotAction RegularBot::getNextAction(const Room& current_room, u8 current_food) {
    memory[current_room.id] = current_room;
    
    if (visited_rooms.count(current_room.id) == 0) {
        visited_rooms.insert(current_room.id);
        free_collect[current_room.id] = true;
    }
    
    if (!is_initial_food_set) {
        initial_food = current_food;
        is_initial_food_set = true;
    }

    if (current_phase == Phase::EXPLORE) {
        if (current_food <= initial_food / 2) {
            current_phase = Phase::RETURN;
        }
    }

    if (current_phase == Phase::EXPLORE) {
        return handleExplorePhase(current_room, current_food);
    } 
    else {
        return handleReturnPhase(current_room, current_food);
    }
}


BotAction RegularBot::handleExplorePhase(const Room& current_room, u8 current_food) {
    ResourceType best_res = getBestResourceInRoom(current_room.resources);
    
    if (best_res != ResourceType::NONE && free_collect[current_room.id]) {
        free_collect[current_room.id] = false;
        return {ActionType::COLLECT, 0, best_res};
    }

    std::optional<u8> best_adj = std::nullopt;
    for (u8 adj : current_room.adjacent_rooms) {
        if (visited_rooms.count(adj) == 0) {
            if (!best_adj.has_value() || adj < best_adj.value()) {
                best_adj = adj;
            }
        }
    }

    if (best_adj.has_value()) {
        return {ActionType::MOVE, best_adj.value(), ResourceType::NONE};
    }

    std::optional<u8> next_step = getNextStepToNearestUnvisited(current_room.id);
    if (next_step.has_value()) {
        return {ActionType::MOVE, next_step.value(), ResourceType::NONE};
    }

    current_phase = Phase::RETURN;
    return handleReturnPhase(current_room, current_food);
}

BotAction RegularBot::handleReturnPhase(const Room& current_room, u8 current_food) {
    if (current_room.id == 0) {
        return {ActionType::FINISH, 0, ResourceType::NONE};
    }

    u8 cost_to_home = calculateCostToHome(current_room.id);

    bool is_free = free_collect[current_room.id];
    u8 collect_cost = is_free ? 0 : 1;

    if (current_food >= cost_to_home + collect_cost) {
        ResourceType best_res = getBestResourceInRoom(current_room.resources);
        if (best_res != ResourceType::NONE) {
            free_collect[current_room.id] = false; 
            return {ActionType::COLLECT, 0, best_res};
        }
    }

    u8 next_step = getNextStepToHome(current_room.id);
    return {ActionType::MOVE, next_step, ResourceType::NONE};
}
ResourceType RegularBot::getBestResourceInRoom(const Resources& res) const {
    u8 val_iron = Config::getResourceBaseValue(ResourceType::IRON) * (target_resource == ResourceType::IRON ? 2 : 1);
    u8 val_gold = Config::getResourceBaseValue(ResourceType::GOLD) * (target_resource == ResourceType::GOLD ? 2 : 1);
    u8 val_gems = Config::getResourceBaseValue(ResourceType::GEMS) * (target_resource == ResourceType::GEMS ? 2 : 1);
    u8 val_exp  = Config::getResourceBaseValue(ResourceType::EXP)  * (target_resource == ResourceType::EXP  ? 2 : 1);

    ResourceType best = ResourceType::NONE;
    u8 max_val = 0;

    if (res.iron > 0 && val_iron > max_val) {
        best = ResourceType::IRON; max_val = val_iron; 
    }
    if (res.gold > 0 && val_gold > max_val) { 
        best = ResourceType::GOLD; max_val = val_gold; 
    }
    if (res.gems > 0 && val_gems > max_val) { 
        best = ResourceType::GEMS; max_val = val_gems; 
    }
    if (res.exp  > 0 && val_exp  > max_val) { 
        best = ResourceType::EXP;  max_val = val_exp;  
    }

    return best;
}

u8 RegularBot::calculateCostToHome(u8 current_id) {
    if (current_id == 0) {
        return 0;
    }
    
    std::queue<u8> q;
    std::unordered_map<u8, u8> dist;
    
    q.push(current_id);
    dist[current_id] = 0;

    while (!q.empty()) {
        u8 curr = q.front();
        q.pop();

        if (curr == 0) {
            return dist[curr];
        }

        for (u8 adj : memory[curr].adjacent_rooms) {
            if (visited_rooms.count(adj) && dist.find(adj) == dist.end()) {
                dist[adj] = dist[curr] + 1;
                q.push(adj);
            }
        }
    }

    return 255; 
}

u8 RegularBot::getNextStepToHome(u8 current_id) {
    std::queue<u8> q;
    std::unordered_map<u8, u8> dist;
    
    q.push(0);
    dist[0] = 0;

    while (!q.empty()) {
        u8 curr = q.front();
        q.pop();

        for (u8 adj : memory[curr].adjacent_rooms) {
            if (visited_rooms.count(adj) && dist.find(adj) == dist.end()) {
                dist[adj] = dist[curr] + 1;
                q.push(adj);
            }
        }
    }

    u8 target_dist = dist[current_id] - 1;
    u8 best_adj = 255;

    for (u8 adj : memory[current_id].adjacent_rooms) {
        if (visited_rooms.count(adj) && dist.count(adj) && dist[adj] == target_dist) {
            if (adj < best_adj) {
                best_adj = adj;
            }
        }
    }
    return best_adj;
}

std::optional<u8> RegularBot::getNextStepToNearestUnvisited(u8 start_id) {
    std::queue<u8> q;
    std::unordered_map<u8, u8> dist;
    std::unordered_map<u8, u8> first_step; 

    q.push(start_id);
    dist[start_id] = 0;

    u8 min_dist = 255;
    std::optional<u8> best_target = std::nullopt;

    while (!q.empty()) {
        u8 curr = q.front();
        q.pop();

        if (dist[curr] > min_dist) continue;

        if (visited_rooms.count(curr) == 0) {
            if (dist[curr] < min_dist) {
                min_dist = dist[curr];
                best_target = curr;
            } else if (dist[curr] == min_dist && (!best_target.has_value() || curr < best_target.value())) {
                best_target = curr;
            }
            continue; 
        }

        std::vector<u8> sorted_adj = memory[curr].adjacent_rooms;
        std::sort(sorted_adj.begin(), sorted_adj.end());

        for (u8 adj : sorted_adj) {
            if (dist.find(adj) == dist.end()) {
                dist[adj] = dist[curr] + 1;
                first_step[adj] = (curr == start_id) ? adj : first_step[curr];
                q.push(adj);
            }
        }
    }

    if (best_target.has_value()) {
        return first_step[best_target.value()];
    }
    
    return std::nullopt;
}