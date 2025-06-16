// doodluz7@gmail.com

#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace ariel {

    // תבנית לקונטיינר גנרי שתומך באיברים הניתנים להשוואה (כמו int, double, string)
    template<typename T>
    class MyContainer {
    private:
        std::vector<T> elements;       // האיברים כפי שהוכנסו ע"י המשתמש
        std::vector<T> sortedAsc;      // עותק ממויין של elements בסדר עולה
        std::vector<T> sortedDesc;     // עותק ממויין של elements בסדר יורד
        std::vector<T> sideCross;      // סידור צדדי: מינימום, מקסימום, שני במינימום וכו'
        std::vector<T> middleOut;      // סידור מהאמצע החוצה: אמצע, שמאלה וימינה לסירוגין
        mutable std::vector<T> reversedCopy; // עותק הפוך שנבנה רק לצורך איטרטור הפוך

        // פונקציה שמעדכנת את כל מבני הנתונים הנגזרים אחרי שינוי ברשימת האיברים
        void updateSortedVectors() {
            sortedAsc = elements; // העתק של האיברים המקוריים
            std::sort(sortedAsc.begin(), sortedAsc.end()); // מיון עולה

            sortedDesc = sortedAsc; // התחלה מהעותק הממויין
            std::reverse(sortedDesc.begin(), sortedDesc.end()); // היפוך לסדר יורד

            // יצירת sideCross: הקטן ביותר, הגדול ביותר, השני הקטן, השני הגדול וכו'
            sideCross.clear();
            size_t i = 0, j = sortedAsc.size();
            if (j == 0) return; // טיפול במקרה של קונטיינר ריק
            --j;
            while (i <= j) {
                if (i == j) sideCross.push_back(sortedAsc[i++]); // אמצע במקרה של מספר אי זוגי
                else {
                    sideCross.push_back(sortedAsc[i++]); // מינימלי
                    sideCross.push_back(sortedAsc[j--]); // מקסימלי
                }
            }

            // יצירת middleOut: תחילה האמצע, ואז שמאלה וימינה לסירוגין
            middleOut.clear();
            if (!elements.empty()) {
                std::vector<T> copy = elements;
                size_t n = copy.size();
                size_t mid = n / 2; // אינדקס האמצע
                middleOut.push_back(copy[mid]);
                int left = mid - 1;
                int right = mid + 1;
                while (left >= 0 || right < (int)n) {
                    if (left >= 0) middleOut.push_back(copy[left--]);
                    if (right < (int)n) middleOut.push_back(copy[right++]);
                }
            }
        }

    public:
        MyContainer() = default; // קונסטרקטור ברירת מחדל

        // הוספת איבר חדש לקונטיינר, עם עדכון מיידי של הסדרים הנגזרים
        void addElement(const T& val) {
            elements.push_back(val);
            updateSortedVectors();
        }

        // הסרת כל המופעים של איבר מהקונטיינר. במידה ואינו קיים - נזרקת חריגה
        void removeElement(const T& val) {
            auto it = std::remove(elements.begin(), elements.end(), val);
            if (it == elements.end()) throw std::runtime_error("Element not found");
            elements.erase(it, elements.end());
            updateSortedVectors();
        }

        // מחזירה את כמות האיברים הנוכחית בקונטיינר
        size_t size() const { return elements.size(); }

        // אופרטור פלט << שמדפיס את כל האיברים ברצף כפי שנשמרו
        friend std::ostream& operator<<(std::ostream& os, const MyContainer<T>& c) {
            for (const auto& x : c.elements) os << x << " ";
            return os;
        }

        // מחלקת בסיס לכל האיטרטורים - כוללת גישה לפי אינדקס, אינקרמנט, והשוואה
        class BaseIterator {
        protected:
            const std::vector<T>& vec; // רפרנס לווקטור שרצים עליו
            size_t index;              // מיקום נוכחי בתוך הווקטור
        public:
            BaseIterator(const std::vector<T>& v, size_t idx) : vec(v), index(idx) {}

            T operator*() const { return vec.at(index); } // גישה לאיבר הנוכחי

            BaseIterator& operator++() { // מעבר לאיבר הבא
                ++index;
                return *this;
            }

            bool operator==(const BaseIterator& other) const {
                return &vec == &other.vec && index == other.index;
            }

            bool operator!=(const BaseIterator& other) const {
                return !(*this == other);
            }
        };

        // איטרטור לסריקה בסדר עולה (מהקטן לגדול)
        class AscendingIterator : public BaseIterator {
        public:
            AscendingIterator(const std::vector<T>& v, size_t idx) : BaseIterator(v, idx) {}
            static AscendingIterator begin(const MyContainer<T>& c) { return AscendingIterator(c.sortedAsc, 0); }
            static AscendingIterator end(const MyContainer<T>& c) { return AscendingIterator(c.sortedAsc, c.sortedAsc.size()); }
        };
        AscendingIterator begin_ascending_order() const { return AscendingIterator::begin(*this); }
        AscendingIterator end_ascending_order() const { return AscendingIterator::end(*this); }

        // איטרטור לסריקה בסדר יורד (מהגדול לקטן)
        class DescendingIterator : public BaseIterator {
        public:
            DescendingIterator(const std::vector<T>& v, size_t idx) : BaseIterator(v, idx) {}
            static DescendingIterator begin(const MyContainer<T>& c) { return DescendingIterator(c.sortedDesc, 0); }
            static DescendingIterator end(const MyContainer<T>& c) { return DescendingIterator(c.sortedDesc, c.sortedDesc.size()); }
        };
        DescendingIterator begin_descending_order() const { return DescendingIterator::begin(*this); }
        DescendingIterator end_descending_order() const { return DescendingIterator::end(*this); }

        // איטרטור לסריקה צולבת - קטן, גדול, שני קטן, שני גדול וכו'
        class SideCrossIterator : public BaseIterator {
        public:
            SideCrossIterator(const std::vector<T>& v, size_t idx) : BaseIterator(v, idx) {}
            static SideCrossIterator begin(const MyContainer<T>& c) { return SideCrossIterator(c.sideCross, 0); }
            static SideCrossIterator end(const MyContainer<T>& c) { return SideCrossIterator(c.sideCross, c.sideCross.size()); }
        };
        SideCrossIterator begin_side_cross_order() const { return SideCrossIterator::begin(*this); }
        SideCrossIterator end_side_cross_order() const { return SideCrossIterator::end(*this); }

        // איטרטור לסריקה מהאמצע החוצה
        class MiddleOutIterator : public BaseIterator {
        public:
            MiddleOutIterator(const std::vector<T>& v, size_t idx) : BaseIterator(v, idx) {}
            static MiddleOutIterator begin(const MyContainer<T>& c) { return MiddleOutIterator(c.middleOut, 0); }
            static MiddleOutIterator end(const MyContainer<T>& c) { return MiddleOutIterator(c.middleOut, c.middleOut.size()); }
        };
        MiddleOutIterator begin_middle_out_order() const { return MiddleOutIterator::begin(*this); }
        MiddleOutIterator end_middle_out_order() const { return MiddleOutIterator::end(*this); }

        // איטרטור לסריקה לפי סדר הכנסת האיברים (ללא מיון)
        class OrderIterator : public BaseIterator {
        public:
            OrderIterator(const std::vector<T>& v, size_t idx) : BaseIterator(v, idx) {}
            static OrderIterator begin(const MyContainer<T>& c) { return OrderIterator(c.elements, 0); }
            static OrderIterator end(const MyContainer<T>& c) { return OrderIterator(c.elements, c.elements.size()); }
        };
        OrderIterator begin_order() const { return OrderIterator::begin(*this); }
        OrderIterator end_order() const { return OrderIterator::end(*this); }

        // איטרטור לסריקה הפוכה לפי סדר הכנסת האיברים
        class ReverseOrderIterator : public BaseIterator {
        public:
            ReverseOrderIterator(const std::vector<T>& v, size_t idx) : BaseIterator(v, idx) {}
            static ReverseOrderIterator begin(const MyContainer<T>& c) {
                c.reversedCopy = c.elements;
                std::reverse(c.reversedCopy.begin(), c.reversedCopy.end()); // הפיכה של הסדר
                return ReverseOrderIterator(c.reversedCopy, 0);
            }
            static ReverseOrderIterator end(const MyContainer<T>& c) {
                return ReverseOrderIterator(c.reversedCopy, c.reversedCopy.size());
            }
        };
        ReverseOrderIterator begin_reverse_order() const { return ReverseOrderIterator::begin(*this); }
        ReverseOrderIterator end_reverse_order() const { return ReverseOrderIterator::end(*this); }
    };
} 