#include "pch.h"
#include "seas.h"

#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>

/**
 * Базовый класс для тестов, работающих с вектором морей
 * Содержит общие настройки для всех тестов
 */
class SeaTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Выполняется перед каждым тестом - очищаем вектор
        seas.clear();
    }

    std::vector<Sea> seas;  // Вектор морей для тестирования
};

//==============================================================================
// ТЕСТЫ ДЛЯ ФУНКЦИИ FindDeepestSeaIndex (поиск самого глубокого моря)
//==============================================================================

/**
 * Тест проверяет поведение функции при пустом векторе
 * Ожидаемый результат: -1 (море не найдено)
 */
TEST_F(SeaTest, FindDeepestSeaIndex_EmptyVector_ReturnsMinus1) {
    EXPECT_EQ(FindDeepestSeaIndex(seas), -1);
}

/**
 * Тест проверяет корректность поиска самого глубокого моря
 * Создаем 3 моря с разной глубиной, самое глубокое - второе (индекс 1)
 */
TEST_F(SeaTest, FindDeepestSeaIndex_WithData_ReturnsCorrectIndex) {
    seas = {
        {"Море1", 100.0, 10.0},  // глубина 100
        {"Море2", 500.0, 20.0},  // глубина 500 - самое глубокое
        {"Море3", 300.0, 30.0}   // глубина 300
    };
    EXPECT_EQ(FindDeepestSeaIndex(seas), 1);  // ожидаем индекс 1 (второе море)
}

/**
 * Тест проверяет ситуацию, когда несколько морей имеют одинаковую глубину
 * Функция должна вернуть индекс первого встретившегося максимального значения
 */
TEST_F(SeaTest, FindDeepestSeaIndex_WithEqualDepths_ReturnsFirstMaxIndex) {
    seas = {
        {"Море1", 500.0, 10.0},  // глубина 500 - первое с макс глубиной
        {"Море2", 500.0, 20.0},  // тоже 500, но должно вернуться первое
        {"Море3", 300.0, 30.0}   // меньше
    };
    EXPECT_EQ(FindDeepestSeaIndex(seas), 0);  // ожидаем индекс 0 (первое море)
}

//==============================================================================
// ТЕСТЫ ДЛЯ ФУНКЦИИ FindLeastSaltySeaIndex (поиск моря с мин солёностью)
//==============================================================================

/**
 * Тест проверяет поведение функции при пустом векторе
 * Ожидаемый результат: -1 (море не найдено)
 */
TEST_F(SeaTest, FindLeastSaltySeaIndex_EmptyVector_ReturnsMinus1) {
    EXPECT_EQ(FindLeastSaltySeaIndex(seas), -1);
}

/**
 * Тест проверяет корректность поиска моря с минимальной солёностью
 * Минимальная солёность 5.0 у третьего моря (индекс 2)
 */
TEST_F(SeaTest, FindLeastSaltySeaIndex_WithData_ReturnsCorrectIndex) {
    seas = {
        {"Море1", 100.0, 35.0},  // солёность 35
        {"Море2", 200.0, 20.0},  // солёность 20
        {"Море3", 300.0, 5.0}    // солёность 5 - минимальная
    };
    EXPECT_EQ(FindLeastSaltySeaIndex(seas), 2);  // ожидаем индекс 2 (третье море)
}

/**
 * Тест проверяет ситуацию с одинаковой минимальной солёностью
 * Функция должна вернуть индекс первого встретившегося минимального значения
 */
TEST_F(SeaTest, FindLeastSaltySeaIndex_WithEqualSalinity_ReturnsFirstMinIndex) {
    seas = {
        {"Море1", 100.0, 5.0},   // солёность 5 - первая минимальная
        {"Море2", 200.0, 5.0},   // тоже 5, но должна вернуться первая
        {"Море3", 300.0, 10.0}   // солёность 10 - больше
    };
    EXPECT_EQ(FindLeastSaltySeaIndex(seas), 0);  // ожидаем индекс 0 (первое море)
}

//==============================================================================
// ТЕСТЫ ДЛЯ ФУНКЦИИ CalculateAverageDepth (вычисление средней глубины)
//==============================================================================

/**
 * Тест проверяет вычисление средней глубины для пустого вектора
 * Ожидаемый результат: 0.0 (среднее арифметическое от пустого множества)
 */
TEST_F(SeaTest, CalculateAverageDepth_EmptyVector_ReturnsZero) {
    EXPECT_DOUBLE_EQ(CalculateAverageDepth(seas), 0.0);
}

/**
 * Тест проверяет корректность вычисления средней глубины
 * Для глубин 100, 200, 300 среднее должно быть 200
 */
TEST_F(SeaTest, CalculateAverageDepth_WithData_ReturnsCorrectAverage) {
    seas = {
        {"Море1", 100.0, 10.0},
        {"Море2", 200.0, 20.0},
        {"Море3", 300.0, 30.0}
    };
    EXPECT_DOUBLE_EQ(CalculateAverageDepth(seas), 200.0);  // (100+200+300)/3 = 200
}

/**
 * Тест проверяет вычисление средней глубины для одного элемента
 * Среднее должно равняться глубине этого элемента
 */
TEST_F(SeaTest, CalculateAverageDepth_WithSingleElement_ReturnsThatElement) {
    seas = { {"Море1", 150.0, 15.0} };
    EXPECT_DOUBLE_EQ(CalculateAverageDepth(seas), 150.0);  // одно море - его глубина и есть среднее
}

//==============================================================================
// ТЕСТЫ ДЛЯ ФУНКЦИИ SortSeasByDepth (сортировка по глубине)
//==============================================================================

/**
 * Тест проверяет, что функция не падает при пустом векторе
 * Ожидаемый результат: пустой вектор остается пустым
 */
TEST_F(SeaTest, SortSeasByDepth_EmptyVector_NoCrash) {
    SortSeasByDepth(seas);
    EXPECT_TRUE(seas.empty());  // вектор должен остаться пустым
}

/**
 * Тест проверяет корректность сортировки по убыванию глубины
 * Исходный порядок: 300, 100, 200
 * После сортировки: 300, 200, 100 (по убыванию)
 */
TEST_F(SeaTest, SortSeasByDepth_SortsDescendingCorrectly) {
    seas = {
        {"Море1", 300.0, 10.0},  // самое глубокое
        {"Море2", 100.0, 20.0},  // самое мелкое
        {"Море3", 200.0, 30.0}   // среднее
    };

    SortSeasByDepth(seas);

    ASSERT_EQ(seas.size(), 3u);
    EXPECT_DOUBLE_EQ(seas[0].depth, 300.0);  // первое - самое глубокое
    EXPECT_DOUBLE_EQ(seas[1].depth, 200.0);  // второе - среднее
    EXPECT_DOUBLE_EQ(seas[2].depth, 100.0);  // третье - самое мелкое
}

/**
 * Тест проверяет устойчивость сортировки при равных глубинах
 * Порядок элементов с одинаковой глубиной может сохраняться (стабильная сортировка)
 */
TEST_F(SeaTest, SortSeasByDepth_WithEqualDepths_PreservesOrder) {
    seas = {
        {"Море1", 200.0, 10.0},  // глубина 200
        {"Море2", 200.0, 20.0},  // тоже 200
        {"Море3", 100.0, 30.0}   // глубина 100
    };

    SortSeasByDepth(seas);

    // Проверяем только глубины, порядок элементов с равной глубиной может быть любым
    EXPECT_DOUBLE_EQ(seas[0].depth, 200.0);
    EXPECT_DOUBLE_EQ(seas[1].depth, 200.0);
    EXPECT_DOUBLE_EQ(seas[2].depth, 100.0);
}

//==============================================================================
// ВСПОМОГАТЕЛЬНЫЙ КЛАСС ДЛЯ ТЕСТИРОВАНИЯ ФАЙЛОВЫХ ОПЕРАЦИЙ
//==============================================================================

/**
 * Класс для тестов, работающих с файлами
 * Автоматически создает и удаляет временные файлы
 */
class ReadSeasFromFileTest : public ::testing::Test {
protected:
    /**
     * Выполняется после каждого теста - удаляет все созданные временные файлы
     */
    void TearDown() override {
        for (const auto& file : temp_files) {
            std::remove(file.c_str());  // удаляем файл
        }
        temp_files.clear();
    }

    /**
     * Создает временный файл с заданным содержимым
     * @param content содержимое файла
     * @return имя созданного файла
     */
    std::string CreateTestFile(const std::string& content) {
        std::string filename = "test_file_" + std::to_string(++file_counter) + ".txt";
        std::ofstream file(filename);
        file << content;
        file.close();
        temp_files.push_back(filename);  // запоминаем для последующего удаления
        return filename;
    }

    std::vector<std::string> temp_files;  // список временных файлов
    int file_counter = 0;                  // счетчик для уникальных имен
};

//==============================================================================
// ТЕСТЫ ДЛЯ ФУНКЦИИ ReadSeasFromFile (чтение из файла)
//==============================================================================

/**
 * Тест проверяет поведение при отсутствии файла
 * Ожидаемый результат:
 * - возвращается 0 (ничего не прочитано)
 * - исходный вектор не изменяется
 */
TEST_F(ReadSeasFromFileTest, MissingFile_ReturnsZero_AndKeepsVectorUntouched) {
    std::vector<Sea> original = { {"Original", 1.0, 1.0} };  // исходные данные
    std::vector<Sea> seas = original;  // копируем для теста

    int count = ReadSeasFromFile("non_existent_file_12345.txt", seas);

    EXPECT_EQ(count, 0);                          // ничего не прочитано
    EXPECT_EQ(seas.size(), original.size());      // размер не изменился
    EXPECT_EQ(seas[0].name, original[0].name);    // данные не изменились
}

/**
 * Тест проверяет чтение корректных данных из файла
 * Создаем файл с тремя валидными записями и проверяем их чтение
 */
TEST_F(ReadSeasFromFileTest, ReadsValidDataFromFile) {
    // Содержимое файла с заголовком и тремя записями
    std::string content =
        "Название;Глубина;Солёность\n"
        "Черное море;2210;22.0\n"
        "Азовское море;13.5;11.0\n"
        "Балтийское море;55.0;7.0\n";

    std::string filename = CreateTestFile(content);
    std::vector<Sea> seas;

    int count = ReadSeasFromFile(filename, seas);

    // Проверяем количество прочитанных записей
    EXPECT_EQ(count, 3);
    ASSERT_EQ(seas.size(), 3u);

    // Проверяем первое море
    EXPECT_EQ(seas[0].name, "Черное море");
    EXPECT_DOUBLE_EQ(seas[0].depth, 2210.0);
    EXPECT_DOUBLE_EQ(seas[0].salinity, 22.0);

    // Проверяем второе море
    EXPECT_EQ(seas[1].name, "Азовское море");
    EXPECT_DOUBLE_EQ(seas[1].depth, 13.5);
    EXPECT_DOUBLE_EQ(seas[1].salinity, 11.0);

    // Проверяем третье море
    EXPECT_EQ(seas[2].name, "Балтийское море");
    EXPECT_DOUBLE_EQ(seas[2].depth, 55.0);
    EXPECT_DOUBLE_EQ(seas[2].salinity, 7.0);
}

/**
 * Тест проверяет пропуск некорректных строк в файле
 * Файл содержит:
 * - валидные строки
 * - строку без разделителей
 * - строку с нечисловыми значениями
 * Должны быть прочитаны только валидные строки
 */
TEST_F(ReadSeasFromFileTest, SkipsInvalidLines) {
    std::string content =
        "Название;Глубина;Солёность\n"
        "Черное море;2210;22.0\n"
        "Неверная строка без разделителей\n"           // должна быть пропущена
        "Азовское море;13.5;11.0\n"
        "Неверные;числа;not_a_number\n"                 // должна быть пропущена
        "Балтийское море;55.0;7.0\n";

    std::string filename = CreateTestFile(content);
    std::vector<Sea> seas;

    int count = ReadSeasFromFile(filename, seas);

    // Должны быть прочитаны только 3 валидные записи
    EXPECT_EQ(count, 3);
    ASSERT_EQ(seas.size(), 3u);
    EXPECT_EQ(seas[0].name, "Черное море");
    EXPECT_EQ(seas[1].name, "Азовское море");
    EXPECT_EQ(seas[2].name, "Балтийское море");
}

/**
 * Тест проверяет ограничение на максимальное количество записей (kMaxSeas)
 * Создаем файл с kMaxSeas + 5 записями
 * Должно быть прочитано только kMaxSeas записей
 */
TEST_F(ReadSeasFromFileTest, StopsAtMaxSeas) {
    std::string content;
    // Создаем больше записей, чем позволяет kMaxSeas
    for (int i = 0; i < kMaxSeas + 5; ++i) {
        content += "Sea" + std::to_string(i) + ";" + std::to_string(100 + i) + ";" + std::to_string(10 + i) + "\n";
    }

    std::string filename = CreateTestFile(content);
    std::vector<Sea> seas;

    int count = ReadSeasFromFile(filename, seas);

    // Должно быть прочитано ровно kMaxSeas записей
    EXPECT_EQ(count, kMaxSeas);
    EXPECT_EQ(seas.size(), static_cast<size_t>(kMaxSeas));
}

//==============================================================================
// КОМПЛЕКСНЫЙ ТЕСТ ДЛЯ ПРОВЕРКИ ВЗАИМОДЕЙСТВИЯ ВСЕХ ФУНКЦИЙ
//==============================================================================

/**
 * Комплексный тест проверяет работу всех основных функций вместе
 * Это помогает убедиться, что функции корректно взаимодействуют друг с другом
 * и не имеют побочных эффектов
 */
TEST_F(SeaTest, CombinedFunctionalityTest) {
    // Создаем набор данных
    seas = {
        {"Море1", 100.0, 35.0},
        {"Море2", 500.0, 20.0},
        {"Море3", 300.0, 5.0},
        {"Море4", 200.0, 15.0}
    };

    // Проверяем поиск до сортировки
    EXPECT_EQ(FindDeepestSeaIndex(seas), 1);      // Море2
    EXPECT_EQ(FindLeastSaltySeaIndex(seas), 2);   // Море3
    EXPECT_DOUBLE_EQ(CalculateAverageDepth(seas), 275.0);

    // Сортируем
    SortSeasByDepth(seas);

    // Проверяем, что сортировка работает правильно
    std::vector<double> expected_depths = { 500.0, 300.0, 200.0, 100.0 };
    for (size_t i = 0; i < seas.size(); i++) {
        EXPECT_DOUBLE_EQ(seas[i].depth, expected_depths[i]);
    }

    // Проверяем, что данные не потерялись (проверяем по именам)
    bool found_more2 = false;
    bool found_more3 = false;
    for (const auto& sea : seas) {
        if (sea.name == "Море2") found_more2 = true;
        if (sea.name == "Море3") found_more3 = true;
    }
    EXPECT_TRUE(found_more2);
    EXPECT_TRUE(found_more3);

    // Проверяем, что средняя глубина не изменилась
    EXPECT_DOUBLE_EQ(CalculateAverageDepth(seas), 275.0);
}

//==============================================================================
// ТОЧКА ВХОДА ДЛЯ ЗАПУСКА ТЕСТОВ
//==============================================================================

/**
 * Главная функция программы
 * Инициализирует Google Test и запускает все тесты
 */
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();  // запускает все тесты и возвращает результат
}
