#ifndef INSTRUMENTS_IMPL_H
#define INSTRUMENTS_IMPL_H

#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include "global.h"

class SerialPort;

class NOT_EXPORT InstrumentImpl
{
public:
    explicit InstrumentImpl(const char* serial_port_name);
    virtual ~InstrumentImpl();

    [[nodiscard]]
    bool initialize();

    bool uninitialize();

    void control(const std::vector<int16_t> &velocities);

    virtual uint8_t id() const = 0;

private:
    void sendPendingCommands();

private:
    SerialPort *serial_port_;

    std::atomic_bool stop_{false};
    std::thread io_thread_;

    std::vector<std::vector<int16_t>> pending_commands_;
    std::mutex pending_commands_mutex_;
};

#endif // INSTRUMENTS_IMPL_H
