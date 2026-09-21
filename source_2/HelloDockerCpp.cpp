#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif


int clientCount = 0;
bool generationDone = false;
std::mutex mtx;

void clientGenerator(int maxClients) {
    for (int i = 0; i < maxClients; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(mtx);
        ++clientCount;
        std::cout << "[Генератор] Клиент добавлен. Очередь: " << clientCount << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        generationDone = true;
        std::cout << "[Генератор] Все клиенты сгенерированы." << std::endl;
    }
}

void operatorWorker() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        bool shouldStop = false;
        {
            std::lock_guard<std::mutex> lock(mtx);

            if (clientCount > 0) {
                --clientCount;
                std::cout << "[Операционист] Клиент обслужен. Очередь: " << clientCount << std::endl;
            }
            else if (generationDone) {
                shouldStop = true;
            }
        }

        if (shouldStop) {
            break;
        }
    }

    std::cout << "[Операционист] Завершил работу." << std::endl;
}

int main(int argc, char* argv[]) {

#ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
#endif

    int maxClients = 10;
    if (argc > 1) {
        maxClients = std::stoi(argv[1]);
        if (maxClients < 0) maxClients = 0;
    }

    std::cout << "Максимальное количество клиентов: " << maxClients << std::endl;

    std::thread generator(clientGenerator, maxClients);
    std::thread operatorThread(operatorWorker);

    generator.join();
    operatorThread.join();

    std::cout << "Программа завершена." << std::endl;
    return 0;
}
