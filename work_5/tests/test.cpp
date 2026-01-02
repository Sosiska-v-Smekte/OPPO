#include "pch.h"
#include "seas.h"

#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>   // tmpnam_s, remove

static std::string WriteTempFile(const std::string& content) {
    char tmp_name[L_tmpnam]{};
#if defined(_MSC_VER)
    // MSVC: безопасная версия
    tmpnam_s(tmp_name);
#else
    std::tmpnam(tmp_name);
#endif
    std::string filename = std::string(tmp_name) + ".txt";

    std::ofstream out(filename, std::ios::binary);
    out << content;
    out.close();

    return filename;
}

static void RemoveFile(const std::string& filename) {
    std::remove(filename.c_str());
}

TEST(FindDeepestSeaIndexTest, EmptyVector_ReturnsMinus1) {
    std::vector<Sea> seas;
    EXPECT_EQ(FindDeepestSeaIndex(seas), -1);
}

TEST(FindDeepestSeaIndexTest, FindsMaxDepthIndex) {
    std::vector<Sea> seas = {
        {"Каспийское", 1025.0, 12.8},
        {"Средиземное", 1500.0, 38.4},
        {"Балтийское", 55.0, 7.0}
    };
    EXPECT_EQ(FindDeepestSeaIndex(seas), 1);
}

TEST(FindDeepestSeaIndexTest, Tie_ReturnsFirstMax) {
    std::vector<Sea> seas = {
        {"A", 100.0, 1.0},
        {"B", 100.0, 2.0},
        {"C", 50.0,  3.0}
    };
    // В коде сравнение строго ">", значит при равенстве остаётся первый максимум
    EXPECT_EQ(FindDeepestSeaIndex(seas), 0);
}

TEST(FindLeastSaltySeaIndexTest, EmptyVector_ReturnsMinus1) {
    std::vector<Sea> seas;
    EXPECT_EQ(FindLeastSaltySeaIndex(seas), -1);
}

TEST(FindLeastSaltySeaIndexTest, FindsMinSalinityIndex) {
    std::vector<Sea> seas = {
        {"A", 10.0, 12.8},
        {"B", 20.0, 38.4},
        {"C", 30.0, 7.0}
    };
    EXPECT_EQ(FindLeastSaltySeaIndex(seas), 2);
}

TEST(FindLeastSaltySeaIndexTest, Tie_ReturnsFirstMin) {
    std::vector<Sea> seas = {
        {"A", 10.0, 5.0},
        {"B", 20.0, 5.0},
        {"C", 30.0, 7.0}
    };
    EXPECT_EQ(FindLeastSaltySeaIndex(seas), 0);
}

TEST(CalculateAverageDepthTest, EmptyVector_ReturnsZero) {
    std::vector<Sea> seas;
    EXPECT_DOUBLE_EQ(CalculateAverageDepth(seas), 0.0);
}

TEST(CalculateAverageDepthTest, ComputesCorrectAverage) {
    std::vector<Sea> seas = {
        {"Море1", 100.0, 10.0},
        {"Море2", 200.0, 20.0},
        {"Море3", 300.0, 30.0}
    };
    EXPECT_DOUBLE_EQ(CalculateAverageDepth(seas), 200.0);
}

TEST(SortSeasByDepthTest, EmptyVector_NoCrash) {
    std::vector<Sea> seas;
    SortSeasByDepth(seas);
    EXPECT_TRUE(seas.empty());
}

TEST(SortSeasByDepthTest, SortsDescending) {
    std::vector<Sea> seas = {
        {"A", 300.0, 10.0},
        {"B", 100.0, 20.0},
        {"C", 200.0, 30.0}
    };

    SortSeasByDepth(seas);

    EXPECT_DOUBLE_EQ(seas[0].depth, 300.0);
    EXPECT_DOUBLE_EQ(seas[1].depth, 200.0);
    EXPECT_DOUBLE_EQ(seas[2].depth, 100.0);
}

TEST(ReadSeasFromFileTest, MissingFile_ReturnsZero_AndKeepsVectorUntouched) {
    std::vector<Sea> seas = { {"X", 1.0, 1.0} };
    int count = ReadSeasFromFile("definitely_not_existing_file_12345.txt", seas);
    EXPECT_EQ(count, 0);
    // В нашей реализации clear() происходит только после успешного открытия
    EXPECT_EQ(seas.size(), 1u);
    EXPECT_EQ(seas[0].name, "X");
}

TEST(ReadSeasFromFileTest, ReadsValidLines_SkipsBadLines_StopsAtMax) {
    // 1) Заголовок
    // 2) Пустая строка
    // 3) Валидные строки
    // 4) Строка без ';' должна быть пропущена
    // 5) Строка с нечислом должна быть пропущена (из-за try/catch)
    std::string content;
    content += "Название;Глубина;Солёность\n";
    content += "\n";
    content += "Sea0;100;10\n";
    content += "bad_line_without_semicolons\n";
    content += "Sea1;200;20\n";
    content += "SeaBad;not_a_number;30\n";

    // добиваем до > kMaxSeas, чтобы проверить лимит
    for (int i = 2; i < 20; ++i) {
        content += "Sea" + std::to_string(i) + ";" + std::to_string(100 + i) + ";" + std::to_string(1 + i) + "\n";
    }

    const std::string fn = WriteTempFile(content);

    std::vector<Sea> seas;
    int count = ReadSeasFromFile(fn, seas);

    // Должно быть ровно kMaxSeas валидных записей
    EXPECT_EQ(count, kMaxSeas);
    EXPECT_EQ(seas.size(), static_cast<size_t>(kMaxSeas));

    // Первые валидные записи сохраняются
    EXPECT_EQ(seas[0].name, "Sea0");
    EXPECT_DOUBLE_EQ(seas[0].depth, 100.0);
    EXPECT_DOUBLE_EQ(seas[0].salinity, 10.0);

    EXPECT_EQ(seas[1].name, "Sea1");
    EXPECT_DOUBLE_EQ(seas[1].depth, 200.0);
    EXPECT_DOUBLE_EQ(seas[1].salinity, 20.0);

    RemoveFile(fn);
}

TEST(LambdaFlowTest, MainPipelineButNoIO) {
    std::vector<Sea> seas = {
        {"A", 100.0, 10.0},
        {"B", 300.0, 35.0},
        {"C", 200.0, 5.0}
    };

    auto do_sort = [&]() { SortSeasByDepth(seas); };
    auto do_deepest = [&]() { return FindDeepestSeaIndex(seas); };
    auto do_least_salty = [&]() { return FindLeastSaltySeaIndex(seas); };
    auto do_avg = [&]() { return CalculateAverageDepth(seas); };

    do_sort();
    EXPECT_DOUBLE_EQ(seas[0].depth, 300.0);
    EXPECT_DOUBLE_EQ(seas[1].depth, 200.0);
    EXPECT_DOUBLE_EQ(seas[2].depth, 100.0);

    EXPECT_EQ(do_deepest(), 0);
    EXPECT_EQ(do_least_salty(), 1);
    EXPECT_DOUBLE_EQ(do_avg(), 200.0);
}


// Если у тебя в проекте нет gtest_main, оставь этот main.
// Если gtest_main есть, этот main можно удалить.
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
