#pragma once

#include <cstddef>
#include <string>

class SerialPort
{
public:
    SerialPort(const char* port_name);
    ~SerialPort();

    bool open();

    bool write(const char* data, size_t size);

    bool read(char* buf, size_t buf_size, size_t desired_size);

    bool flush();

    bool close();

private:
    const char* port_name_;
    int fd_ = -1;
};
