/* Copyright (c) 2013 Antonie Jovanoski <minimoog77_at_gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "modbusregisterlistmodel.h"
#include <QScopedPointer>
#include <modbus/modbus.h>
#include <stdint.h>

ModbusRegisterListModel::ModbusRegisterListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_modbusContext(0),
    m_registers(new uint8_t[MODBUS_MAX_READ_BITS]),
    m_registerType(COILS),
    m_outputType(INT16),
    m_registerReadAddr(0),
    m_registerReadSize(10),
    m_connected(false)
{
}

bool ModbusRegisterListModel::connectToServer(const QString &ip, int port)
{
    if (m_connected)
        disconnect();

    m_modbusContext = modbus_new_tcp(ip.toStdString().c_str(), port);

    if (m_modbusContext == 0)
        return false;

    if (modbus_connect(m_modbusContext) == -1)
        return false;

    m_connected = true;

    return true;
}

bool ModbusRegisterListModel::isConnected() const
{
    return m_connected;
}

void ModbusRegisterListModel::disconnect()
{
    if (m_modbusContext) {
        modbus_close(m_modbusContext);
        modbus_free(m_modbusContext);
    }

    m_modbusContext = 0;
}

ModbusRegisterListModel::~ModbusRegisterListModel()
{
    disconnect();
}

ModbusRegisterListModel::RegisterType ModbusRegisterListModel::registerType() const
{
    return m_registerType;
}

void ModbusRegisterListModel::setRegisterType(RegisterType datatype)
{
    m_registerType = datatype;
}


int ModbusRegisterListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (m_registerType == COILS || m_registerType == INPUTS)
        return m_numRead;

    switch (m_outputType) {
    case INT16:
        return m_numRead;

    case INT32:
    case FLOAT:
    case FLIPPED_FLOAT:
        return m_numRead / 2;

    case DOUBLE:
        return m_numRead / 4;

    default:
        return m_numRead;
    }
}

QVariant ModbusRegisterListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() > m_numRead)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (m_registerType) {
        case COILS:
        case INPUTS:
            return m_registers[index.row()];

        case REGISTERS: {
            if (m_outputType == INT16) {
                uint16_t *reg16 = reinterpret_cast<uint16_t *>(m_registers.data());
                return reg16[index.row()];
            } else if (m_outputType == INT32) {
                uint32_t *reg32 = reinterpret_cast<uint32_t *>(m_registers.data());
                return reg32[index.row()];
            } else if (m_outputType == FLOAT) {
                float *floattype = reinterpret_cast<float *>(m_registers.data());
                return floattype[index.row()];
            } else if (m_outputType == DOUBLE) {
                double *doubletype = reinterpret_cast<double *>(m_registers.data());
                return doubletype[index.row()];
            }
        }
        }
    }

    return QVariant();
}

void ModbusRegisterListModel::readRegisters()
{
    beginResetModel();

    switch (m_registerType) {
    case COILS:
        m_numRead = modbus_read_bits(m_modbusContext, m_registerReadAddr, m_registerReadSize, m_registers.data());
        break;
    case INPUTS:
        m_numRead = modbus_read_input_bits(m_modbusContext, m_registerReadAddr, m_registerReadSize, m_registers.data());
        break;
    case REGISTERS:
        m_numRead = modbus_read_registers(m_modbusContext, m_registerReadAddr, m_registerReadSize, reinterpret_cast<uint16_t *>(m_registers.data()));
        break;


    default:
        break;
    }

    endResetModel();
}

int ModbusRegisterListModel::registerReadSize() const
{
    return m_registerReadSize;
}

void ModbusRegisterListModel::setRegisterReadSize(int registerReadSize)
{
    m_registerReadSize = registerReadSize;
}

int ModbusRegisterListModel::registerReadAddr() const
{
    return m_registerReadAddr;
}

void ModbusRegisterListModel::setRegisterReadAddr(int registerReadAddr)
{
    m_registerReadAddr = registerReadAddr;
}

ModbusRegisterListModel::OutputType ModbusRegisterListModel::outputType() const
{
    return m_outputType;
}

void ModbusRegisterListModel::setOutputType(OutputType outputType)
{
    m_outputType = outputType;
}

