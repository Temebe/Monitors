#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <memory>
#include "queue.cpp"

#define N 50
#define PERCENTAGE 100
#define MIN 1
#define CHANCE_OF_DRIVING_AWAY 20
#define CHANCE_OF_NEW_CAR 45
#define CHANGE_OF_PRIORITY_CAR 5

void crossroad(Queue &front, Queue &back, int crossID, std::mutex &m) {
    //Car *inlet = nullptr, *stopped = nullptr;
    std::shared_ptr<Car> inlet, stopped;
    unsigned int number = 1;
    for (int i = 0; i < N; i++) {
        srand(time(NULL));

        /*if (stopped != nullptr) {
            front->push(stopped);
            stopped = nullptr;
        }*/
        if (back.priorityOnFront()) {
            stopped = back.pop();
        }
        else if (inlet != nullptr) {
            stopped = inlet;
            inlet = nullptr;
        }
        else if (!back.isEmpty()) {
            stopped = back.pop();
        }
        {
            std::lock_guard<std::mutex> lock(m);
            std::cout << "Crossroad " << crossID << std::endl;
            if (inlet != nullptr)
                std::cout << "  " << inlet->getName();
            std::cout << std::endl << "[ ";
            if (stopped != nullptr)
                std::cout << stopped->getName() << " ] ";
            else
                std::cout << "  ] ";
            back.status();
            std::cout << std::endl;
        }
        if (stopped != nullptr) {
            number = rand()%PERCENTAGE + MIN;
            if(number <= CHANCE_OF_DRIVING_AWAY)
                front.push(stopped);
            else {
                stopped = nullptr;
                std::lock_guard<std::mutex> lock(m);
                std::cout << "Car drove away from " << crossID << std::endl;
            }
                //stopped = nullptr;
            //stopped = nullptr;
        }

        // GENERATIRN NEW CAR
        srand(time(NULL)+crossID);
        number = rand()%PERCENTAGE + MIN;
        if (number <= CHANCE_OF_NEW_CAR) {
            srand(time(NULL));
            number = rand()%PERCENTAGE + MIN;
            if(number == CHANGE_OF_PRIORITY_CAR)
                inlet = std::make_shared<Car>(true, "Æ");
            else
                inlet = std::make_shared<Car>(false, "æ");
        }
        {
            std::lock_guard<std::mutex> lock(m);
            std::cout << "Crossroad " << crossID << std::endl;
            if (inlet != nullptr)
                std::cout << "  " << inlet->getName();
            std::cout << std::endl << "[   ] ";
            back.status();
            std::cout << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
