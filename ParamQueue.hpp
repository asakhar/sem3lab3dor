#pragma once
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

template <const size_t __n>
struct QueueElem
{
  int num;
  char str[__n];
  QueueElem(int const& _num = 0, char const _str[__n] = nullptr) : num(_num)
  {
    std::memset(reinterpret_cast<char*>(str), 0, sizeof(str));
    if (_str)
      std::memcpy(reinterpret_cast<char*>(str), reinterpret_cast<const char*>(_str), std::min(std::strlen(_str), sizeof(str)));
  }
  friend std::istream& operator>>(std::istream& stream, QueueElem<__n>& elem)
  {
    return (stream >> elem.num).get(*elem.str).get(elem.str, __n);
  }
  friend std::ostream& operator<<(std::ostream& stream, QueueElem<__n>& elem)
  {
    return stream << "(" << elem.num << ", " << elem.str << ")";
  }
};

enum QueueState
{
  empt,
  part,
  full
};
template <typename Item, const size_t __n>
struct ParamQueue
{
  size_t front, back;
  Item queue[__n];

  ParamQueue<Item, __n>(std::vector<Item> _q = {}) : front(0), back(std::min(__n, _q.size()))
  {
    if (!_q.empty())
      std::memcpy(queue, _q.data(), std::min(sizeof(queue), sizeof(Item) * _q.size()));
  }
  friend std::istream& operator>>(std::istream& stream, ParamQueue<Item, __n>& q)
  {
    if ((q.back >= __n) != (q.front >= __n) && q.back % __n == q.front % __n)
      throw std::runtime_error("Queue overflow");
    stream >> q.queue[(q.back++) % __n];
    q.back %= 2 * __n;
    return stream;
  }
  friend std::ostream& operator<<(std::ostream& stream, ParamQueue<Item, __n>& q)
  {
    for (size_t i = q.front; i != q.back; (++i) %= 2 * __n)
    {
      stream << q.queue[i % __n];
    }
    return stream;
  }
  ParamQueue<Item, __n>& operator()(Item& elem)
  {
    if ((back >= __n) == (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue is empty");
    std::memcpy(reinterpret_cast<Item*>(&elem), reinterpret_cast<Item*>(queue + (front++) % __n), sizeof(Item));
    front %= 2 * __n;
    return *this;
  }
  Item& operator+=(Item const& elem)
  {
    if ((back >= __n) != (front >= __n) && back % __n == front % __n)
      throw std::runtime_error("Queue overflow");
    std::memcpy(reinterpret_cast<Item*>(queue + (back++) % __n), reinterpret_cast<Item*>(&elem), sizeof(Item));
    back %= 2 * __n;
    return queue[(back - 1) % __n];
  }
  QueueState check_state()
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
std::ostream& operator<<(std::ostream& stream, QueueState state)
{
  return stream << (state != QueueState::empt ? (state == QueueState::part ? "Partially full" : "Full") : "Empty");
}