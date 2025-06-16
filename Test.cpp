// doodluz7@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "MyContainer.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
using namespace ariel;

// בדיקה: התנהגות של קונטיינר ריק
TEST_CASE("Empty container behavior") {
    MyContainer<int> c;
    CHECK(c.size() == 0); // הגודל צריך להיות אפס
    CHECK_THROWS(c.removeElement(1)); // הסרה של איבר לא קיים צריכה לזרוק חריגה
    CHECK(c.begin_ascending_order() == c.end_ascending_order()); // התחלה == סוף
}

// בדיקה: הוספה והסרה של כל המופעים של איבר
TEST_CASE("Adding and removing all occurrences") {
    MyContainer<int> c;
    c.addElement(1);
    c.addElement(1);
    c.addElement(1);
    CHECK(c.size() == 3);
    c.removeElement(1); // אמור להסיר את כל המופעים
    CHECK(c.size() == 0);
}

// בדיקה: ניסיון להסיר איבר שלא קיים
TEST_CASE("Removing element not present") {
    MyContainer<int> c;
    c.addElement(10);
    CHECK_THROWS(c.removeElement(5)); // 5 לא קיים
}

// בדיקה: הסרה חלקית של איברים כפולים
TEST_CASE("Handling duplicate values correctly") {
    MyContainer<int> c;
    c.addElement(5);
    c.addElement(3);
    c.addElement(5);
    c.addElement(3);
    c.addElement(1);
    c.removeElement(3); // כל המופעים של 3 יוסרו
    CHECK(c.size() == 3);
    CHECK_THROWS(c.removeElement(3)); // אין עוד 3 להסיר
}

// בדיקה: טיפוס שאינו מספר שלם – מחרוזות
TEST_CASE("Non-integer types: string") {
    MyContainer<std::string> c;
    c.addElement("apple");
    c.addElement("banana");
    c.addElement("apple");
    c.removeElement("apple"); // אמור להסיר את שניהם
    CHECK(c.size() == 1);
    for (auto it = c.begin_order(); it != c.end_order(); ++it) {
        CHECK(*it == "banana");
    }
}

// בדיקה: טיפוס double ובדיקת סדר השמירה
TEST_CASE("Non-integer types: double") {
    MyContainer<double> c;
    c.addElement(1.5);
    c.addElement(3.3);
    c.addElement(2.2);
    std::vector<double> expected = {1.5, 3.3, 2.2};
    size_t i = 0;
    for (auto it = c.begin_order(); it != c.end_order(); ++it) {
        CHECK(doctest::Approx(*it) == expected[i++]);
    }
}

// בדיקה: סריקת MiddleOut כאשר יש מספר זוגי של איברים
TEST_CASE("MiddleOutIterator even size behavior") {
    MyContainer<int> c;
    c.addElement(10);
    c.addElement(20);
    c.addElement(30);
    c.addElement(40);
    std::vector<int> pattern;
    for (auto it = c.begin_middle_out_order(); it != c.end_middle_out_order(); ++it) {
        pattern.push_back(*it);
    }
    CHECK(pattern.size() == 4);
    CHECK(std::find(pattern.begin(), pattern.end(), 10) != pattern.end());
    CHECK(std::find(pattern.begin(), pattern.end(), 40) != pattern.end());
}

// בדיקה: כל האיטרטורים פועלים כראוי על קונטיינר עם איבר יחיד
TEST_CASE("All iterators on single-element container") {
    MyContainer<int> c;
    c.addElement(99);
    for (auto it = c.begin_ascending_order(); it != c.end_ascending_order(); ++it) CHECK(*it == 99);
    for (auto it = c.begin_descending_order(); it != c.end_descending_order(); ++it) CHECK(*it == 99);
    for (auto it = c.begin_side_cross_order(); it != c.end_side_cross_order(); ++it) CHECK(*it == 99);
    for (auto it = c.begin_reverse_order(); it != c.end_reverse_order(); ++it) CHECK(*it == 99);
    for (auto it = c.begin_order(); it != c.end_order(); ++it) CHECK(*it == 99);
    for (auto it = c.begin_middle_out_order(); it != c.end_middle_out_order(); ++it) CHECK(*it == 99);
}

// בדיקה: סריקת MiddleOut עם מספר אי-זוגי של איברים
TEST_CASE("MiddleOutIterator odd size behavior") {
    MyContainer<int> c;
    c.addElement(5);
    c.addElement(10);
    c.addElement(15);
    std::vector<int> expected = {10, 5, 15}; // אמצע → שמאלה → ימינה
    size_t i = 0;
    for (auto it = c.begin_middle_out_order(); it != c.end_middle_out_order(); ++it) {
        CHECK(*it == expected[i++]);
    }
}

// בדיקה: SideCross אמור להחזיר איברים לפי: מינ', מקס', שני מינ', שני מקס'...
TEST_CASE("SideCrossIterator order") {
    MyContainer<int> c;
    c.addElement(1);
    c.addElement(2);
    c.addElement(3);
    c.addElement(4);
    c.addElement(5);
    std::vector<int> expected = {1, 5, 2, 4, 3};
    size_t i = 0;
    for (auto it = c.begin_side_cross_order(); it != c.end_side_cross_order(); ++it) {
        CHECK(*it == expected[i++]);
    }
}

// בדיקה: ReverseIterator אמור להחזיר את האיברים בסדר ההפוך להוספה
TEST_CASE("ReverseIterator order") {
    MyContainer<int> c;
    c.addElement(10);
    c.addElement(20);
    c.addElement(30);
    std::vector<int> expected = {30, 20, 10};
    size_t i = 0;
    for (auto it = c.begin_reverse_order(); it != c.end_reverse_order(); ++it) {
        CHECK(*it == expected[i++]);
    }
}

// בדיקה: אופרטור הפלט << מדפיס את כל האיברים
TEST_CASE("Output operator") {
    MyContainer<int> c;
    c.addElement(1);
    c.addElement(2);
    std::ostringstream oss;
    oss << c;
    CHECK(oss.str().find("1") != std::string::npos);
    CHECK(oss.str().find("2") != std::string::npos);
}

// בדיקה: שימוש ב-++ לאחר הגעה ל-end
TEST_CASE("Iterator increment past end") {
    MyContainer<int> c;
    c.addElement(42);
    auto it = c.begin_ascending_order();
    ++it;
    CHECK(it == c.end_ascending_order()); // לא אמור לחרוג או לקרוס
}
