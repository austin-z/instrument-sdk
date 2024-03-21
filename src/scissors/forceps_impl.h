#ifndef FORCES_IMPL_H
#define FORCES_IMPL_H

#include "instrument_impl.h"

class NOT_EXPORT ForcepsImpl : public InstrumentImpl
{
public:
    explicit ForcepsImpl(const char* serial_port_name);
    ~ForcepsImpl() override = default;

    uint8_t id() const override { return 3; }

private:

};

#endif