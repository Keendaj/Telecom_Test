#pragma once

#include "../utils/types.hpp"
#include "labyrinth_types.hpp"
#include <memory>
#include <vector>

/**
 * @brief Класс, представляющий граф подземелья.
 *
 * Управляет комнатами, их ресурсами и валидацией переходов.
 * Реализован с помощью PImpl, чтобы скрыть внутренности и ускорить компиляцию
 */
class Labyrinth {
  public:
    /**
     * @brief Конструктор лабиринта.
     * @param n Количество комнат в подземелье (не считая стартовую).
     */
    explicit Labyrinth(u8 n);

    ~Labyrinth();
    Labyrinth(const Labyrinth &) = delete;
    Labyrinth &operator=(const Labyrinth &) = delete;
    Labyrinth(Labyrinth &&) = delete;
    Labyrinth &operator=(Labyrinth &&) = delete;

    /**
     * @brief Добавляет новую комнату в лабиринт.
     * @param id Уникальный идентификатор комнаты (0 - стартовая).
     * @param adj Список ID смежных комнат.
     * @param iron Количество ресурса 'iron' [0, 255].
     * @param gold Количество ресурса 'gold' [0, 255].
     * @param gems Количество ресурса 'gems' [0, 255].
     * @param exp Количество ресурса 'exp' [0, 255].
     */
    void addRoom(u8 id, const std::vector<u8> &adj, u8 iron, u8 gold, u8 gems,
                 u8 exp);

    /**
     * @brief Проверяет правильность лабиринта.
     * Переходы между комнатами должны быть строго двунаправленными.
     * @return true, если граф корректен
     */
    bool validate() const;

    /**
     * @brief Возвращает информацию о комнате по ее ID.
     * @param id Идентификатор искомой комнаты.
     * @return Константная ссылка на Room.
     * @throw std::out_of_range Если комната с указанным ID не существует.
     */
    const Room &getRoom(u8 id) const;

    /**
     * @brief Проверяет наличие прямого перехода между двумя комнатами.
     * @param from_id ID исходной комнаты.
     * @param to_id ID целевой комнаты.
     * @return true, если прямой переход существует.
     */
    bool hasConnection(u8 from_id, u8 to_id) const;

    /**
     * @brief Собирает весь ресурс указанного типа из комнаты.
     * При сборе ресурс полностью забирается из комнаты (становится равным 0).
     * @param room_id ID комнаты, в которой происходит сбор.
     * @param type Тип собираемого ресурса.
     * @return Количество успешно собранного ресурса.
     */
    u8 collectResource(u8 room_id, ResourceType type);

    /**
     * @brief Возвращает общее заявленное количество комнат.
     * @return Количество комнат N (без учета нулевой).
     */
    u8 getRoomsCount() const;

  private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};