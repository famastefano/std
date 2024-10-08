#include <Framework/UnitTest.h>
#include <stl/Allocator/Allocator.h>

using namespace stl;

UNIT_TEST_SUITE(Allocator)
{
  static bool IsAlignedAs(void const* p, u64 const alignment)
  {
    return !((u64)p & (alignment - 1));
  }

  UNIT_TEST(GlobalAllocator_Allocating_1B_1Align)
  {
    void* p = globalAllocator->Alloc(1, 1);
    UNIT_TEST_REQUIRE(p);
    globalAllocator->Free(p);
  }

  UNIT_TEST(GlobalAllocator_Allocating_1B_16Align)
  {
    void* p = globalAllocator->Alloc(1, 16);
    UNIT_TEST_REQUIRE(p);
    UNIT_TEST_REQUIRE(IsAlignedAs(p, 16));
    globalAllocator->Free(p);
  }

  UNIT_TEST(GlobalAllocator_Allocating_123B_2BAlign)
  {
    void* p = globalAllocator->Alloc(123, 2);
    UNIT_TEST_REQUIRE(p);
    UNIT_TEST_REQUIRE(IsAlignedAs(p, 2));
    globalAllocator->Free(p);
  }

  UNIT_TEST(GlobalAllocator_Allocating_20MB_8BAlign)
  {
    void* p = globalAllocator->Alloc(20 * 1'024 * 1'024, 8);
    UNIT_TEST_REQUIRE(p);
    UNIT_TEST_REQUIRE(IsAlignedAs(p, 8));
    globalAllocator->Free(p);
  }
}
