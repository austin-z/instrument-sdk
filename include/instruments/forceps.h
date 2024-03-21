#ifndef INSTRUMENTS_FORCEPS_H
#define INSTRUMENTS_FORCEPS_H

#include <vector>
#include <cstdint>
#include <memory>

#include "abstract_instrument.h"

class Forceps : public AbstractInstrument
{
public:
    explicit Forceps(const char* serial_port_name);
    ~Forceps() override;

    bool initialize() override;

    bool uninitialize() override;

    void control(const std::vector<int16_t> &velocities) override;

private:
    const std::unique_ptr<class ForcepsImpl> impl_;
};

#endif // INSTRUMENTS_FORCEPS_H
