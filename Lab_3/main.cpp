#include <iostream>
#include <vector>
#include <time.h>
#include <climits>
#include <omp.h>
#include <algorithm>

using namespace std;

int genRndmNum() {
    return rand() % 131 - 30;
}

int main() {
    srand(time(0));

    // Генерируем вектор со случайными числами
    int vectorSize = 10000000;
    vector<int> numbers(vectorSize);
    for (int i = 0; i < vectorSize; i++) {
        numbers[i] = genRndmNum();
    }

    // Определение числа потоков
    vector<int> numThreads = {4, 10, 20, 40};

    // Измерение времени выполнения для каждого числа потоков
    for (int i = 0; i < numThreads.size(); i++) {
        vector<int> minNum(numThreads[i], INT_MAX);

        // Разбиваем вектор на участки для каждого потока
        int chunkSize = vectorSize / numThreads[i];

        clock_t startTime = clock();
        // Задаем количество потоков и описываем их работу
        #pragma omp parallel num_threads(numThreads[i])
        {
            int threadID = omp_get_thread_num();    // Номер потока
            int start = threadID * chunkSize;       // Номер потока * размер участка проверки (усл. 0 * 1000000 = 0)
            int end = start + chunkSize;            // Начало участка проверки + размер участка (усл. 0 + 1000000 = 1000000)

            // Ищем минимальное положительное число из дипазаона
            for (int i = start; i < end; i++) {
                if (numbers[i] > 0 && numbers[i] < minNum[threadID]) {
                    minNum[threadID] = numbers[i];
                }
            }
        }
        clock_t endTime = clock();

        double deltaTime = double(endTime - startTime) / CLOCKS_PER_SEC;

        // Поиск минимального положительного элемента среди результатов потоков
        int minNumber = *min_element(minNum.begin(), minNum.end());

        cout << "Кол-во потоков: " << numThreads[i] << ", Минимальный положительный элемент: " << minNumber
             << ", Затраченное время: " << deltaTime << " секунд." << endl;
    }
    return 0;
}