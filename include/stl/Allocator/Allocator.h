#pragma once

#include <stl/API.h>
#include <stl/Definitions.h>

namespace stl
{
// Interface used by all the containers in the std.
// Instead of being type-intrusive, as it literally changes the container type,
// we pay the price of storing 1 pointer to an allocator
class STL_API IAllocator
{
public:
  virtual ~IAllocator() = default;

  // Allocates a contiguous block of memory with at least `size` capacity.
  // Returns:
  //   success, a valid pointer
  //   fail, nullptr
  __declspec(allocator) __declspec(restrict) virtual void* Alloc(i64 const size, i32 const alignment) = 0;

  // Frees the block of memory previously allocated via a call to Alloc()
  // WARNING: `p` shall be the exact same pointer returned by Alloc()
  virtual void Free(void const* p) = 0;

  // If the allocator is also "moved into" the new container
  // when a move operation is performed.
  // If not, the memory needs to be taken over by the other allocator,
  // usually by copying everything.
  virtual bool FollowsContainerDuringMove() = 0;

  // The container this allocator is passed to, will automatically destroy it
  // depending on the operations performed on the container itself (destructor, changing allocator etc.)
  virtual bool OwnedByContainer() = 0;
};

extern STL_API IAllocator* globalAllocator;
} // namespace stl
