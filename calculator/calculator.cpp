#include "calculator.h"

Calculator::Calculator(){

}

QString Calculator::equation(){
    return eq;
}

void Calculator::setEquation(QString equ){
    eq = equ;
}

QChar Calculator::token() {
    QChar ch = eq[i];
    i++;
    return ch;
}

double Calculator::factor() {
    double val = 0;
    QChar ch = token();
    if(ch == '(') {
        val = calculate();
        ch = token();
        if(ch != ')') {
            QString error =  QString("Expected ')', got: ") + ch;
            throw std::runtime_error(error.toStdString());
        }
    }
    else if(ch.isDigit()) {
        QString ss;
        while(ch.isDigit() || ch == '.' || ch == 'e'){
            ss += ch;
            if(ch == 'e'){
                ch = token();
                ss += ch;
            }
            ch = token();
        }

        val = ss.toDouble();
        i--;
    }
    else if(ch == 'i'){
        val = std::numeric_limits<double>::infinity();
        i += 3;
    }
    else if(ch == '-'){
        QString ss;
        ss += ch;
        ch = token();
        while(ch.isDigit() || ch == '.' || ch == 'e'){
            ss += ch;
            if(ch == 'e'){
                ch = token();
                ss += ch;
            }
            ch = token();
        }

        val = ss.toDouble();
        i--;
    }
    else throw std::runtime_error(QString("Unexpected QCharacter").toStdString());
    return val;
}

double Calculator::term() {
    QChar ch;
    double val = factor();
    ch = token();
    if(ch == 'x' || ch == '/' || ch == '%') {
        double b = term();
        if(ch == 'x')
            val *= b;
        else if(ch == '/')
            val /= b;
        else
            val = val - int(val/b) * b;
    }
    else i--;
    return val;
}

double Calculator::calculate() {
    double val = term();
    QChar ch = token();
    if(ch == '-' || ch =='+') {
        double b = calculate();
        if (ch == '+')
            val += b;
        else
            val -= b;
    }
    else i--;
    return val;
}
