#include "labyrinth.hpp"
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

struct Labyrinth::Impl {
    std::unordered_map<u8, Room> rooms;
    u8 total_rooms_N;

    explicit Impl(u8 n) : total_rooms_N(n) {}
};

Labyrinth::Labyrinth(u8 n) : pimpl(std::make_unique<Impl>(n)) {}

Labyrinth::~Labyrinth() = default;

void Labyrinth::addRoom(u8 id, const std::vector<u8> &adj, u8 iron, u8 gold,
                        u8 gems, u8 exp) {
    Room newRoom(id, adj);

    if (id != 0) {
        newRoom.resources = {iron, gold, gems, exp};
    }

    pimpl->rooms[id] = newRoom;
}

bool Labyrinth::validate() const {
    for (const auto &pair : pimpl->rooms) {
        u8 current_id = pair.first;
        for (u8 adj_id : pair.second.adjacent_rooms) {
            auto adj_it = pimpl->rooms.find(adj_id);
            if (adj_it == pimpl->rooms.end()) {
                return false;
            }

            const auto &reverse_adj = adj_it->second.adjacent_rooms;
            if (std::find(reverse_adj.begin(), reverse_adj.end(), current_id) ==
                reverse_adj.end()) {
                return false;
            }
        }
    }
    return true;
}

const Room &Labyrinth::getRoom(u8 id) const {
    auto it = pimpl->rooms.find(id);
    if (it != pimpl->rooms.end()) {
        return it->second;
    }
    throw std::out_of_range("Комната с таким ID не существует.");
}

bool Labyrinth::hasConnection(u8 from_id, u8 to_id) const {
    auto it = pimpl->rooms.find(from_id);
    if (it != pimpl->rooms.end()) {
        const auto &adj = it->second.adjacent_rooms;
        return std::find(adj.begin(), adj.end(), to_id) != adj.end();
    }
    return false;
}

u8 Labyrinth::collectResource(u8 room_id, ResourceType type) {
    auto it = pimpl->rooms.find(room_id);
    if (it != pimpl->rooms.end()) {
        return it->second.resources.collect(type);
    }
    return 0;
}

u8 Labyrinth::getRoomsCount() const { return pimpl->total_rooms_N; }