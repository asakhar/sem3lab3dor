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
  int num;

private:
  std::unique_ptr<char[]> str;

public:
  /**
   * @brief Construct a new Queue Elem object
   *
   * @param _num Number data
   * @param _str String data
   */
  QueueElem(int const& _num = 0, std::string const& _str = "") : num(_num), str(new char[_str.size() + 1])
  {
    for (size_t i = 0; i < _str.size(); i++)
      str[i] = _str[i];
    str[_str.size()] = '\0';
  }
  /**
   * @brief Copy-construct a new Queue Elem object
   *
   * @param other Const reference of an object to be copied from
   */
  QueueElem(QueueElem const& other) : num{other.num}, str(new char[std::strlen(other.str.get()) + 1])
  {
    for (size_t i = 0; i < std::strlen(other.str.get()) + 1; i++)
      str[i] = other.str[i];
  }
  /**
   * @brief Move-construct a new Queue Elem object
   *
   * @param other rvalue reference of an object to be moved from
   */
  QueueElem(QueueElem&& other) : num{other.num}, str(std::move(other.str)){};
  /**
   * @brief Move-assignment operator
   *
   * @param other rvalue reference of an object to be moved from
   * @return QueueElem&
   */
  QueueElem& operator=(QueueElem&& other)
  {
    num = other.num;
    str = std::move(other.str);
    return *this;
  }
  /**
   * @brief Copy-assignment operator
   *
   * @param other Const reference of an object to be copied from
   * @return QueueElem&
   */
  QueueElem& operator=(QueueElem const& other)
  {
    num      = other.num;
    auto len = std::strlen(other.str.get()) + 1;
    str.reset(new char[len]);
    for (size_t i = 0; i < len; i++)
      str[i] = other.str[i];
    return *this;
  }
  /**
   * @brief Input from stream
   *
   * @param stream
   * @param elem
   * @return std::istream&
   */
  friend std::istream& operator>>(std::istream& stream, QueueElem& elem)
  {
    stream >> elem.num;
    stream.get();
    std::string tmp;
    std::getline(stream, tmp);

    elem.str.reset(new char[tmp.size() + 1]);
    for (size_t i = 0; i < tmp.size(); i++)
      elem.str[i] = tmp[i];
    elem.str[tmp.size()] = '\0';
    return stream;
  }
  /**
   * @brief Output to stream
   *
   * @param stream
   * @param elem
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& stream, QueueElem& elem)
  {
    return stream << "(" << elem.num << ", " << elem.str.get() << ")";
  }
  /**
   * @brief Set the string
   *
   * @param value
   * @return char const*
   */
  char const* set_str(char const* value = "")
  {
    auto len = std::strlen(value) + 1;
    str.reset(new char[len]);
    for(size_t i = 0; i < len; i++)
      str[i] = value[i];
    return str.get();
  }
  /**
   * @brief Get the string object
   *
   * @return char const*
   */
  char const* get_str() const { return str.get(); }
  // int const& set_num(int const& value) { // not nessesserily to encapsulate number
  //   return num
  // }
};

int main() {
    QueueElem a, b(1,"123");
    QueueElem c = QueueElem(std::move(b));
}
/**
 * @enum
 * @brief Queue filling state enum
 *
 */
enum QueueState
{
  empt /**
        * @brief Queue is empty
        *
        */
  ,
  part /**
        * @brief Queue is partially full
        *
        */
  ,
  full /**
        * @brief Queue is full
        *
        */
};
/**
 * @brief Queue class
 *
 */
class ParamQueue
{
private:
  std::unique_ptr<QueueElem[]> head;
  size_t front, back;

  size_t _vector_size;

public:
  /**
   * @brief Construct a new Param Queue object from std::vector of elements
   *
   * @param _q vector of elements
   */
  ParamQueue(std::vector<QueueElem> const& _q = {}) : _vector_size(_q.capacity()), back(_q.size()), front(0)
  {
    head.reset(new QueueElem[_q.capacity()]);
    for (size_t i = 0; i < _q.size(); i++)
      head[i] = _q[i];
  }
  /**
   * @brief Copy-construct a new Param Queue object with possible size extencion
   *
   * @param other Object to copy from
   * @param multiplier Size multiplication
   */
  ParamQueue(ParamQueue const& other, size_t multiplier = 1) : back(0), front(0), _vector_size(other._vector_size)
  {
    head.reset(new QueueElem[other._vector_size * multiplier]);
    for (size_t i = other.front; i != other.back; (++i) %= 2 * _vector_size, back++)
    {
      head[back] = other.head[i % _vector_size];
    }
    _vector_size *= multiplier;
  }
  /**
   * @brief Move-construct a new Param Queue object
   *
   * @param other rvalue reference to move from
   */
  ParamQueue(ParamQueue&& other)
      : _vector_size(other._vector_size), back(other.back), front(other.front), head(other.head.release())
  {
  }
  /**
   * @brief Increase vector size by mul times
   *
   * @param mul
   */
  inline void resize(size_t mul) { *this = ParamQueue(*this, mul); }
  /**
   * @brief Copy-assignment operator
   *
   * @param other Object to copy from
   * @return ParamQueue<QueueElem>&
   */
  ParamQueue& operator=(ParamQueue const& other)
  {
    back         = 0;
    front        = 0;
    _vector_size = other._vector_size;
    head.reset(new QueueElem[_vector_size]);
    for (size_t i = other.front; i != other.back; (++i) %= 2 * _vector_size, back++)
    {
      head[back] = other.head[i % _vector_size];
    }
    return *this;
  }
  /**
   * @brief Move-assignment operator
   *
   * @param other Object to move from
   * @return ParamQueue<QueueElem>&
   */
  ParamQueue& operator=(ParamQueue&& other)
  {
    front        = other.front;
    back         = other.back;
    _vector_size = other._vector_size;
    head.reset(other.head.release());
    return *this;
  }
  /**
   * @brief Input from stream operator
   *
   * @param stream e.g. std::cin
   * @param q Object to fill
   * @return std::istream&
   */
  friend std::istream& operator>>(std::istream& stream, ParamQueue& q)
  {
    QueueElem new_item;
    stream >> new_item;
    q += new_item;
    return stream;
  }
  /**
   * @brief Output to stream operator
   *
   * @param stream e.g. std::cout
   * @param q Queue to print to stream
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& stream, ParamQueue& q)
  {
    for (size_t i = q.front; i != q.back; (++i) %= 2 * q.capacity())
      stream << q.head[i % q.capacity()];
    return stream;
  }
  /**
   * @brief Take the first element from queue
   *
   * @param elem Object to fill reference
   * @return ParamQueue&
   */
  ParamQueue& operator()(QueueElem& elem)
  {
    if ((back >= _vector_size) == (front >= _vector_size) && back % _vector_size == front % _vector_size)
      throw std::runtime_error("Queue is empty");
    elem = head[(front++) % _vector_size];
    front %= 2 * _vector_size;
    return *this;
  }
  /**
   * @brief Add element to queue
   *
   * @param elem
   * @return QueueElem&
   */
  QueueElem& operator+=(QueueElem const& elem)
  {
    if ((back >= _vector_size) != (front >= _vector_size) && back % _vector_size == front % _vector_size)
      // throw std::runtime_error("Queue overflow"); /*resize vector instead*/
      resize(2);
    head[(back++) % _vector_size] = elem;
    back %= 2 * _vector_size;
    return head[(back + _vector_size - 1) % _vector_size];
  }
  /**
   * @brief State cheking
   *
   * @return QueueState
   */
  QueueState check_state() const
  {
    if (back % _vector_size == front % _vector_size)
    {
      if ((back >= _vector_size) == (front >= _vector_size))
        return QueueState::empt;
      else
        return QueueState::full; // array is full
    }
    else
      return QueueState::part;
  }
  /**
   * @brief Get vector capacity
   *
   * @return size_t
   */
  inline size_t capacity() const { return _vector_size; }
  /**
   * @brief Get queue size
   *
   * @return size_t
   */
  inline size_t size() const
  {
    if (back < front)
      return 2 * _vector_size - back + front;
    return back - front;
  }
};

/**
 * @brief Queue state stream print operator
 *
 * @param stream e.g. std::cout
 * @param state
 * @return std::ostream&
 */
std::ostream& operator<<(std::ostream& stream, QueueState state)
{
  return stream << (state != QueueState::empt ? "Not empty" : "Empty");
}