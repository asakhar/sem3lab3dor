#pragma once
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Queue element
 *
 */
struct QueueElem
{
  static constexpr size_t __n = 20;
  int num;
  char str[__n];
  /**
   * @brief Construct a new Queue Elem object
   *
   * @param _num Number data
   * @param _str String data of length __n
   */
  QueueElem(int const& _num = 0, char const _str[__n] = "") : num(_num)
  {
    size_t strl = std::strlen(_str);
    for (size_t i = 0; i < __n; i++)
      if (i < strl)
        str[i] = _str[i];
      else
        str[i] = '\0';
  }
  /**
   * @brief Input from stream
   *
   * @param stream e.g. std::cin
   * @return std::istream&
   */
  std::istream& input(std::istream& stream = std::cin) { return (stream >> num).get(*str).get(str, __n); }
  /**
   * @brief Output to stream
   *
   * @param stream e.g. std::cout
   * @return std::ostream&
   */
  std::ostream& output(std::ostream& stream = std::cout) { return stream << "(" << num << ", " << str << ")"; }
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
 */
struct ParamQueue
{
  static constexpr size_t __n = 20;

private:
  size_t front, back;
  QueueElem queue[__n];

public:
  /**
   * @brief Construct a new ParamQueue object from std::vector of elements
   *
   * @param _q elements
   */
  ParamQueue(std::vector<QueueElem> _q = {}) : front(0), back(0)
  {
    for (size_t i = 0; i < _q.size(); i++)
      push(_q[i]);
  }
  /**
   * @brief Input one element from stream
   *
   * @param stream e.g. std::cin
   * @return std::istream&
   */
  std::istream& input(std::istream& stream = std::cin)
  {
    if ((back >= __n) != (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue overflow");
    queue[(back++) % __n].input(stream);
    back %= 2 * __n;
    return stream;
  }
  /**
   * @brief Output to stream
   *
   * @param stream e.g. std::cout
   * @return std::ostream&
   */
  std::ostream& output(std::ostream& stream = std::cout)
  {
    for (size_t i = front; i != back; (++i) %= 2 * __n)
    {
      queue[i % __n].output(stream);
    }
    return stream;
  }
  /**
   * @brief Take first element from queue
   *
   * @param elem Reference to an object to fill
   * @return ParamQueue<QueueElem, __n>&
   */
  ParamQueue& pop(QueueElem& elem)
  {
    if ((back >= __n) == (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue is empty");
    elem = *(queue + (front++) % __n);
    front %= 2 * __n;
    return *this;
  }
  /**
   * @brief Add to queue
   *
   * @param elem Element to be added
   * @return QueueElem&
   */
  QueueElem& push(QueueElem const& elem)
  {
    if ((back >= __n) != (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue overflow");
    *(queue + (back++) % __n) = elem;
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