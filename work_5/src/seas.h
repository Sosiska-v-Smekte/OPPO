#pragma once
#include <string>
#include <vector>

// Общие данные
const int kMaxSeas = 10;              // Максимальное количество морей
const double kMinDepth = 0.0;         // Минимальная допустимая глубина
const double kMaxDepth = 12000.0;     // Максимальная допустимая глубина
const double kMinSalinity = 0.0;      // Минимальная допустимая солёность
const double kMaxSalinity = 45.0;     // Максимальная допустимая солёность

// Структура для хранения данных о море
struct Sea {
    std::string name;   // Название моря
    double depth;       // Глубина в метрах
    double salinity;    // Солёность в промилле
};

// === ФУНКЦИИ, КОТОРЫЕ РЕАЛЬНО ИМЕЕТ СМЫСЛ ТЕСТИРОВАТЬ (НЕ I/O) ===
int FindDeepestSeaIndex(const std::vector<Sea>& seas);
int FindLeastSaltySeaIndex(const std::vector<Sea>& seas);
double CalculateAverageDepth(const std::vector<Sea>& seas);
void SortSeasByDepth(std::vector<Sea>& seas);

// Файловый ввод (формально тоже I/O, но обычно преподаватели его считают частью логики)
int ReadSeasFromFile(const std::string& filename, std::vector<Sea>& seas);

// Остальные функции (I/O) объявлять можно, но тестировать по условию не обязательно.
double SafeInputDouble(const std::string& prompt, double min_value, double max_value);
void InputSeaData(Sea& sea);
void PrintSeaData(const Sea& sea);
void FindSeasBySalinity(const std::vector<Sea>& seas, double target_salinity, double tolerance = 0.1);
void PrintAllSeas(const std::vector<Sea>& seas);
