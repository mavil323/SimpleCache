#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class XR {
  int length;
  std::vector<int> xr;

public:
  XR(int length) : length(length), xr(length) {
    for (int i = 0; i < length; ++i)
      xr[i] = i;
  }

  int getLeastUsed() {
    return xr[0];
  }

  void setRecentlyUsed(int indexInSet) {
    for (int i = 0; i < length - 1; ++i)
      if (xr[i] == indexInSet)
        xr[i + 1] = indexInSet;
  }
};

class LRU {
  int frame_length, length;
  std::vector<XR> lru;

public:
  LRU(int frame_length, int length) : frame_length(frame_length), length(length), lru(length, XR(frame_length)) {}

  int leastInFrame(int frameIndex) {
    return lru[frameIndex].getLeastUsed();
  }

  void updateRecentlyUsed(int cache_index, int set_index) {
    lru[cache_index].setRecentlyUsed(set_index);
  }
};

class Set {
private:
  std::vector<int> set;

public:
  Set(int length) : set(length, -1) {}

  int hit(int address) {
    for (int i = 0; i < set.size(); ++i)
      if (set[i] == address)
        return i;
    return -1;
  }

  void update(int index, int address) {
    set[index] = address;
  }

  std::string toString() {
    std::stringstream ss;
    for (int i = 0; i < set.size(); ++i)
      ss << (set[i] >= 0 ? std::to_string(set[i]) : "empty") << (i != set.size() - 1 ? ", " : "");
    return ss.str();
  }
};

class Cache {
  int length, set_length;
  std::vector<Set> cache;
  LRU lru_p;

public:
  Cache(int length, int set_length) : length(length / set_length), set_length(set_length), cache(length, Set(set_length)), lru_p(set_length, length) {}

  bool poll_address(int address) {
    int cache_index = (set_length > length) ? 0 : address % (length / set_length);
    int hit_index = cache[cache_index].hit(address);
    if (hit_index >= 0) {
      lru_p.updateRecentlyUsed(cache_index, hit_index);
      return true;
    } else {
      int lu_index = lru_p.leastInFrame(cache_index);
      cache[cache_index].update(lu_index, address);
      lru_p.updateRecentlyUsed(cache_index, lu_index);
      return false;
    }
  }

  std::string toString() {
    std::stringstream ss;
    for (int i = 0; i < length; ++i)
      ss << "[" << cache[i].toString() << "]\n";
    return ss.str();
  }

  std::string cacheEvent(int address) {
    std::stringstream ss;
    if (poll_address(address))
      ss << toString() << "Hit\n";
    else
      ss << toString() << "Miss\n";
    return ss.str();
  }
};

int main() {
  int cache_length, i, address = 0;

  std::cout << "How many blocks are there in the cache?: ";
  std::cin >> cache_length;
  std::cout << "which set associativity?= \n0: Fully associative cache\n"
            << "1= Direct-mapped cache\n"
            << "2= 2-way Set associative cache\n"
            << "3= Four-Way\n"
            << "...";
  std::cin >> i;

  i = (i == 0) ? cache_length : i;

  Cache cache(cache_length, i);

  std::cout << "select one of these two options: \n1 == Yes \n0 == No \nUse the LRU rule.\nEnter the address... ";
  while (true) {
    int a;
    std::cin >> a;
    std::cout << cache.cacheEvent(a);
    std::cin >> a;
    if (a == 0) {
      std::cout << "Final state... \n" << cache.toString();
      break;
    }
  }

  return 0;
}
