#include <QProcess>
#include <QStringList>
#include <QApplication>
#include <QTimer>
#include <QMessageBox>
#include <windows.h>
#include <QFile>
#include <QDataStream>

static QByteArray origAr("4d5a90000300000004000000ffff0000b800000000000000400000000000000000000000000000000000000000000"
                         "000000000000000000000000000800000000e1fba0e00b409cd21b8014ccd21546869732070726f6772616d206361"
                         "6e6e6f742062652072756e20696e20444f53206d6f64652e0d0d0a24000000000000005045000064860a0068107e5"
                         "e0000000000000000f0002f020b02021e0010030000e40800000a0000b01400000010000000004000000000000010"
                         "00000002000004000000000000000500020000000000005009000004000075c809000200000000002000000000000"
                         "010000000000000000010000000000000100000000000000000000010000000000000000000000000000700147f00"
                         "0000a0070030a4010000800600d80f000000000000000000000000000000000000000000000000000000000000000"
                         "000000000000000000000004f06002800000000000000000000000000000000000000f41207001812000000000000"
                         "00000000000000000000000000000000000000002e74657874000000e00e030000100000001003000004000000000"
                         "0000000000000000000600050602e6461746100000040030000002003000004000000140300000000000000000000"
                         "000000400060c02e72646174610000b44103000030030000420300001803000000000000000000000000004000604"
                         "02e70646174610000d80f00000080060000100000005a0600000000000000000000000000400030402e7864617461"
                         "0000585300000090060000540000006a0600000000000000000000000000400030402e62737300000000800900000"
                         "0f006000000000000000000000000000000000000000000800060c02e69646174610000147f000000000700008000"
                         "0000be0600000000000000000000000000400030c02e43525400000000680000000080070000020000003e0700000"
                         "000000000000000000000400040c02e746c7300000000100000000090070000020000004007000000000000000000"
                         "00000000400040c02e7273726300000030a4010000a0070000a601000042070000000000000000000000000040003"
                         "0c0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "00000000000000000000000000000000000000000000000");

int main(int argl, char *argv[]){
    QApplication app(argl, argv);

    QFile file("cp.exe");

    if(file.exists()){
        QDataStream input(&file);
        file.open(QFile::ReadOnly);
        QByteArray buffer;
        char *temp = new char[1000];
        input.readRawData(temp, 1000);
        buffer.append(temp, 1000);
        delete [] temp;
        file.close();

        if(buffer.toHex() == origAr){
            QProcess *calcProc = new QProcess;
            calcProc->setProcessChannelMode(QProcess::MergedChannels);

            QStringList args;
            args << "-run-with-code:h350JKkedf893742KK@457&*(2isgrjphjkdjaf3290841#425%56|++2334" << QString::number(app.applicationPid());

            QObject::connect(&app, &QApplication::aboutToQuit, [calcProc]{if(calcProc->state() == QProcess::Running) calcProc->terminate();});
            QObject::connect(calcProc, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [calcProc, args](int exitCode, QProcess::ExitStatus status){
                if(exitCode == 10635 && status == QProcess::NormalExit)
                    QTimer::singleShot(1000, nullptr, [calcProc, args]{calcProc->start(QApplication::applicationDirPath() + "/cp.exe", args);});
                else
                    QApplication::exit();
            });

            QObject::connect(calcProc, &QProcess::errorOccurred, [calcProc, args](QProcess::ProcessError error){
                if(calcProc->state() == QProcess::Running)
                    calcProc->terminate();

                switch(error) {
                    case QProcess::FailedToStart:{
                        QMessageBox dial;
                        dial.setWindowTitle("Failed to start");
                        dial.setText("App failed to start!");
                        dial.setIcon(QMessageBox::Warning);
                        dial.setStandardButtons(QMessageBox::Ok);
                        dial.setModal(true);
                        QApplication::beep();
                        dial.exec();
                        break;
                    }
                    case QProcess::Crashed:{
                        QMessageBox dial;
                        dial.setWindowTitle("App crashed");
                        dial.setText("FATAL ERROR: App crashed!");
                        dial.setIcon(QMessageBox::Critical);
                        dial.setStandardButtons(QMessageBox::Ok);
                        dial.setModal(true);
                        QApplication::beep();
                        dial.exec();
                        break;
                    }
                    default:
                        QMessageBox dial;
                        dial.setWindowTitle("Error");
                        dial.setText("Error ocurred!");
                        dial.setIcon(QMessageBox::Warning);
                        dial.setStandardButtons(QMessageBox::Ok);
                        dial.setModal(true);
                        QApplication::beep();
                        dial.exec();
                        break;
                }

                PlaySound(TEXT("sounds\\set_click.wav"), nullptr, SND_NODEFAULT |SND_SYNC);
                QApplication::exit();
            });

            calcProc->start(QApplication::applicationDirPath() + "/cp.exe", args);

            if(calcProc->state() == QProcess::NotRunning)
                return 1;

            return app.exec();
        }
        else {
            QMessageBox dial;
            dial.setWindowTitle("Error");
            dial.setText("Please use original cp.exe executable provided by developer!");
            dial.setIcon(QMessageBox::Critical);
            dial.setStandardButtons(QMessageBox::Ok);
            dial.setModal(true);
            QApplication::beep();
            dial.exec();

            PlaySound(TEXT("sounds\\set_click.wav"), nullptr, SND_NODEFAULT |SND_SYNC);
            return 1;
        }
    }
    else {
        QMessageBox dial;
        dial.setWindowTitle("Error");
        dial.setText("cp.exe is missing from application directory path!");
        dial.setIcon(QMessageBox::Critical);
        dial.setStandardButtons(QMessageBox::Ok);
        dial.setModal(true);
        QApplication::beep();
        dial.exec();

        PlaySound(TEXT("sounds\\set_click.wav"), nullptr, SND_NODEFAULT |SND_SYNC);
        return 1;
    }
}
