#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QString>
#include <QObject>

class Calculator{
    Q_PROPERTY(QString eq READ equation WRITE setEquation)

public:
    Calculator();
    double calculate();
    QString equation();
    void setEquation(QString);

private:
    int i = 0;
    QString eq;
    QChar token();
    double factor();
    double term();
};

#endif // CALCULATOR_H
