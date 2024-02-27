#include "serial_port.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <cstring>

SerialPort::SerialPort(const char *port_name)
    : port_name_(port_name)
{

}

SerialPort::~SerialPort()
{

}

bool SerialPort::open()
{
    fd_ = ::open(port_name_, O_RDWR | O_NOCTTY);
    if (fd_ == -1) {
        std::cerr << "Failed to open com port: " << strerror(errno) << "\n";
        return false;
    }

    fcntl(fd_, F_SETFL, 0); // 配置串口为阻塞模式

    // 配置串口
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(fd_, &tty) != 0) {
        std::cerr << "tcgetattr failed: " << strerror(errno) << "\n";
        ::close(fd_);
        return false;
    }

    // Set baud rate
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    // 8N1 Mode
    tty.c_cflag &= ~PARENB;     // Disable parity
    tty.c_cflag &= ~CSTOPB;     // Clear stop field, only one stop bit used in communication
    tty.c_cflag |= CS8;         // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;    // Disable RTS/CTS hardware flow control (most common)

    // Enable receiver, Ignore Modem Control lines
    tty.c_cflag |= CREAD | CLOCAL;

    // Disable canonical mode, and set buffer size
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

    // Raw output
    tty.c_oflag &= ~OPOST;

    // Read timeout
    tty.c_cc[VMIN]  = 10; // Minimum number of characters to read
    tty.c_cc[VTIME] = 1;  // Time to wait for data (tenths of seconds)

    if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
        std::cerr << "tcsetattr failed: " << strerror(errno) << "\n";
        ::close(fd_);
        return false;
    }

    return flush();
}

// TODO: deal with ::write returning 0 or EAGAIN
bool SerialPort::write(const char *data, size_t size)
{
    while (size > 0) {
        int ret = ::write(fd_, data, size);
        if (ret == -1) {
            std::cerr << "Write failed: " << strerror(errno) << "\n";
            break;
        }

        size -= ret;
    }

    return size == 0;
}

// TODO: deal with ::read returning 0 or EAGAIN
bool SerialPort::read(char *buf, size_t buf_size, size_t desired_size)
{
    size_t count = 0;
    while (count < desired_size) {
        int ret = ::read(fd_, buf + count, desired_size - count);
        if (ret == -1) {
            std::cerr << "Read failed: " << strerror(errno) << "\n";
            break;
        }

        count += ret;
    }

    return count == desired_size;
}

bool SerialPort::flush()
{
    if (fd_ == -1)
    {
        std::cerr << "flush: Invalid fd\n";
        return false;
    }

    // flush input cache
    //tcflush(fd, TCIFLUSH);

    // flush output cache
    //tcflush(fd, TCOFLUSH);

    // flush input and output cache
    if (int ret = tcflush(fd_, TCIOFLUSH); ret != 0)
    {
        std::cerr << "flush failed: " << strerror(errno) << "\n";
        return false;
    }

    return true;
}

bool SerialPort::close()
{
    ::close(fd_);

    return true;
}
