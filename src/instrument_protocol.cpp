#include "instrument_protocol.h"

InstrumentProtocol::InstrumentProtocol()
    : data_(25, 0)
{
    data_[0] = 0x55;
    data_[1] = 0xaa;

    // The data length is fixed at 19 currently.
    data_[2] = 0x19;

    // Default command valid time (unit: 25ms)
    data_[4] = 4;

    data_[23] = 0xf1;
    data_[24] = 0xf2;
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

