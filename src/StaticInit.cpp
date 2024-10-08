#include <new>
#include <stl/Allocator/GlobalAllocator.h>
#include <stl/Platform/Platform.h>

namespace stl
{
struct StdStaticInit
{
  StdStaticInit();
};
static StdStaticInit _stdStaticInit;
} // namespace stl

stl::StdStaticInit::StdStaticInit()
{
  std::set_new_handler(+[] {
    if (IsDebuggerAttached())
      __debugbreak();
    std::terminate();
  });
}
