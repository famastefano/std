#include <Framework/UnitTest.h>

#include <algorithm>
#include <string>
#include <string_view>
#include <iostream>
#include <mutex>

static std::mutex GlobalOutputMutex;

namespace UnitTest::Private
{
    std::vector<TestBase*> GlobalTestCollection;
    std::atomic<int> GlobalPassedTestsCounter = 0;
    std::atomic<int> GlobalFailedTestsCounter = 0;

    TestBase::TestBase(char const* SuiteName, char const* TestName) : SuiteName(SuiteName), TestName(TestName)
    {
        const auto it = lower_bound(begin(GlobalTestCollection), end(GlobalTestCollection), this, [](TestBase* a, TestBase* b) -> bool
            {
                std::string nameA = a->SuiteName;
                nameA += a->TestName;

                std::string nameB = b->SuiteName;
                nameB += b->TestName;

                return nameA < nameB;
            });
        GlobalTestCollection.emplace(it, this);
    }
    void TestBase::Run()
    {
        std::string msg = "TEST ";
        msg += SuiteName;
        msg += '.';
        msg += TestName;

        ExecuteTest();

        std::string endMsg = "RESULT: ";
        if (HasFailed)
        {
            endMsg += "FAILED";
            UnitTest::Private::GlobalFailedTestsCounter.fetch_add(1, std::memory_order_relaxed); \
        }
        else
        {
            endMsg += "OK";
            UnitTest::Private::GlobalPassedTestsCounter.fetch_add(1, std::memory_order_relaxed); \
        }

        std::scoped_lock lck{ GlobalOutputMutex };
        std::cout << msg << '\n' << endMsg << '\n' << '\n';
    }
    void TestBase::MarkAsFailed()
    {
        HasFailed = true;
    }
}
