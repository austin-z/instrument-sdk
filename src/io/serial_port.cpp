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
    fd_ = ::open(port_name_, O_RDWR | O_NOCTTY | O_NDELAY);
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
        return -1;
    }

    cfsetospeed(&tty, B115200); // 设置输出波特率
    cfsetispeed(&tty, B115200); // 设置输入波特率

    tty.c_cflag &= ~PARENB; // 关闭奇偶校验
    tty.c_cflag &= ~CSTOPB; // 1停止位
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8数据位
    tty.c_cflag &= ~CRTSCTS; // 无硬件流控
    tty.c_cflag |= CREAD | CLOCAL; // 打开接收者，忽略modem控制线

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // 关闭软件流控
    tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 设置为原始输入

    tty.c_oflag &= ~OPOST; // 设置为原始输出

    tty.c_cc[VMIN] = 10; // 最小接收字符数 10
    tty.c_cc[VTIME] = 1; // 等待最多0.1秒，到时间后即使没读取到VMIN个字符，也会返回

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
