#ifndef SCISSORS_IMPL_H
#define SCISSORS_IMPL_H

#include "instrument_impl.h"

class NOT_EXPORT ScissorsImpl : public InstrumentImpl
{
public:
    explicit ScissorsImpl(const char* serial_port_name);
    ~ScissorsImpl() override = default;

    uint8_t id() const override { return 2; }

private:

};

#endif