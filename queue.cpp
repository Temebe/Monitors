#include <sys/types.h>
#include <mutex>
#include <iostream>
#include <memory>
#include <condition_variable>

#define FULL 10

class Car{
    std::string name;
    bool priority;

public:
    Car() {
        priority = false;
    }

    Car(bool priority, std::string name = "NULL") {
    this->priority = priority;
    this->name = name;
    }

    ~Car() {

    }
    bool hasPriority() {
        return priority;
    }
    std::string getName() {
        return name;
    }
};

class Queue {
    std::shared_ptr<Car> line[FULL];
    size_t actual;
    size_t capacity;
    std::condition_variable noRoom;

public:
    std::mutex m;

//public:
    Queue() {
        actual = 0;
        capacity = FULL;
    }

    std::shared_ptr<Car> pop() {
        std::lock_guard<std::mutex> lock(m);
        std::shared_ptr<Car> temp = line[actual];
        line[actual] = nullptr;
        actual = (actual + 1)%FULL;
        capacity++;
        if(capacity == 1)
            noRoom.notify_one();
        return temp;
    }

    void push(std::shared_ptr<Car> car) {
        std::unique_lock<std::mutex> lock;
        if(capacity == 0)
            noRoom.wait(lock);
        if(!isEmpty()) {
            if(car->hasPriority()) {
                actual = (actual + 9)%FULL;
                line[actual] = car;
                capacity--;
            }
            else {
                //actual = (actual + FULL - capacity)%FULL; //actual + size
                line[(actual + FULL - capacity)%FULL] = car;
                capacity--;

            }
        }
        else {
            line[actual] = car;
            capacity--;
        }
    }
    void status() {
        std::lock_guard<std::mutex> lock(m);
        for (int i = actual, j = 0; j < FULL; i = (i+1)%FULL, j++) {
        //std::cout << i << " " << j << " ";
            if(j < FULL - capacity)
                std::cout << line[i]->getName() << " ";
            else
                std::cout << "_ ";
        }
        std::cout << std::endl;
    }
    bool isEmpty() {
        return capacity == FULL;
    }
    bool priorityOnFront() { //no need for guarding, no one can do anything about that priority car
        if (capacity == FULL)
            return false;
        return line[actual]->hasPriority();
    }
};
