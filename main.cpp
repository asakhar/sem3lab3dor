#include <map>

#ifdef DYNAMIC
#ifdef LIST_VERSION
#include "ParamQueueLList.hpp"
#else
#include "ParamQueueDynamic.hpp"
#endif
typedef QueueElem Param;
typedef ParamQueue<Param> Queue;
#else
#include "ParamQueue.hpp"
typedef QueueElem<6> Param;
typedef ParamQueue<Param, 3> Queue;
#endif
typedef std::map<std::string, Queue> QueueMap;

void exit(QueueMap& qs, std::string& sel) { exit(0); }
void new_queue(QueueMap& qs, std::string& sel)
{
  std::cout << "Input queue name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  std::cout << "Do you want to input params now?(0_/1) > ";
  char ch;
  std::cin >> ch;
  std::vector<Param> params{};
  while (ch == '1')
  {
    std::cout << "Input param (format: <number> <string>): ";
    params.push_back({});
    std::cin >> params.back();
    std::cout << "Do you want to continue?(0_/1) > ";
    std::cin >> ch;
  }
  qs[name] = Queue(params);
  sel      = name;
  std::cout << "Successfully created!";
}

void remove_queue(QueueMap& qs, std::string& sel)
{
  std::cout << "Input queue name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  qs.erase(name);
  std::cout << "Successfully removed!";
}

void print_queue(QueueMap& qs, std::string& sel) { std::cout << "Selected queue: \n" << qs[sel]; }

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
  std::cout << "Input queue name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  if (qs.find(name) == qs.end())
  {
    std::cout << "Queue not found!";
    return;
  }
  sel = name;
  std::cout << "Queue '" << sel << "' selected!";
}

void add_params(QueueMap& qs, std::string& sel)
{
  char ch = '1';
  while (ch == '1')
  {
    std::cout << "Input param (format: <number> <string>): ";
    std::cin >> qs[sel];
    std::cout << "Do you want to continue?(0_/1) > ";
    std::cin >> ch;
  }
}

void pop_params(QueueMap& qs, std::string& sel)
{
  char ch = '1';
  while (ch == '1')
  {
    Param a;
    qs[sel](a);
    std::cout << "Popped: " << a << "\nDo you want to continue?(0_/1) > ";
    std::cin >> ch;
  }
}

void check_stat(QueueMap& qs, std::string& sel) { std::cout << "Queue status: " << qs[sel].check_state(); }
#ifdef DYNAMIC
void check_size_capacity(QueueMap& qs, std::string& sel)
{
  std::cout << "Size=" << qs[sel].size() << ", Capacity=" << qs[sel].capacity();
}

void copy_queue(QueueMap& qs, std::string& sel)
{
  std::cout << "Input copying queue name: ";
  std::string name;
  std::cin.get();
  std::getline(std::cin, name);
  std::cout << "Input new queue name: ";
  std::string namenew;
  //std::cin.get();
  std::getline(std::cin, namenew);
  sel     = namenew;
  qs[sel] = Queue(qs[name]);
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
  queues["numbers"]    = Queue({{1, "one"}, {2, "two"}, {3, "three"}});

  while (1)
  {
    std::cout << "Ask... \n\
    [0]Exit\n\
    [1]New queue\n\
    [2]Remove queue\n\
    [3]List queues\n\
    [4]Select queue\n\
    [5]Print queue\n\
    [6]Add to queue\n\
    [7]Pop from queue\n\
    [8]Check queue status\n"
#ifdef DYNAMIC
                 "\
    [9]Check size and capacity\n\
    [10]Copy queue\n\
    "
#endif
                 ">>";
    int choice;
    std::cin >> choice;
    if (choice > sizeof(options)/8-1
        || choice < 0)
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
  // auto queue = ParamQueue<QueueElem>({{123, "456"}, {543, "sdfv"}});
  // for (int i = 0; i < 13; i++)
  // {
  //   std::cout << queue << "\ncapacity()=" << queue.capacity() << "\nsize()=" << queue.size() << std::endl;
  //   a.num = i;
  //   a.set_str(std::to_string(i+10).c_str());
  //   queue += a;
  //   //std::cin >> queue;
  // }
  // std::cout << queue << "\ncapacity()=" << queue.capacity() << "\nsize()=" << queue.size() << std::endl;
  // for (int i = 0; i < 15; i++) {
  //   queue(a);
  //   std::cout << a << std::endl;
  // }
  // for (int i = 13; i < 20; i++)
  // {
  //   std::cout << queue << "\ncapacity()=" << queue.capacity() << "\nsize()=" << queue.size() << std::endl;
  //   a.num = i;
  //   a.set_str(std::to_string(i+10).c_str());
  //   queue += a;
  //   //std::cin >> queue;
  // }

  // ---------------------------------------------------------------

  // ParamQueue<QueueElem> queue;
  // std::cin >> queue >> queue;
  // std::cout << queue << std::endl;
  // QueueElem a;
  // queue(a)(a);
  // std::cout << a << std::endl;
  // a.num = 12;
  // queue += a;
  // a.str = "abcdfffdf";
  // queue += a;
  // std::cout << queue << std::endl;
  // queue(a)(a);
  // std::cout << queue << std::endl;

  //----------------------------------------------------

  // QueueElem<3> a;
  // sizeof(a);
  // ParamQueue<QueueElem<3>, 4> queue;
  // std::cin >> queue >> queue >> queue; // 3
  // std::cout << queue.check_state();

  // std::cout << queue << std::endl;
  // queue(a)(a)(a); // 0
  // std::cout << a << std::endl;

  // std::cin >> queue >> queue; // 2
  // queue(a); // 1

  // std::cin >> queue >> queue >> queue; // 4
  // queue(a)(a)(a)(a); // 0
}