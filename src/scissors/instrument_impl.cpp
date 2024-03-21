#include "instrument_impl.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include "protocol/instrument_protocol.h"
#include "io/serial_port.h"
#include "global.h"

namespace {

void printHex(const char* buf, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (i > 0)
            std::cout << ' ';
        std::cout << std::hex << std::setfill('0') << std::setw(2)
                  << static_cast<int>(buf[i] & 0xff);
    }

    std::cout << std::dec << std::endl; // 重置cout为十进制输出，换行
}

}

// ------------------------------------------------------

InstrumentImpl::InstrumentImpl(const char* serial_port_name)
    : serial_port_(new SerialPort(serial_port_name))
{
    //InstrumentProtocol();
}

InstrumentImpl::~InstrumentImpl()
{
    uninitialize();

    if (serial_port_)
        delete serial_port_;
}

bool InstrumentImpl::initialize()
{
    // serial port
    {
        if (!serial_port_)
            return false;

        if (!serial_port_->open()) {
            std::cerr << "Failed to open com port\n";
            return false;
        }
    }

    // io thread
    {
        if (io_thread_.joinable()) {
            return false;
        }

        stop_ = false;
        io_thread_ = std::thread(&InstrumentImpl::sendPendingCommands, this);
    }

    return true;
}

bool InstrumentImpl::uninitialize()
{
    if (stop_)
        return true;

    if (!stop_) {
        stop_ = true;

        if (io_thread_.joinable()) {
            io_thread_.join();
        }
    }

    serial_port_->close();

    return true;
}

void InstrumentImpl::control(const std::vector<int16_t> &velocities)
{
    //qDebug() << cmd;
    std::lock_guard guard(pending_commands_mutex_);
    pending_commands_.push_back(velocities);
}

void InstrumentImpl::sendPendingCommands()
{
    while (!stop_)
    {
        std::vector<std::vector<int16_t>> pending_commands;
        {
            std::lock_guard locker(pending_commands_mutex_);
            pending_commands.swap(pending_commands_);
        }

        for (const auto& joints : pending_commands) {
            // if (stop_)
            //     break;

            InstrumentProtocol cmd;
            cmd.setInstrument(Instrument(id()));
            cmd.setDof1(joints[0]);
            cmd.setDof2(joints[1]);
            cmd.setDof3(joints[2]);
            cmd.setDof4(joints[3]);

            // If all joint values are zero, let motors return to the original state.
            if (joints[0] == 0 && joints[1] == 0 && joints[2] == 0 && joints[3] == 0) {
                cmd.setData(20, 1);
            }

            const std::string &data = cmd.get();
            printHex(data.c_str(), 25);
            if (!serial_port_->write(data.c_str(), data.size())) {
                continue;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));

#if 0
            std::cout << "reading" << std::endl;

            char buf[26] = {0};
            if (!serial_port_->read(buf, sizeof(buf), 25)) {
                std::cerr << "Failed to read serial port" << std::endl;
            }

            printHex(buf, 25);

            std::cout << "read" << std::endl;
#endif
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
