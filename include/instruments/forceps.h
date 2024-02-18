#ifndef INSTRUMENTS_FORCEPS_H
#define INSTRUMENTS_FORCEPS_H

#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>

class SerialPort;

class Forceps
{
public:
    explicit Forceps(const char* serial_port_name);
    ~Forceps();

    [[nodiscard]]
    bool start();

    [[nodiscard]]
    bool stop();

    void control(const std::vector<int16_t> &joints);

private:
    void sendPendingCommands();

private:
    SerialPort *serial_port_;

    std::atomic_bool stop_{false};
    std::thread io_thread_;

    std::vector<std::vector<int16_t>> pending_commands_;
    std::mutex pending_commands_mutex_;
};

#endif
