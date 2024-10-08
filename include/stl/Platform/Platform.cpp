#include "Platform.h"

#include <Windows.h>

namespace stl
{
bool IsDebuggerAttached()
{
  return IsDebuggerPresent();
}
} // namespace stl
