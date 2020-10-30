#pragma once
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct QueueElem
{
  int num;

private:
  std::unique_ptr<char[]> str;

public:
  QueueElem(int const& _num = 0, std::string const& _str = "") : num(_num), str(new char[_str.size() + 1])
  {
    std::memcpy(str.get(), _str.c_str(), _str.size() + 1);
  }
  QueueElem(QueueElem const& other) : num{other.num}, str(new char[std::strlen(other.str.get()) + 1])
  {
    std::memcpy(str.get(), other.str.get(), std::strlen(other.str.get()) + 1);
  }
  QueueElem(QueueElem&& other) : num{other.num}, str(std::move(other.str)){};
  QueueElem& operator=(QueueElem&& other)
  {
    num = other.num;
    str = std::move(other.str);
    return *this;
  }
  QueueElem& operator=(QueueElem const& other)
  {
    num = other.num;
    str.reset(new char[std::strlen(other.str.get()) + 1]);
    std::memcpy(str.get(), other.str.get(), std::strlen(other.str.get()) + 1);
    return *this;
  }
  friend std::istream& operator>>(std::istream& stream, QueueElem& elem)
  {
    stream >> elem.num;
    stream.get();
    std::string tmp;
    std::getline(stream, tmp);

    elem.str.reset(new char[tmp.size() + 1]);
    std::memcpy(elem.str.get(), tmp.c_str(), tmp.size() + 1);

    return stream;
  }
  friend std::ostream& operator<<(std::ostream& stream, QueueElem& elem)
  {
    return stream << "(" << elem.num << ", " << elem.str.get() << ")";
  }
  char const* set_str(char const* value = "")
  {
    str.reset(new char[std::strlen(value) + 1]);
    std::memcpy(str.get(), value, std::strlen(value) + 1);
    return str.get();
  }
  char const* get_str() const { return str.get(); }
  // int const& set_num(int const& value) { // not nessesserily to encapsulate
  //   return num
  // }
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
  std::unique_ptr<Item[]> head;
  size_t front, back;

  size_t _vector_size;

public:
  ParamQueue<Item>(std::vector<Item> const& _q = {}) : _vector_size(_q.capacity()), back(_q.size()), front(0)
  {
    head.reset(new Item[_q.capacity()]);
    for (size_t i = 0; i < _q.size(); i++)
      head[i] = _q[i];
  }
  ParamQueue<Item>(ParamQueue<Item> const& other, size_t multiplier = 1) : back(0), front(0), _vector_size(other._vector_size)
  {
    head.reset(new Item[other._vector_size * multiplier]);
    for (size_t i = other.front; i != other.back; (++i) %= 2 * _vector_size, back++)
    {
      head[back] = other.head[i % _vector_size];
    }
    _vector_size *= multiplier;
  }
  ParamQueue<Item>(ParamQueue<Item>&& other)
      : _vector_size(other._vector_size), back(other.back), front(other.front), head(other.head.release())
  {
  }
  inline void resize(size_t mul) { *this = std::move(ParamQueue<Item>(*this, mul)); }
  ParamQueue<Item>& operator=(ParamQueue<Item> const& other)
  {
    back         = 0;
    front        = 0;
    _vector_size = other._vector_size;
    head.reset(new Item[_vector_size]);
    for (size_t i = other.front; i != other.back; (++i) %= 2 * _vector_size, back++)
    {
      head[back] = other.head[i % _vector_size];
    }
    return *this;
  }
  ParamQueue<Item>& operator=(ParamQueue<Item>&& other)
  {
    front        = other.front;
    back         = other.back;
    _vector_size = other._vector_size;
    head.reset(other.head.release());
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
    for (size_t i = q.front; i != q.back; (++i) %= 2 * q.capacity())
      stream << q.head[i % q.capacity()];
    return stream;
  }
  ParamQueue<Item>& operator()(Item& elem)
  {
    if ((back >= _vector_size) == (front >= _vector_size) && back % _vector_size == front % _vector_size)
      throw std::runtime_error("Queue is empty");
    elem = head[(front++) % _vector_size];
    front %= 2 * _vector_size;
    return *this;
  }
  Item& operator+=(Item const& elem)
  {
    if ((back >= _vector_size) != (front >= _vector_size) && back % _vector_size == front % _vector_size)
      // throw std::runtime_error("Queue overflow"); /*resize vector instead*/
      resize(2);
    head[(back++) % _vector_size] = elem;
    back %= 2 * _vector_size;
    return head[(back - 1) % _vector_size];
  }
  QueueState check_state()
  {
    if (back % _vector_size == front % _vector_size)
    {
      if ((back >= _vector_size) == (front >= _vector_size))
        return QueueState::empt;
      else
        return QueueState::full;
    }
    else
      return QueueState::part;
  }
  inline size_t capacity() const { return _vector_size; }
  inline size_t size() const
  {
    if (back < front)
      return 2 * _vector_size - back + front;
    return back - front;
  }
};
std::ostream& operator<<(std::ostream& stream, QueueState state)
{
  return stream << (state != QueueState::empt ? "Not empty" : "Empty");
}