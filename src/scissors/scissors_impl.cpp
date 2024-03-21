#include "scissors_impl.h"
#include "instruments/scissors.h"

Scissors::Scissors(const char* serial_port_name)
    : AbstractInstrument()
    , impl_(new ScissorsImpl(serial_port_name))
{

}

Scissors::~Scissors() = default;

bool Scissors::initialize()
{
    return impl_->initialize();
}

bool Scissors::uninitialize()
{
    return impl_->uninitialize();
}

void Scissors::control(const std::vector<int16_t> &velocities)
{
    impl_->control(velocities);
}

//

ScissorsImpl::ScissorsImpl(const char *serial_port_name)
    : InstrumentImpl(serial_port_name)
{
    
}