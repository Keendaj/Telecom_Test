#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "bot/regular_bot.hpp"
#include "labyrinth/labyrinth.hpp"
#include "manager/game_manager.hpp"
#include "utils/types.hpp"

int strict_stoi(const std::string &s) {
    size_t pos;
    int val = std::stoi(s, &pos);
    if (pos != s.length()) {
        throw std::invalid_argument("Invalid characters in number: " + s);
    }
    return val;
}

std::vector<u8> parseAdjStrict(const std::string &s) {
    if (s.empty() || s.front() == ',' || s.back() == ',') {
        throw std::invalid_argument("Invalid adjacency list format");
    }

    for (char c : s) {
        if (!std::isdigit(c) && c != ',') {
            throw std::invalid_argument(
                "Forbidden character in adjacency list");
        }
    }

    std::vector<u8> adj;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, ',')) {
        if (item.empty()) {
            throw std::invalid_argument("Empty adjacency item");
        }
        int val = strict_stoi(item);
        if (val < 0 || val > 255) {
            throw std::out_of_range("Room ID out of range");
        }
        adj.push_back(static_cast<u8>(val));
    }
    return adj;
}

int main() {
    std::ifstream in("in.txt");
    std::ofstream out("result.txt");

    if (!in.is_open()) {
        std::cerr << "Ошибка: не удалось открыть in.txt" << std::endl;
        return 1;
    }

    std::string line;

    if (!std::getline(in, line)) {
        return 1;
    }

    u8 n;
    try {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (ss >> token)
            tokens.push_back(token);

        if (tokens.size() != 1) {
            throw std::invalid_argument("N must be a single number");
        }

        int n_int = strict_stoi(tokens[0]);
        if (n_int < 1 || n_int > 255) {
            throw std::out_of_range("N out of bounds");
        }

        n = static_cast<u8>(n_int);
    } catch (...) {
        out << line << std::endl;
        return 0;
    }

    Labyrinth lab(n);
    std::unordered_map<u8, std::string> original_lines;
    std::unordered_map<u8, std::vector<u8>> adjacency_map;

    for (int i = 0; i <= n; ++i) {
        if (!std::getline(in, line)) {
            break;
        }

        try {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (ss >> token) {
                tokens.push_back(token);
            }

            if (tokens.size() != 6 && tokens.size() != 2) {
                throw std::invalid_argument("Wrong number of tokens");
            }

            int id_int = strict_stoi(tokens[0]);
            if (id_int < 0 || id_int > 255) {
                throw std::out_of_range("ID out of bounds");
            }
            u8 id = static_cast<u8>(id_int);

            if (id != 0 && tokens.size() != 6) {
                throw std::invalid_argument("Room must have 6 tokens");
            }

            std::vector<u8> adj = parseAdjStrict(tokens[1]);

            int iron = 0, gold = 0, gems = 0, exp = 0;
            if (tokens.size() == 6) {
                iron = strict_stoi(tokens[2]);
                gold = strict_stoi(tokens[3]);
                gems = strict_stoi(tokens[4]);
                exp = strict_stoi(tokens[5]);

                if (iron < 0 || iron > 255 || gold < 0 || gold > 255 ||
                    gems < 0 || gems > 255 || exp < 0 || exp > 255) {
                    throw std::out_of_range("Resource out of bounds");
                }
            }

            original_lines[id] = line;
            adjacency_map[id] = adj;
            lab.addRoom(id, adj, static_cast<u8>(iron), static_cast<u8>(gold),
                        static_cast<u8>(gems), static_cast<u8>(exp));

        } catch (...) {
            out << line << std::endl;
            return 0;
        }
    }

    for (const auto &[u, neighbors] : adjacency_map) {
        for (u8 v : neighbors) {
            auto it = adjacency_map.find(v);
            if (it == adjacency_map.end() ||
                std::find(it->second.begin(), it->second.end(), u) ==
                    it->second.end()) {
                out << original_lines[u] << std::endl;
                return 0;
            }
        }
    }

    if (!std::getline(in, line)) {
        return 1;
    }
    u8 m;
    ResourceType target = ResourceType::NONE;

    try {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (ss >> token) {
            tokens.push_back(token);
        }

        if (tokens.size() != 2) {
            throw std::invalid_argument("Last line must have 2 tokens");
        }

        int m_int = strict_stoi(tokens[0]);
        if (m_int < 2 || m_int > 255) {
            throw std::out_of_range("M out of bounds");
        }
        m = static_cast<u8>(m_int);

        std::string target_str = tokens[1];
        std::transform(target_str.begin(), target_str.end(), target_str.begin(),
                       ::tolower);

        if (target_str == "iron") {
            target = ResourceType::IRON;
        } else if (target_str == "gold") {
            target = ResourceType::GOLD;
        } else if (target_str == "gems") {
            target = ResourceType::GEMS;
        } else if (target_str == "exp") {
            target = ResourceType::EXP;
        } else {
            throw std::invalid_argument("Unknown resource type");
        }
    } catch (...) {
        out << line << std::endl;
        return 0;
    }

    RegularBot bot(target);
    GameManager manager(lab, bot, m);
    manager.run(out, target);

    return 0;
}