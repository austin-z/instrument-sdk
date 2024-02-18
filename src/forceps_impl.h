#ifndef INSTRUMENTS_FORCEPS_IMPL_H
#define INSTRUMENTS_FORCEPS_IMPL_H

#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>

class SerialPort;

class ForcepsImpl
{
public:
    explicit ForcepsImpl(const char* serial_port_name);
    ~ForcepsImpl();

    [[nodiscard]]
    bool initialize();

    [[nodiscard]]
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

#endif // INSTRUMENTS_FORCEPS_IMPL_H
