#include "instruments/forceps.h"
#include <chrono>
#include <iostream>
#include "instrument_protocol.h"
#include "serial_port.h"
#include "global.h"

Forceps::Forceps(const char* serial_port_name)
    : serial_port_(new SerialPort(serial_port_name))
{

}

Forceps::~Forceps()
{
    if (serial_port_)
        delete serial_port_;
}

bool Forceps::start()
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
        io_thread_ = std::thread(&Forceps::sendPendingCommands, this);
    }

    return true;
}

bool Forceps::stop()
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

void Forceps::control(const std::vector<int16_t> &joints)
{
    //qDebug() << cmd;
    std::lock_guard guard(pending_commands_mutex_);
    pending_commands_.push_back(joints);
}

void Forceps::sendPendingCommands()
{
    while (!stop_)
    {
        std::vector<std::vector<int16_t>> pending_commands;
        {
            std::lock_guard locker(pending_commands_mutex_);
            pending_commands.swap(pending_commands_);
        }

        for (const auto& joints : pending_commands) {
            InstrumentProtocol cmd;
            cmd.setInstrument(Instrument::Forceps);
            cmd.setDof1(joints[0]);
            cmd.setDof2(joints[1]);
            cmd.setDof3(joints[2]);
            cmd.setDof4(joints[3]);

            const std::string &data = cmd.get();
            //qDebug() << data.toHex(' ');
            if (!serial_port_->write(data.c_str(), data.size())) {
                continue;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));

#if 0
            char buf[26] = {0};
            if (!serial_port_->read(buf, sizeof(buf), 25)) {
                LOGE("Failed to read serial port");
            }

            qDebug() << QByteArray(buf, 26);
#endif
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
