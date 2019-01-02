#include <thread>
#include <vector>
#include "crossroad.cpp"

int main() {
    /*Queue *A = new Queue;
    Queue *B = new Queue;
    Queue *C = new Queue;*/
    //Queue A, B, C;
    /*std::thread cross1 (crossroad, std::ref(A), std::ref(B), 1);
    std::thread cross2 (crossroad, B, C, 2);
    std::thread cross3 (crossroad, C, A, 3);
    cross1.join();
    cross2.join();
    cross3.join();*/

    /*boost::scoped_ptr<Queue> A(new Queue);
    boost::scoped_ptr<Queue> B(new Queue);
    boost::scoped_ptr<Queue> C(new Queue);*/
    //Queue A, B, C;
    Queue A, B, C;

    std::mutex m;

    std::vector<std::thread> threads;
    threads.emplace_back(crossroad, std::ref(A), std::ref(B), 1, std::ref(m));
    threads.emplace_back(crossroad, std::ref(B), std::ref(C), 2, std::ref(m));
    threads.emplace_back(crossroad, std::ref(C), std::ref(A), 3, std::ref(m));
    for (auto &thread : threads) thread.join();

}
