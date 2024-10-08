#include <Framework/UnitTest.h>
#include <algorithm>
#include <iostream>
#include <thread>

void ParseOptions(int argc, char** argv);
void RunTests();
void PrintTestResults();

using namespace UnitTest::Private;

struct Options
{
  bool Parallel = false;
} Options;

int main(int argc, char** argv)
{
  if (argc > 1)
    ParseOptions(argc, argv);

  sort(begin(TestBase::GetTests()), end(TestBase::GetTests()), [](TestBase* a, TestBase* b) -> bool {
    std::string nameA  = a->SuiteName;
    nameA             += a->TestName;

    std::string nameB  = b->SuiteName;
    nameB             += b->TestName;

    return nameA < nameB;
  });
  RunTests();
  PrintTestResults();
}

void ParseOptions(int argc, char** argv)
{
  for (int i = 0; i < argc; ++i)
    Options.Parallel |= strcmp(argv[i], "--parallel") == 0;
}

void RunTests()
{
  if (!Options.Parallel)
  {
    for (auto* test : TestBase::GetTests())
      test->Run();
    return;
  }

  int const max = (int)TestBase::GetTests().size();

  std::atomic<int>          sharedTestIndex = 0;
  std::vector<std::jthread> workers(std::thread::hardware_concurrency());
  for (auto& worker : workers)
  {
    worker = std::jthread([max, &sharedTestIndex] {
      while (true)
      {
        const int localTestIndex = sharedTestIndex.fetch_add(1, std::memory_order::relaxed);
        if (localTestIndex >= max)
          return;
        auto* test = TestBase::GetTests()[localTestIndex];
        test->Run();
      }
    });
  }
}

void PrintTestResults()
{
  int const passed = GlobalPassedTestsCounter.load(std::memory_order::relaxed);
  int const failed = GlobalFailedTestsCounter.load(std::memory_order::relaxed);
  std::cout << "Total: " << (passed + failed) << " Passed: " << passed << " Failed: " << failed << '\n';
}
