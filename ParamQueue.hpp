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
   * @brief Input from stream operator
   *
   * @param stream e.g. std::cin
   * @param elem element to be filled from stream
   * @return std::istream&
   */
  friend std::istream& operator>>(std::istream& stream, QueueElem& elem)
  {
    return (stream >> elem.num).get(*elem.str).get(elem.str, __n);
  }
  /**
   * @brief Output to stream operator
   *
   * @param stream e.g. std::cout
   * @param elem element to be translated to stream
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& stream, QueueElem& elem)
  {
    return stream << "(" << elem.num << ", " << elem.str << ")";
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
 */

struct ParamQueue
{
public:
  static const size_t __n = 20;

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
    ParamQueue a;
    for (size_t i = 0; i < _q.size(); i++)
      operator+=(_q[i]);
  }
  /**
   * @brief Input one element from stream operator
   *
   * @param stream e.g. std::cin
   * @param q Queue
   * @return std::istream&
   */
  friend std::istream& operator>>(std::istream& stream, ParamQueue& q)
  {
    if ((q.back >= __n) != (q.front >= __n) && q.back % __n == q.front % __n)
      throw std::runtime_error("Queue overflow");
    stream >> q.queue[(q.back++) % __n];
    q.back %= 2 * __n;
    return stream;
  }
  /**
   * @brief Output to stream operator
   *
   * @param stream e.g. std::cout
   * @param q Queue to be printed to stream
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& stream, ParamQueue& q)
  {
    for (size_t i = q.front; i != q.back; (++i) %= 2 * __n)
    {
      stream << q.queue[i % __n];
    }
    return stream;
  }
  /**
   * @brief Take first element from queue
   *
   * @param elem Reference to an object to fill
   * @return ParamQueue<QueueElem, __n>&
   */
  ParamQueue& operator()(QueueElem& elem)
  {
    if ((back >= __n) == (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue is empty");
    elem = queue[(front++) % __n];
    front %= 2 * __n;
    return *this;
  }
  /**
   * @brief Add to queue operator
   *
   * @param elem Element to be added
   * @return QueueElem&
   */
  QueueElem& operator+=(QueueElem const& elem)
  {
    if ((back >= __n) != (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue overflow");
    queue[(back++) % __n] = elem;
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