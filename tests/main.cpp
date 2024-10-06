#include <Framework/UnitTest.h>
#include <thread>
#include <iostream>

void RunTests();
void PrintTestResults();

int main(int, char**)
{
    RunTests();
    PrintTestResults();
}

void RunTests()
{
    const int max = (int)UnitTest::Private::GlobalTestCollection.size();
    std::atomic<int> sharedTestIndex = 0;
    std::vector<std::jthread> workers(std::thread::hardware_concurrency());
    for (auto& worker : workers)
    {
        worker = std::jthread([max, &sharedTestIndex] {
            while (true)
            {
                const int localTestIndex = sharedTestIndex.fetch_add(1, std::memory_order::relaxed);
                if (localTestIndex >= max)
                    return;
                auto* test = UnitTest::Private::GlobalTestCollection[localTestIndex];
                test->Run();
            }
            });
    }
}

void PrintTestResults()
{
    const int passed = UnitTest::Private::GlobalPassedTestsCounter.load(std::memory_order::relaxed);
    const int failed = UnitTest::Private::GlobalFailedTestsCounter.load(std::memory_order::relaxed);
    std::cout << "Total: " << (passed + failed) << " Passed: " << passed << " Failed: " << failed << '\n';
}