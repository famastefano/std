#include "Platform.h"

#include <Windows.h>

namespace std
{
bool IsDebuggerAttached()
{
  return IsDebuggerPresent();
}
} // namespace std
