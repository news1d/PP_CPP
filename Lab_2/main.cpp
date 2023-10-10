#include <iostream>
#include <thread>
#include <vector>
#include <time.h>

using namespace std;

// Функция для генерации случайных чисел от -30 до 100
int genRndmNum() {
    return rand() % 131 - 30;
}

// Функция для поиска минимального положительного элемента вектора в заданном диапазоне
void findMin(vector<int> &vec, int start, int end, int &result) {
    int min = INT_MAX;
    for (int i = start; i < end; i++){
        if (vec[i] > 0 && vec[i] < min)
            min = vec[i];
    }
    result = min;
}


int main() {
    srand(time(0));

    // Генерируем вектор со случайными числами
    int vectorSize = 10000000;
    vector<int> numbers;
    for (int i = 0; i < vectorSize; i++){
        numbers.push_back(genRndmNum());
    }

    // Определение числа потоков
    vector<int> numThreads = {4, 10, 20, 40};

    // Измерение времени выполнения для каждого числа потоков
    for (int i = 0; i < numThreads.size(); i++){
        vector<thread> threads;
        vector<int> minNum(numThreads[i], INT_MAX);

        // Разбиваем вектор на участки для каждого потока
        int chunkSize = vectorSize / numThreads[i];
        int start = 0;

        // Создаем потоки и задаем границы вектора для каждого потока
        for (int k = 0; k < numThreads[i]; k++){
            int end = start + chunkSize;
            threads.emplace_back(findMin, ref(numbers), start, end, ref(minNum[k]));
            start = end;
        }

        clock_t startTime = clock();

        // Запуск потоков
        for (thread& t : threads) {
            t.join();
        }

        clock_t endTime = clock();
        double deltaTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

        // Поиск минимального положительного элемента среди результатов потоков
        int minNumber = INT_MAX;
        for (int result = 0; result < minNum.size(); result++){
            if (result > 0 && result < minNumber)
                minNumber = result;
        }


        cout << "Кол-во потоков: " << numThreads[i] << ", Минимальный положительный элемент: " << minNumber
                  << ", Затраченное время: " << deltaTime << " секунд." << endl;

    }
    return 0;
}