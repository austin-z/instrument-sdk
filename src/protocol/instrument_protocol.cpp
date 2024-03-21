#include "instrument_protocol.h"
#include <iostream>

namespace {

uint16_t getChecksum(const char *data, size_t len)//Modbus-CRC校验-----计算法
{
	uint8_t temp;
	uint16_t wcrc = 0XFFFF;//16位crc寄存器预置
	uint32_t i = 0, j = 0;//计数
	for (i = 0; i < len; i++)//循环计算每个数据
	{
		temp = data[i] & 0X00FF;//将八位数据与crc寄存器亦或
		wcrc ^= temp;						//将数据存入crc寄存器
		for (j = 0; j < 8; j++)	//循环计算数据的
		{
			if (wcrc & 0X0001)//判断右移出的是不是1，如果是1则与多项式进行异或。
			{
				wcrc >>= 1;//先将数据右移一位
				wcrc ^= 0XA001;//与上面的多项式进行异或
			}
			else//如果不是1，则直接移出
			{
				wcrc >>= 1;//直接移出
			}
		}
	}

    return wcrc;
}

void testChecksum()
{
    uint8_t bytes[] = {0x55, 0xAA, 0x02, 0x02, 0x04,
            0x00, 0x00, 0x02, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x8B, 0x0A, 0xF1, 0xF2};
    uint16_t chksum = getChecksum(reinterpret_cast<const char*>(bytes), 21);

    std::cout << "testChecksum " << chksum << ' ' << (0x8B | (0x0A << 8)) << std::endl;
}

}

InstrumentProtocol::InstrumentProtocol()
    : data_(25, 0)
{
    data_[0] = 0x55;
    data_[1] = 0xaa;

    // The data length is fixed at 19 currently.
    data_[2] = 0x19;

    // Default command valid time (unit: 25ms)
    data_[4] = 2;

    data_[23] = 0xf1;
    data_[24] = 0xf2;

    //testChecksum();
}

InstrumentProtocol::~InstrumentProtocol()
{

}

void InstrumentProtocol::setInstrument(Instrument id)
{
    data_[3] = char(id);
}

void InstrumentProtocol::setDof1(uint16_t v)
{
    data_[5] = v >> 8;
    data_[6] = v & 0xff;
}

void InstrumentProtocol::setDof2(uint16_t v)
{
    data_[7] = v >> 8;
    data_[8] = v & 0xff;
}

void InstrumentProtocol::setDof3(uint16_t v)
{
    data_[9] = v >> 8;
    data_[10] = v & 0xff;
}

void InstrumentProtocol::setDof4(uint16_t v)
{
    data_[11] = v >> 8;
    data_[12] = v & 0xff;
}

void InstrumentProtocol::setData(size_t index, char value)
{
    data_[index] = value;
}

const std::string &InstrumentProtocol::get()
{
    uint16_t chksum = getChecksum(data_.c_str(), data_.size() - 4);
    data_[21] = chksum & 0x00ff;
    data_[22] = (chksum & 0xff00) >> 8;

    return data_;
}

