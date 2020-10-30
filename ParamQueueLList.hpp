#pragma once
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

struct QueueElem
{
  int num;
  std::string str;
  std::unique_ptr<QueueElem> next;
  QueueElem(int const& _num = 0, std::string const& _str = "") : num(_num), str(_str), next(nullptr) {}
  QueueElem(QueueElem const& other) : num{other.num}, str(other.str), next(nullptr) {}
  QueueElem(QueueElem&& other) : num{other.num}, str(other.str), next(std::move(other.next)){};
  QueueElem& operator=(QueueElem&& other)
  {
    num = other.num;
    str = other.str;
    next.reset(other.next.release());
    return *this;
  }
  QueueElem& operator=(QueueElem const& other)
  {
    num = other.num;
    str = other.str;
    next.reset();
    return *this;
  }
  friend std::istream& operator>>(std::istream& stream, QueueElem& elem)
  {
    stream >> elem.num;
    stream.get();
    return std::getline(stream, elem.str);
  }
  friend std::ostream& operator<<(std::ostream& stream, QueueElem& elem)
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
template <typename Item>
struct ParamQueue
{
private:
  std::unique_ptr<Item> front;
  Item* back;
  size_t _size;

public:
  ParamQueue<Item>(std::vector<Item> _q = {}) : _size(0), back(0)
  {
    for (auto& i : _q)
      (*this) += i;
  }
  ParamQueue<Item>(ParamQueue<Item> const& other) : _size(0), back(0)
  {
    auto otherf = other.front.get();
    while (otherf)
    {
      this->operator+=(*otherf);
      otherf = otherf->next.get();
    }
  }
  ParamQueue<Item>(ParamQueue<Item>&& other) : _size(other._size), back(other.back) { front.reset(other.front.release()); }
  ParamQueue<Item>& operator=(ParamQueue<Item> const& other)
  {
    auto otherf = other.front.get();
    _size        = 0;
    front.reset();
    back = nullptr;
    while (otherf)
    {
      this->operator+=(*otherf);
      otherf = otherf->next.get();
    }
    return *this;
  }
  ParamQueue<Item>& operator=(ParamQueue<Item>&& other)
  {
    _size = other._size;
    back = other.back;
    front.reset(other.front.release());
    return *this;
  }
  friend std::istream& operator>>(std::istream& stream, ParamQueue<Item>& q)
  {
    Item new_item;
    stream >> new_item;
    q += new_item;
    return stream;
  }
  friend std::ostream& operator<<(std::ostream& stream, ParamQueue<Item>& q)
  {
    auto item = q.front.get();
    while (item)
    {
      stream << *item;
      item = item->next.get();
    }
    return stream;
  }
  ParamQueue<Item>& operator()(Item& elem)
  {
    if (_size == 0)
      throw std::runtime_error("Queue is empty");
    elem = std::move(*front.release());
    front.reset(elem.next.release());
    _size--;
    return *this;
  }
  Item& operator+=(Item const& elem)
  {
    back = ((front ? back->next : front) = std::make_unique<Item>(elem)).get();
    _size++;
    return *back;
  }
  QueueState check_state()
  {
    if (_size == 0)
      return QueueState::empt;
    else
      return QueueState::part;
  }
  size_t size() {
    return _size;
  }
  size_t capacity() {
    return _size;
  }
};
std::ostream& operator<<(std::ostream& stream, QueueState state)
{
  return stream << (state != QueueState::empt ? "Not empty" : "Empty");
}