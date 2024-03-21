#include "forceps_impl.h"
#include "instruments/abstract_instrument.h"
#include "instruments/forceps.h"

Forceps::Forceps(const char* serial_port_name)
    : AbstractInstrument()
    , impl_(new ForcepsImpl(serial_port_name))
{

}

Forceps::~Forceps() = default;

bool Forceps::initialize()
{
    return impl_->initialize();
}

bool Forceps::uninitialize()
{
    return impl_->uninitialize();
}

void Forceps::control(const std::vector<int16_t> &velocities)
{
    impl_->control(velocities);
}

// 
ForcepsImpl::ForcepsImpl(const char *serial_port_name)
    : InstrumentImpl(serial_port_name)
{

}