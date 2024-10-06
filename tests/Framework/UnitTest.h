#pragma once

#include <functional>
#include <vector>
#include <atomic>

namespace UnitTest::Private
{
    class TestBase
    {
    public:
        char const* SuiteName;
        char const* TestName;

        TestBase(char const* SuiteName, char const* TestName);
        void Run();

    protected:
        virtual void ExecuteTest() = 0;

        void MarkAsFailed();

    private:
        bool HasFailed = false;
    };

    extern std::vector<TestBase*> GlobalTestCollection;
    extern std::atomic<int> GlobalPassedTestsCounter;
    extern std::atomic<int> GlobalFailedTestsCounter;
}

#define UNIT_TEST_SUITE(Name) \
namespace UnitTest::##Name::Private \
{ \
    constexpr char const* InternalSuiteName = #Name; \
} \
namespace UnitTest::##Name \

#define UNIT_TEST(Name) \
class AutoRegisteringTest_##Name final : UnitTest::Private::TestBase \
{ \
public: \
AutoRegisteringTest_##Name() : UnitTest::Private::TestBase(Private::InternalSuiteName, #Name) {} \
protected:\
virtual void ExecuteTest() override; \
}; \
static AutoRegisteringTest_##Name _unit_test_autoreg_##Name;\
void AutoRegisteringTest_##Name::ExecuteTest()

#define REQUIRE(Expr) \
{ \
    const bool result = (Expr); \
    if (!result) \
    { \
        MarkAsFailed();\
        return;        \
    }\
}
#define REQUIRE_FALSE(expr) REQUIRE(!(expr))
#define FAIL() { MarkAsFailed(); return; }
#define PASS() { return; }