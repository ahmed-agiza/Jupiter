#ifndef COUNTINGLIST_H
#define COUNTINGLIST_H
#include <vector>
using namespace std;

class CountingListElement
{
public:
    int value;
    int count;

    CountingListElement()
    {
        count = 0;
        value = 0;
    }

    CountingListElement(int value)
    {
        count = 1;
        this->value = value;
    }

    bool canAdd(int value)
    {
        return ((this->value % 4 == 0) && (value - this->value == count));
    }

    void setValue(int value)
    {
        this->value = value;
    }

    int getValue() const
    {
        return value;
    }

    int getCount() const
    {
        return count;
    }

    void increment()
    {
        count++;
    }
};

class CountingList : public vector<CountingListElement>
{
public:
    void push_back(int val)
    {
        if (size() != 0)
        {
            CountingListElement& le = (*this)[size() - 1];
            if (le.canAdd(val))
            {
                le.increment();
                return;
            }
        }
        vector<CountingListElement>::push_back(CountingListElement(val));
    }
};

#endif // COUNTINGLIST_H
