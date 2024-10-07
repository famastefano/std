#include <Windows.h>
#include <cstddef>
#include <intrin.h>
#include <new>
#include <std/Allocator/GlobalAllocator.h>
#include <std/Assert/Assert.h>

static std::GlobalAllocator globalAllocatorInstance;

namespace std
{
IAllocator* globalAllocator = &globalAllocatorInstance;

static void* FromOverAligned(void const* p)
{
  u64           addr = (u64)p;
  unsigned long index;
  _BitScanForward(&index, (unsigned long)addr);
  addr >>= index - 4;
  return (void*)addr;
}

static void* ToOverAligned(void const* p, i32 alignment)
{
  unsigned long index;
  _BitScanForward(&index, alignment);
  u64 const addr = (u64)p << (index - 4);
  return (void*)addr;
}

void* GlobalAllocator::Alloc(i64 size, i32 const alignment)
{
  check(alignment % 2 == 0, "Alignment must be a power of 2.");

  if (alignment <= MEMORY_ALLOCATION_ALIGNMENT)
    return HeapAlloc(MemHandle, HEAP_ZERO_MEMORY, size);

  // TODO: verify correctness
  i32 const deltaAlign  = alignment - MEMORY_ALLOCATION_ALIGNMENT;
  size                 += deltaAlign;
  void* p               = HeapAlloc(MemHandle, HEAP_ZERO_MEMORY, size);
  return ToOverAligned(p, alignment);
}

void GlobalAllocator::Free(void const* p)
{
  HeapFree(MemHandle, 0, (void*)p);
}

bool GlobalAllocator::FollowsContainerDuringMove()
{
  return true;
}

GlobalAllocator::GlobalAllocator()
{
  MemHandle = GetProcessHeap();
  if (!MemHandle)
    MemHandle = HeapCreate(0, 0, 0);
}
} // namespace std

#pragma warning(disable : 28'251)
void* operator new(std::size_t count)
{
  return globalAllocatorInstance.Alloc((int)count, MEMORY_ALLOCATION_ALIGNMENT);
}

void* operator new[](std::size_t count)
{
  return globalAllocatorInstance.Alloc((int)count, MEMORY_ALLOCATION_ALIGNMENT);
}

void* operator new(std::size_t count, std::align_val_t al)
{
  return globalAllocatorInstance.Alloc((int)count, (int)al);
}

void* operator new[](std::size_t count, std::align_val_t al)
{
  return globalAllocatorInstance.Alloc((int)count, (int)al);
}

void* operator new(std::size_t count, std::nothrow_t const&) noexcept
{
  return globalAllocatorInstance.Alloc((int)count, MEMORY_ALLOCATION_ALIGNMENT);
}

void* operator new[](std::size_t count, std::nothrow_t const&) noexcept
{
  return globalAllocatorInstance.Alloc((int)count, MEMORY_ALLOCATION_ALIGNMENT);
}

void* operator new(std::size_t count, std::align_val_t al, std::nothrow_t const&) noexcept
{
  return globalAllocatorInstance.Alloc((int)count, (int)al);
}

void* operator new[](std::size_t count, std::align_val_t al, std::nothrow_t const&) noexcept
{
  return globalAllocatorInstance.Alloc((int)count, (int)al);
}

void operator delete(void* ptr) noexcept
{
  globalAllocatorInstance.Free(ptr);
}

void operator delete(void* ptr, std::size_t) noexcept
{
  globalAllocatorInstance.Free(ptr);
}

void operator delete[](void* ptr) noexcept
{
  globalAllocatorInstance.Free(ptr);
}

void operator delete[](void* ptr, std::size_t) noexcept
{
  globalAllocatorInstance.Free(ptr);
}
#pragma warning(default : 28'251)
