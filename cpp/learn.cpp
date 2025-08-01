#include <atomic>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <set>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

volatile const int gIdx = 1;

enum class Week { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

typedef struct Student {
    int age;
    std::string name;
} Stu;

template <typename T>
class Person {
  public:
    Person(T age) : age_(age) { printf("init\n"); }
    ~Person() { printf("destroy\n"); }

    T getAge() { return age_; }

  private:
    T age_;
};

class Shape {
  public:
    Shape() {}
    virtual double calArea();
    virtual ~Shape();
};

class Triangle : public Shape {};

class Animal {
  public:
    virtual void yell() const = 0; // pure virtual function, must be implemented by sub-class
    virtual ~Animal() = default;
};

class Dog : public Animal {
  public:
    void yell() const override { std::cout << "Woof!" << std::endl; }
};

class Cat : public Animal {
  public:
    void yell() const override { std::cout << "Meow!" << std ::endl; }
};

void iterateMap() {
    std::unordered_map<std::string, int> uMap;
    uMap["1"] = 1;
    uMap["2"] = 2;
    // mode 1
    for (auto it = uMap.begin(); it != uMap.end(); it++) {
        std::cout << "K: " << it->first << ", V: " << it->second << std::endl;
    }
    // mode 2
    for (auto& p : uMap) { std::cout << "K: " << p.first << ", V: " << p.second << std::endl; }
}

void useSmartPtr() {
    std::shared_ptr<Animal> sharedDog = std::make_shared<Dog>();
    sharedDog->yell();
    std::shared_ptr<Animal> sharedDog1 = sharedDog;
    sharedDog1->yell();

    std::unique_ptr<Cat> uniqueCat = std::make_unique<Cat>();
    uniqueCat->yell();

    std::unique_ptr<Cat> uniqueCat1 = std::move(uniqueCat);
}

void useAtomic() {
    std::atomic<size_t> a;
    a = 1;
    std::cout << a << std::endl;

    std::unique_ptr<std::atomic<size_t>[]> buckets_ = std::make_unique<std::atomic<size_t>[]>(10);
    buckets_[0] = 1;
    for (int i = 0; i < 10; i++) { std::cout << buckets_[i]; }
}

template <typename T>
class SharedPtr {
  public:
    explicit SharedPtr(T* ptr = nullptr) : ptr_(ptr), cnt_(ptr_ ? new std::atomic<size_t>(1) : nullptr) {}

    ~SharedPtr() { release(); }

    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), cnt_(other.cnt_) {
        if (cnt_) { cnt_->fetch_add(1); }
    }

    SharedPtr(SharedPtr&& other) : ptr_(other.ptr_), cnt_(other.cnt_) {
        other.ptr_ = nullptr;
        other.cnt_ = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            cnt_ = other.cnt_;
            if (cnt_) { cnt_->fetch_add(1); }
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            cnt_ = other.cnt_;
            other.ptr_ = nullptr;
            other.cnt_ = nullptr;
        }
        return *this;
    }

    T& operator*() { return *ptr_; }

    T* operator->() { return ptr_; }

    T* get() const { return ptr_; }

    size_t useCount() const { return cnt_ ? cnt_->load() : 0UL; }

  private:
    void release() {
        if (cnt_) {
            if (cnt_->fetch_sub(1) == 0) {
                delete ptr_;
                delete cnt_;
            }
        }
    }

    T* ptr_;
    std::atomic<size_t>* cnt_;
};

template <typename T>
class UniquePtr {
  public:
    explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr) {}

    ~UniquePtr() { delete ptr_; }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) { other.ptr_ = nullptr; }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr_; }

    T* operator->() const { return ptr_; }

    T* get() { return ptr_; }

    T* release() {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    void reset(T* ptr = nullptr) {
        if (ptr_ != ptr) {
            delete ptr_;
            ptr_ = ptr;
        }
    }

  private:
    T* ptr_;
};

// Perfect forwarding
void process(int& x) { std::cout << "lvalue" << std::endl; }

void process(int&& x) { std::cout << "rvalue" << std::endl; }

template <typename T>
void forwarder(T&& x) {
    process(std::forward<T>(x));
}

// multithreading
void multithreading() {
    int x = 0, cnt = 5;
    std::mutex mutex_;
    std::thread threads[5];
    std::function<void(int)> f = [&x, &mutex_](int cnt) {
        for (int i = 0; i < cnt; i++) {
            mutex_.lock();
            x++;
            mutex_.unlock();
        }
    };
    for (int i = 0; i < 5; i++) { threads[i] = std::thread(f, cnt); }
    for (int i = 0; i < 5; i++) { threads[i].join(); }
    // should be 25
    std::cout << x << std::endl;
}

class Empty {
  public:
    Empty();                              // constructor
    ~Empty();                             // destructor
    Empty(const Empty& other);            // copy constructor
    Empty* operator&();                   // address function
    const Empty* operator&() const;       // const address function
    Empty& operator=(const Empty& other); // assigning operator
    Empty(Empty&& other);                 // move constructor
    Empty& operator=(Empty&& other);      // move assigning operator
};

// const overload
class A {
  public:
    void func(int& x) { std::cout << "non const " << x << std::endl; }
    void func(const int& x) { std::cout << "const " << x << std::endl; }

    void count() { std::cout << "non const" << std::endl; }
    void count() const { std::cout << "const" << std::endl; }
};

void constOverload() {
    A a;
    a.func(1);
    int x = 2;
    a.func(x);
    a.count();

    const A ca;
    ca.count();
}

// CRTP
template <typename Derived>
class Base {
  public:
    void interface() { static_cast<Derived*>(this)->implementation(); }

    void implementation() { std::cout << "Base implementation" << std::endl; }
};

class D1 : public Base<D1> {
  public:
    void implementation() { std::cout << "D1 implementation" << std::endl; }
};

class D2 : public Base<D2> {
  public:
    void implementation() { std::cout << "D2 implementation" << std::endl; }
};

void CRTP() {
    D1 d1;
    d1.interface(); // "D1 implementation"

    D2 d2;
    d2.interface(); // "D2 implementation"
}

int main() {
    // std::cout << static_cast<int>(Week::Monday) << std::endl;
    // Person p = Person<int>(1);
    // std::vector<int> arr;
    // for (std::size_t i = 0; i < arr.size(); i++) {
    // }
    // for (auto it = arr.cbegin(); it != arr.cend(); it++) {
    //     std::cout << *it << std::endl;
    // }
    // std::cout << sizeof(Derived) << std::endl;

    // forwarder(1);
    // int x = 2;
    // forwarder(x);

    // multithreading();

    // constOverload();

    return 0;
}
