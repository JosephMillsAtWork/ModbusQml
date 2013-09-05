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

    Q_ENUMS(ModbusRegisterType)
    Q_ENUMS(OutputType)

    Q_PROPERTY(ModbusRegisterType modbusRegisterType READ modbusRegisterType WRITE setModbusRegisterType)
    Q_PROPERTY(OutputType outputType READ outputType WRITE setOutputType)
    Q_PROPERTY(int registerReadAddr READ registerReadAddr WRITE setRegisterReadAddr)
    Q_PROPERTY(int registerReadSize READ registerReadSize WRITE setRegisterReadSize)

public:

    enum ModbusRegisterType {
        Coil = 1,
        Input = 2,
        Register = 3
    };

    enum OutputType {
        Integer16,
        Integer32,
        Float,
        FlippedFloat,
        DoubleFloat
    };

    explicit ModbusRegisterListModel(QObject *parent = 0);

    Q_INVOKABLE bool connectToServer(const QString& ip, int port = 502);
    Q_INVOKABLE bool isConnected() const;

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    ~ModbusRegisterListModel();

    ModbusRegisterType modbusRegisterType() const;
    void setModbusRegisterType(ModbusRegisterType modbusRegisterType);

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
    ModbusRegisterType m_registerType;
    OutputType m_outputType;
    int m_registerReadAddr;
    int m_registerReadSize;
    bool m_connected;
};

#endif // MODBUSREGISTERLISTMODEL_H
