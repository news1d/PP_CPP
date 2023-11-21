#include <mpi.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    srand(time(NULL) + world_rank);

    vector<string> menu = {"Пицца", "Макароны", "Пельмени", "Борщ", "Курица", "Салат", "Котлета"};
    int students = 15;
    int ordersPerStudent = 2;

    // Создание вектора, представляющего порядок студентов
    vector<int> studentOrder(students);
    for (int i = 0; i < students; i++) {
        studentOrder[i] = i + 1;
    }

    // Перемешивание порядка студентов
    random_shuffle(studentOrder.begin(), studentOrder.end());

    if (world_rank == 0) {
        // Повар
        for (int i = 0; i < students; i++) {
            int student_id = studentOrder[i];
            // Получение заказа от студента
            for (int order = 0; order < ordersPerStudent; order++) {
                sleep(1); // Пауза, чтобы не путался вывод
                char received_dish[50];
                MPI_Status received_status;
                MPI_Recv(&received_dish, 50, MPI_CHAR, student_id, 0, MPI_COMM_WORLD, &received_status);

                cout << "Повар получил заказ " << received_dish << " от студента " << student_id << endl;

                sleep(1); // Имитация времени приготовления

                // Отправка блюда студенту
                MPI_Send(&received_dish, 50, MPI_CHAR, student_id, 1, MPI_COMM_WORLD);
            }
        }
    } else {
        // Студенты
        for (int i = 0; i < ordersPerStudent; i++) {
            // Генерация заказа
            string dish = menu[rand() % menu.size()];

            // Отправка заказа повару
            MPI_Send(&dish[0], 50, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

            // Получение блюда от повара
            char received_dish[50];
            MPI_Recv(&received_dish, 50, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            cout << "Студент " << world_rank << " получил: " << received_dish << endl;
        }
    }

    MPI_Finalize();
    return 0;
}