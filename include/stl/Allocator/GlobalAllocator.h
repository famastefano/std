#pragma once

#include <stl/Allocator/Allocator.h>

namespace stl
{
class STL_API GlobalAllocator : public IAllocator
{
public:
  // Inherited via IAllocator
  __declspec(allocator) __declspec(restrict) void* Alloc(i64 const size, i32 const alignment) override;
  void Free(void const* p) override;
  bool FollowsContainerDuringMove() override;
  bool OwnedByContainer() override;

  GlobalAllocator();

private:
  inline static void* MemHandle;
};
} // namespace stl
