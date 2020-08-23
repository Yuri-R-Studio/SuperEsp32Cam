#include "IoExtender.h"
#include "DebugAssert.h"
#include <stdio.h>
#include <stdlib.h>

namespace Hal
{

IoExtender::IoExtender(Gpio *IoPins) : _gpio(IoPins)
{
}

IoExtender::~IoExtender()
{
}

}; // namespace Hal
