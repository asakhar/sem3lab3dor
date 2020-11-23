#pragma once
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Queue element
 *
 * @tparam __n String length
 */
template <const size_t __n>
struct QueueElem
{
  int num;
  char str[__n];
  /**
   * @brief Construct a new Queue Elem object
   *
   * @param _num Number data
   * @param _str String data of length __n
   */
  QueueElem(int const& _num = 0, char const _str[__n] = nullptr) : num(_num)
  {
    std::memset(reinterpret_cast<char*>(str), 0, sizeof(str));
    if (_str)
      std::memcpy(reinterpret_cast<char*>(str), reinterpret_cast<const char*>(_str), std::min(std::strlen(_str), sizeof(str)));
  }
  /**
   * @brief Input from stream
   *
   * @param stream e.g. std::cin
   * @return std::istream&
   */
  std::istream& input(std::istream& stream = std::cin)
  {
    return (stream >> num).get(*str).get(str, __n);
  }
  /**
   * @brief Output to stream
   *
   * @param stream e.g. std::cout
   * @return std::ostream&
   */
  std::ostream& output(std::ostream& stream = std::cin)
  {
    return stream << "(" << num << ", " << str << ")";
  }
};

/**
 * @brief Queue filling state enum
 *
 */
enum QueueState
{ /**
   * @brief Empty
   *
   */
  empt,
  /**
   * @brief Partially full
   *
   */
  part,
  /**
   * @brief Full
   *
   */
  full
};
/**
 * @brief Parameter Queue
 *
 * @tparam Item Queue element type
 * @tparam __n Size of vector
 */
template <typename Item, const size_t __n>
struct ParamQueue
{
private:
  size_t front, back;
  Item queue[__n];

public:
  /**
   * @brief Construct a new ParamQueue object from std::vector of elements
   *
   * @param _q elements
   */
  ParamQueue(std::vector<Item> _q = {}) : front(0), back(std::min(__n, _q.size()))
  {
    if (!_q.empty())
      std::memcpy(queue, _q.data(), std::min(sizeof(queue), sizeof(Item) * _q.size()));
  }
  /**
   * @brief Input one element from stream
   *
   * @param stream e.g. std::cin
   * @return std::istream&
   */
  std::istream& input(std::istream& stream)
  {
    if ((back >= __n) != (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue overflow");
    stream >> queue[(back++) % __n];
    back %= 2 * __n;
    return stream;
  }
  /**
   * @brief Output to stream 
   *
   * @param stream e.g. std::cout
   * @return std::ostream&
   */
  std::ostream& output(std::ostream& stream)
  {
    for (size_t i = front; i != back; (++i) %= 2 * __n)
    {
      stream << queue[i % __n];
    }
    return stream;
  }
  /**
   * @brief Take first element from queue
   *
   * @param elem Reference to an object to fill
   * @return ParamQueue<Item, __n>&
   */
  ParamQueue<Item, __n>& pop(Item& elem)
  {
    if ((back >= __n) == (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue is empty");
    std::memcpy(reinterpret_cast<Item*>(&elem), reinterpret_cast<Item*>(queue + (front++) % __n), sizeof(Item));
    front %= 2 * __n;
    return *this;
  }
  /**
   * @brief Add to queue
   *
   * @param elem Element to be added
   * @return Item&
   */
  Item& push(Item const& elem)
  {
    if ((back >= __n) != (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue overflow");
    std::memcpy(reinterpret_cast<Item*>(queue + (back++) % __n), reinterpret_cast<Item*>(&elem), sizeof(Item));
    back %= 2 * __n;
    return queue[(back + __n - 1) % __n];
  }
  /**
   * @brief Empty, partially full & full state checking
   *
   * @return QueueState
   */
  QueueState check_state() const
  {
    if (back % __n == front % __n)
    {
      if ((back >= __n) == (front >= __n))
        return QueueState::empt;
      else
        return QueueState::full;
    }
    else
      return QueueState::part;
  }
};
/**
 * @brief State output format operator
 *
 * @param stream e.g. std::cout
 * @param state
 * @return std::ostream&
 */
std::ostream& operator<<(std::ostream& stream, QueueState state)
{
  return stream << (state != QueueState::empt ? (state == QueueState::part ? "Partially full" : "Full") : "Empty");
}