#include <map>

#ifdef DYNAMIC
#include "ParamQueueDynamic.hpp"
#else
#ifdef NOOPERATOR
#include "ParamQueueMethodsOnly.hpp"
#else
#include "ParamQueue.hpp"
#endif
#endif
typedef std::map<std::string, ParamQueue> QueueMap;

void exit(QueueMap& qs, std::string& sel) { exit(0); }
void new_queue(QueueMap& qs, std::string& sel)
{
  std::cout << "Input ParamQueue name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  std::cout << "Do you want to input params now?(0_/1) > ";
  char ch;
  std::cin >> ch;
  std::vector<QueueElem> params{};
  while (ch == '1')
  {
    std::cout << "Input QueueElem (format: <number> <string>): ";
    params.push_back({});
#ifdef NOOPERATOR
    params.back().input();
#else
    std::cin >> params.back();
#endif
    std::cout << "Do you want to continue?(0_/1) > ";
    std::cin >> ch;
  }
  qs[name] = ParamQueue(params);
  sel      = name;
  std::cout << "Successfully created!";
}

void remove_queue(QueueMap& qs, std::string& sel)
{
  std::cout << "Input ParamQueue name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  qs.erase(name);
  std::cout << "Successfully removed!";
}

void print_queue(QueueMap& qs, std::string& sel)
{
#ifdef NOOPERATOR
  qs[sel].output(std::cout << "Selected ParamQueue: \n");
#else
  std::cout << "Selected ParamQueue: \n" << qs[sel];
#endif
}

void list_queues(QueueMap& qs, std::string& sel)
{
  for (auto& keyval : qs)
  {
    std::cout << keyval.first << (&*qs.rbegin() == &keyval ? "" : ", ");
  }
  std::cout << "\n";
}

void select_queue(QueueMap& qs, std::string& sel)
{
  std::cout << "Input ParamQueue name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  if (qs.find(name) == qs.end())
  {
    std::cout << "ParamQueue not found!";
    return;
  }
  sel = name;
  std::cout << "ParamQueue '" << sel << "' selected!";
}

void add_params(QueueMap& qs, std::string& sel)
{
  char ch = '1';
  while (ch == '1')
  {
    std::cout << "Input QueueElem (format: <number> <string>): ";
    try
    {
#ifdef NOOPERATOR
      qs[sel].input();
#else
      std::cin >> qs[sel];
#endif
    }
    catch (std::runtime_error& e)
    {
      std::cout << e.what() << std::endl;
      return;
    }
    std::cout << "Do you want to continue?(0_/1) > ";
    std::cin >> ch;
  }
}

void pop_params(QueueMap& qs, std::string& sel)
{
  char ch = '1';
  while (ch == '1')
  {
    QueueElem a;
    try
    {
#ifdef NOOPERATOR
      qs[sel].pop(a);
      a.output(std::cout << "Popped: ") << "\nDo you want to continue?(0_/1) > ";
#else
      qs[sel](a);
      std::cout << "Popped: " << a << "\nDo you want to continue?(0_/1) > ";
#endif
    }
    catch (std::runtime_error& e)
    {
      std::cout << e.what() << std::endl;
      return;
    }
    std::cin >> ch;
  }
}

void check_stat(QueueMap& qs, std::string& sel) { std::cout << "ParamQueue status: " << qs[sel].check_state(); }
#ifdef DYNAMIC
void check_size_capacity(QueueMap& qs, std::string& sel)
{
  std::cout << "Size=" << qs[sel].size() << ", Capacity=" << qs[sel].capacity();
}

void copy_queue(QueueMap& qs, std::string& sel)
{
  std::cout << "Input copying ParamQueue name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  std::cout << "Input new ParamQueue name: ";
  std::string namenew;
  // std::cin.get();
  std::getline(std::cin, namenew);
  sel     = namenew;
  qs[sel] = ParamQueue(qs[name]);
  std::cout << "Copied successfully!";
}
#endif

void (*options[])(QueueMap&, std::string&) = {exit,
                                              new_queue,
                                              remove_queue,
                                              list_queues,
                                              select_queue,
                                              print_queue,
                                              add_params,
                                              pop_params,
                                              check_stat,
#ifdef DYNAMIC
                                              check_size_capacity,
                                              copy_queue
#endif
};

int main()
{
  QueueMap queues;
  std::string selected = "numbers";
  queues["numbers"]    = ParamQueue({{1, "one"}, {2, "two"}, {3, "three"}});

  while (1)
  {
    std::cout << "Ask... \n\
    [0]Exit\n\
    [1]New ParamQueue\n\
    [2]Remove ParamQueue\n\
    [3]List queues\n\
    [4]Select ParamQueue\n\
    [5]Print ParamQueue\n\
    [6]Add to ParamQueue\n\
    [7]Pop from ParamQueue\n\
    [8]Check ParamQueue status\n"
#ifdef DYNAMIC
                 "\
    [9]Check size and capacity\n\
    [10]Copy ParamQueue\n\
    "
#endif
                 ">>";
    int choice;
    std::cin >> choice;
    if (choice > sizeof(options) / 8 - 1 || choice < 0)
    {
      std::cout << "Try again!\n";
      continue;
    }
    options[choice](queues, selected);
    std::cout << std::endl;
  }
  // ---------------------------------------------------------------

  // QueueElem a(123, "12345asfs");
  // // std::cout << a << std::endl;
  // // a.set_str("456mkdpoqkdkkf");
  // // std::cout << a << std::endl;
  // auto ParamQueue = ParamQueue<QueueElem>({{123, "456"}, {543, "sdfv"}});
  // for (int i = 0; i < 13; i++)
  // {
  //   std::cout << ParamQueue << "\ncapacity()=" << ParamQueue.capacity() << "\nsize()=" << ParamQueue.size() << std::endl;
  //   a.num = i;
  //   a.set_str(std::to_string(i+10).c_str());
  //   ParamQueue += a;
  //   //std::cin >> ParamQueue;
  // }
  // std::cout << ParamQueue << "\ncapacity()=" << ParamQueue.capacity() << "\nsize()=" << ParamQueue.size() << std::endl;
  // for (int i = 0; i < 15; i++) {
  //   ParamQueue(a);
  //   std::cout << a << std::endl;
  // }
  // for (int i = 13; i < 20; i++)
  // {
  //   std::cout << ParamQueue << "\ncapacity()=" << ParamQueue.capacity() << "\nsize()=" << ParamQueue.size() << std::endl;
  //   a.num = i;
  //   a.set_str(std::to_string(i+10).c_str());
  //   ParamQueue += a;
  //   //std::cin >> ParamQueue;
  // }

  // ---------------------------------------------------------------

  // ParamQueue<QueueElem> ParamQueue;
  // std::cin >> ParamQueue >> ParamQueue;
  // std::cout << ParamQueue << std::endl;
  // QueueElem a;
  // ParamQueue(a)(a);
  // std::cout << a << std::endl;
  // a.num = 12;
  // ParamQueue += a;
  // a.str = "abcdfffdf";
  // ParamQueue += a;
  // std::cout << ParamQueue << std::endl;
  // ParamQueue(a)(a);
  // std::cout << ParamQueue << std::endl;

  //----------------------------------------------------

  // QueueElem<3> a;
  // sizeof(a);
  // ParamQueue<QueueElem<3>, 4> ParamQueue;
  // std::cin >> ParamQueue >> ParamQueue >> ParamQueue; // 3
  // std::cout << ParamQueue.check_state();

  // std::cout << ParamQueue << std::endl;
  // ParamQueue(a)(a)(a); // 0
  // std::cout << a << std::endl;

  // std::cin >> ParamQueue >> ParamQueue; // 2
  // ParamQueue(a); // 1

  // std::cin >> ParamQueue >> ParamQueue >> ParamQueue; // 4
  // ParamQueue(a)(a)(a)(a); // 0
}