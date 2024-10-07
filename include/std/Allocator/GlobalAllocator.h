#pragma once

#include <std/Allocator/Allocator.h>

namespace std
{
struct GlobalAllocator : public IAllocator
{
  // Inherited via IAllocator
  void* Alloc(i64 const size, i32 const alignment) override;
  void  Free(void const* p) override;
  bool  FollowsContainerDuringMove() override;

  GlobalAllocator();
  void* MemHandle;
};
} // namespace std
