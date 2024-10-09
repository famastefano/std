#include <Framework/UnitTest.h>
#include <stl/Container/Vector.h>

UNIT_TEST_SUITE(Vector)
{
  UNIT_TEST(Vector_int_Ctor)
  {
    stl::Vector<int> x;
    UNIT_TEST_REQUIRE(x.IsEmpty());
  }
}
