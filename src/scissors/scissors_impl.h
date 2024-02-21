#ifndef INSTRUMENTS_SCISSORS_IMPL_H
#define INSTRUMENTS_SCISSORS_IMPL_H

#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include "global.h"

class SerialPort;

class NOT_EXPORT ScissorsImpl
{
public:
    explicit ScissorsImpl(const char* serial_port_name);
    ~ScissorsImpl();

    [[nodiscard]]
    bool initialize();

    bool uninitialize();

    void control(const std::vector<int16_t> &velocities);

private:
    void sendPendingCommands();

private:
    SerialPort *serial_port_;

    std::atomic_bool stop_{false};
    std::thread io_thread_;

    std::vector<std::vector<int16_t>> pending_commands_;
    std::mutex pending_commands_mutex_;
};

#endif // INSTRUMENTS_SCISSORS_IMPL_H
