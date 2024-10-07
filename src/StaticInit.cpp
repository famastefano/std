// TODO: Register https://en.cppreference.com/w/cpp/memory/new/set_new_handler, ideally with a debugbreak

#include <new>
#include <std/Allocator/GlobalAllocator.h>
#include <std/Platform/Platform.h>

namespace std
{
struct StdStaticInit
{
  StdStaticInit();
};
static StdStaticInit _stdStaticInit;
} // namespace std

std::StdStaticInit::StdStaticInit()
{
  std::set_new_handler(+[] {
    if (IsDebuggerAttached())
      __debugbreak();
    std::terminate();
  });
}
