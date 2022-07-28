#include "mainwindow.h"
#include "soundthread.h"
#include <QApplication>
#include <QFont>
#include <QMessageBox>

bool vectorContains(std::vector<std::string> v, std::string str){
    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i] == str) {
            return true;
        }
    }

    return false;
}

int main(int argl, char *argv[]){
    std::vector<std::string> v(argv, argv+argl);
    QApplication app(argl, argv);

    if(vectorContains(v, "-run-with-code:h350JKkedf893742KK@457&*(2isgrjphjkdjaf3290841#425%56|++2334")){
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QFont font = qApp->font();
        font.setPixelSize(13);
        qApp->setFont(font);

        mainWindow *window = new mainWindow;
        window->setFixedSize(785, 600);
        window->setWindowTitle("Calculator");
        window->setProcessId(QString(argv[2]).toUInt());
        window->show();
        window->activateWindow();

        return app.exec();
    }
    else{
        QMessageBox dial;
        dial.setWindowTitle("Info");
        dial.setText("Please run the app using calculator.exe executable!");
        dial.setIcon(QMessageBox::Information);
        dial.setStandardButtons(QMessageBox::Ok);
        dial.setModal(true);
        QApplication::beep();
        dial.exec();

        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\set_click.wav"));
        thread->start();
        thread->wait();

        return 0;
    }
}
