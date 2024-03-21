#ifndef INSTRUMENTS_SCISSORS_H
#define INSTRUMENTS_SCISSORS_H

#include <vector>
#include <cstdint>
#include <memory>

#include "abstract_instrument.h"

class Scissors : public AbstractInstrument
{
public:
    explicit Scissors(const char* serial_port_name);
    ~Scissors() override;

    bool initialize() override;

    bool uninitialize() override;

    void control(const std::vector<int16_t> &velocities) override;

private:
    const std::unique_ptr<class ScissorsImpl> impl_;
};

#endif // INSTRUMENTS_SCISSORS_H
