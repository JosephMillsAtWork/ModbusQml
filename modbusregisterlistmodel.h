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

#ifndef MODBUSREGISTERLISTMODEL_H
#define MODBUSREGISTERLISTMODEL_H

#include <QAbstractListModel>
#include <stdint.h>

struct _modbus;
typedef _modbus modbus_t;

class ModbusRegisterListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(RegisterType)
    Q_ENUMS(OutputType)
public:

    enum RegisterType {
        COILS = 1,
        INPUTS = 2,
        REGISTERS = 3
    };

    enum OutputType {
        INT16,
        INT32,
        FLOAT,
        FLIPPED_FLOAT,
        DOUBLE
    };

    explicit ModbusRegisterListModel(QObject *parent = 0);

    bool connectToServer(const QString& ip, int port = 502);
    bool isConnected() const;


    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    ~ModbusRegisterListModel();

    RegisterType registerType() const;
    void setRegisterType(RegisterType registerType);

    OutputType outputType() const;
    void setOutputType(OutputType outputType);

    int registerReadAddr() const;
    void setRegisterReadAddr(int registerReadAddr);

    int registerReadSize() const;
    void setRegisterReadSize(int registerReadSize);

public slots:
    void readRegisters();
    void disconnect();

private:
    modbus_t* m_modbusContext;
    QScopedArrayPointer<uint8_t> m_registers;
    int m_numRead;
    RegisterType m_registerType;
    OutputType m_outputType;
    int m_registerReadAddr;
    int m_registerReadSize;
    bool m_connected;
};

#endif // MODBUSREGISTERLISTMODEL_H
