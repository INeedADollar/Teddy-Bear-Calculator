#include "mainwindow.h"
#include "calculator.h"
#include <processthread.h>
#include <QRegularExpression>
#include <QPainter>
#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>
#include <QTextStream>

mainWindow::mainWindow(QWidget *parent) : QWidget(parent, Qt::FramelessWindowHint){
    setAttribute(Qt::WA_TranslucentBackground);
    setProperty("cFile", true);

    HWND id = HWND(winId());
    DWORD style = DWORD(GetWindowLong(id, GWL_STYLE));
    SetWindowLong(id, GWL_STYLE, LONG(style) | WS_MINIMIZEBOX);
    createUI();

    knot = QImage(":/icons/funda.png");
    cap = QImage(":/icons/sapca.png");
    honey = QPixmap(":/icons/honey.png");
    bot = QImage(":/icons/bot.png");

    readSettingsFile();

    fileWatcher = new QFileSystemWatcher(this);
    fileWatcher->addPath(QApplication::applicationDirPath() + "/settings.ini");
    QObject::connect(fileWatcher, &QFileSystemWatcher::fileChanged, this, &mainWindow::verifyFile);

    dialog = new QSettingsDialog(this);
    dialog->setWindowTitle("Settings");
    dialog->setModal(true);
    QObject::connect(dialog , &QSettingsDialog::colorReady, this, &mainWindow::setNewColor);
    QObject::connect(dialog, &QSettingsDialog::timeToRestore, this, &mainWindow::restoreDefaults);
    QObject::connect(dialog, &QSettingsDialog::textColorChanged, this, &mainWindow::changeTextColor);
    QObject::connect(dialog, &QSettingsDialog::genderChanged, this, &mainWindow::genderChanged);

    fadeInAnimation = new QPropertyAnimation(this, "windowOpacity");
    fadeInAnimation->setDuration(350);
    fadeInAnimation->setStartValue(0.0);
    fadeInAnimation->setEndValue(1.0);
    QObject::connect(fadeInAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(repaint()));

    fadeOutAnimation = new QPropertyAnimation(this, "windowOpacity");
    fadeOutAnimation->setDuration(350);
    fadeOutAnimation->setStartValue(1.0);
    fadeOutAnimation->setEndValue(0.0);
    QObject::connect(fadeOutAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(repaint()));
    QObject::connect(fadeOutAnimation , &QPropertyAnimation::finished, this, &mainWindow::showMinimized);

    renderer = new ImageRenderer;
    timer = new QTimeLine(12000, this);
    timer->setFrameRange(0, 10000);
    QObject::connect(timer, &QTimeLine::frameChanged, this, &mainWindow::animateWindow);
    QObject::connect(timer, &QTimeLine::finished, this, [this]{
        renderer = new ImageRenderer;
        mini = false;
        but_e->show();
        but_par_l->show();
        but_par_r->show();
        but_mod->show();
        but_ce->show();
        but_c->show();
        but_del->show();
        but7->show();
        but8->show();
        but9->show();
        but_add->show();
        but_decr->show();
        but_mult->show();
        but_div->show();
        but4->show();
        but5->show();
        but6->show();
        but_pow_2->show();
        but_pow_3->show();
        but_pow_y->show();
        but_inv->show();
        but_log->show();
        but_ln->show();
        but_rad->show();
        but_fact->show();
        but_equ->show();
        but1->show();
        but2->show();
        but3->show();
        but_minoradd->show();
        but0->show();
        but_dot->show();
        display->show();
        ec_display->show();
        label1->show();
        label2->show();
        close->show();
        settings->show();
        minimize->show();
        repaint();
        showMinimized();
        nr ++;
    });

    setProperty("cFile", true);
}

void mainWindow::createUI(){
    ec_display = new TextEdit(this);
    ec_display->setGeometry(200, 130, 220, 50);
    ec_display->setStyleSheet("QTextEdit {border:none; background: transparent; color: grey;}");
    ec_display->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ec_display->setReadOnly(true);

    display = new LineEdit("0", this);
    display->setGeometry(160, 250, 290, 50);
    display->setFont(QFont("Sans-Serif", 20));
    display->setStyleSheet("QLineEdit {border: none; background: transparent;}");
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(19);
    display->setReadOnly(true);

    label1 = new QLabel(this);
    label1->setGeometry(114, 329, 225, 139);

    label2 = new QLabel(this);
    label2->setGeometry(381, 329, 130, 136);

    minimize = new MinimizeButton("-", this);
    minimize->setGeometry(281, 81, 18, 18);
    minimize->setStyleSheet("QPushButton {border: none; background: transparent; font: 15px sans-serif;} QPushButton::hover {background: rgb(208, 206, 206);}");
    QObject::connect(minimize, &MinimizeButton::buttonClicked, this, &mainWindow::buttonMinPressed);

    settings = new QPushButton(this);
    settings->setGeometry(299, 81, 22, 18);
    settings->setIcon(QIcon(":/icons/settings_icon.png"));
    settings->setIconSize(QSize(16, 16));
    settings->setStyleSheet("QPushButton {border: none; background: transparent; font: 15px sans-serif;} QPushButton::hover {background: rgb(208, 206, 206);}");
    QObject::connect(settings, &QPushButton::clicked, this, &mainWindow::showSettingsDialog);

    close = new QPushButton("x", this);
    close->setGeometry(321, 81, 18, 18);
    close->setStyleSheet("QPushButton {border: none; background: transparent; font: 15px sans-serif;} QPushButton::hover {background: red; color: white;}");
    QObject::connect(close, &QPushButton::clicked, this, &mainWindow::buttonClosePressed); //rgb(246, 121, 38)

    but_pow_2 = new QPushButton(this);
    but_pow_2->setGeometry(111, 340, 34, 30);
    but_pow_2->setIconSize(QSize(34, 30));
    QObject::connect(but_pow_2, &QPushButton::clicked, this, &mainWindow::buttonPower2Pressed);

    but_pow_3 = new QPushButton(this);
    but_pow_3->setGeometry(111, 375, 34, 30);
    but_pow_3->setIconSize(QSize(34, 30));
    QObject::connect(but_pow_3, &QPushButton::clicked, this, &mainWindow::buttonPower3Pressed);

    but_pow_y = new QPushButton(this);
    but_pow_y->setProperty("power", "true");
    but_pow_y->setGeometry(111, 410, 34, 30);
    but_pow_y->setIconSize(QSize(34, 30));
    QObject::connect(but_pow_y, &QPushButton::clicked, this, &mainWindow::buttonPowerYPressed);

    but_inv = new QPushButton(this);
    but_inv->setGeometry(111, 445, 34, 30);
    but_inv->setIconSize(QSize(34, 30));
    QObject::connect(but_inv, &QPushButton::clicked, this, &mainWindow::buttonInvPressed);

    but_log = new QPushButton("log", this);
    but_log->setGeometry(150, 410, 34, 30);
    QObject::connect(but_log, &QPushButton::clicked, this, &mainWindow::buttonLogPressed);

    but_ln = new QPushButton("ln", this);
    but_ln->setGeometry(187, 410, 35, 30);
    QObject::connect(but_ln, &QPushButton::clicked, this, &mainWindow::buttonLogPressed);

    but_e = new QPushButton("e", this);
    but_e->setGeometry(227, 410, 35, 30);
    QObject::connect(but_e, &QPushButton::clicked, this, &mainWindow::buttonePressed);

    but_rad = new QPushButton("√x", this);
    but_rad->setGeometry(267, 410, 35, 30);
    QObject::connect(but_rad, &QPushButton::clicked, this, &mainWindow::buttonRadPressed);

    but_mod = new QPushButton("%", this);
    but_mod->setGeometry(307, 410, 35, 30);
    QObject::connect(but_mod, &QPushButton::clicked, this, &mainWindow::operationButtonPressed);

    but_fact = new QPushButton("n!", this);
    but_fact->setGeometry(150, 445, 38, 30);
    QObject::connect(but_fact, &QPushButton::clicked, this, &mainWindow::buttonFactPressed);

    but_par_l = new QPushButton("(", this);
    but_par_l->setGeometry(194, 445, 38, 30);
    QObject::connect(but_par_l, &QPushButton::clicked, this, &mainWindow::buttonParLPressed);

    but_par_r = new QPushButton(")", this);
    but_par_r->setGeometry(238, 445, 38, 30);
    QObject::connect(but_par_r, &QPushButton::clicked, this, &mainWindow::buttonParRPressed);

    but_ce = new QPushButton("CE", this);
    but_ce->setGeometry(150, 340, 60, 30);
    but_ce->setFocus();
    QObject::connect(but_ce, &QPushButton::clicked, this, &mainWindow::buttonCEPressed);

    but_c = new QPushButton("C", this);
    but_c->setGeometry(215, 340, 60, 30);
    QObject::connect(but_c, &QPushButton::clicked, this, &mainWindow::buttonCPressed);

    but_del = new QPushButton(this);
    but_del->setGeometry(280, 340, 61, 30);
    but_del->setIcon(createClearButtonIcon());
    but_del->setIconSize(but_del->size());
    QObject::connect(but_del, &QPushButton::clicked, this, &mainWindow::buttonXPressed);

    but7 = new QPushButton("7", this);
    but7->setGeometry(380, 340, 40, 30);
    QObject::connect(but7, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but8 = new QPushButton("8", this);
    but8->setGeometry(425, 340, 40, 30);
    QObject::connect(but8, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but9 = new QPushButton("9", this);
    but9->setGeometry(470, 340, 40, 30);
    QObject::connect(but9, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but_add = new QPushButton("+", this);
    but_add->setGeometry(150, 375, 60, 30);
    QObject::connect(but_add, &QPushButton::clicked, this, &mainWindow::operationButtonPressed);

    but_decr = new QPushButton("-", this);
    but_decr->setGeometry(215, 375, 38, 30);
    QObject::connect(but_decr, &QPushButton::clicked, this, &mainWindow::operationButtonPressed);

    but_mult = new QPushButton("x", this);
    but_mult->setGeometry(259, 375, 38, 30);
    QObject::connect(but_mult, &QPushButton::clicked, this, &mainWindow::operationButtonPressed);

    but_div = new QPushButton("÷", this);
    but_div->setGeometry(303, 375, 38, 30);
    QObject::connect(but_div, &QPushButton::clicked, this, &mainWindow::operationButtonPressed);

    but4 = new QPushButton("4", this);
    but4->setGeometry(380, 375, 40, 30);
    QObject::connect(but4, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but5 = new QPushButton("5", this);
    but5->setGeometry(425, 375, 40, 30);
    QObject::connect(but5, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but6 = new QPushButton("6", this);
    but6->setGeometry(470, 375, 40, 30);
    QObject::connect(but6, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but_equ = new QPushButton("=", this);
    but_equ->setGeometry(282, 445, 59, 30);
    QObject::connect(but_equ, &QPushButton::clicked, this, &mainWindow::buttonEqualPressed);

    but1 = new QPushButton("1", this);
    but1->setGeometry(380, 410, 40, 30);
    QObject::connect(but1, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but2 = new QPushButton("2", this);
    but2->setGeometry(425, 410, 40, 30);
    QObject::connect(but2, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but3 = new QPushButton("3", this);
    but3->setGeometry(470, 410, 40, 30);
    QObject::connect(but3, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but_minoradd = new QPushButton("+/-" , this);
    but_minoradd->setGeometry(380, 445, 40, 30);
    QObject::connect(but_minoradd, &QPushButton::clicked, this, &mainWindow::buttonPlusMinusPressed);

    but0 = new QPushButton("0", this);
    but0->setGeometry(425, 445, 40, 30);
    QObject::connect(but0, &QPushButton::clicked, this, &mainWindow::updateDisplay);

    but_dot = new QPushButton(".", this);
    but_dot->setGeometry(470, 445, 40, 30);
    QObject::connect(but_dot, &QPushButton::clicked, this, &mainWindow::buttonDotPressed);
}

void mainWindow::readSettingsFile(){
    setProperty("cFile", false);

    QFile file(QApplication::applicationDirPath() + "/settings.ini");
    if(file.exists()){
        QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
        if(settings.value("DO_NOT_MODIFY_THIS_FILE_IF_YOU_DO_NOT_KNOW_HOW_TO_DO_IT_OR_FOR_WHAT_THIS_FILE_IS___YOUR_SETTINGS_WILL_BE_LOST", 0) == 0){
            settings.setValue("DO_NOT_MODIFY_THIS_FILE_IF_YOU_DO_NOT_KNOW_HOW_TO_DO_IT_OR_FOR_WHAT_THIS_FILE_IS___YOUR_SETTINGS_WILL_BE_LOST", "");
            file.open(QFile::ReadWrite);
            QString data = file.readAll();
            if(data.contains("[General]"))
                data.remove("[General]");

            QTextStream str(&file);
            str << "[General]\n" << "DO_NOT_MODIFY_THIS_FILE_IF_YOU_DO_NOT_KNOW_HOW_TO_DO_IT_OR_FOR_WHAT_THIS_FILE_IS___YOUR_SETTINGS_WILL_BE_LOST=" << data;
            file.close();
        }

        settings.beginGroup("mainwindow");
        if(settings.value("backColor").toString() != " " && QColor(settings.value("backColor").toString()).isValid())
            setBackgroundColor(QColor(settings.value("backColor").toString()));
        else{
            settings.setValue("backColor", "#ca732c");
            setBackgroundColor(QColor("#ca732c"));
        }

        if(!(settings.value("displayColor").toString() != " " && QColor(settings.value("displayColor").toString()).isValid()))
            settings.setValue("displayColor", "#40E0D0");

        if(settings.value("displayTextColor").toString() != " " && (QColor(settings.value("displayTextColor").toString()) == QColor("#000000") || QColor(settings.value("displayTextColor").toString()) == QColor("#ffffff")))
            setDisplayColor(QColor(settings.value("displayColor").toString()), QColor(settings.value("displayTextColor").toString()));
        else{
            setDisplayColor(QColor(settings.value("displayColor").toString()), Qt::black);
            settings.setValue("displayTextColor", QColor().name());
        }

        if(!(settings.value("col1Color").toString() != " " && QColor(settings.value("col1Color").toString()).isValid()))
            settings.setValue("col1Color", "#8b58d3");

        if(settings.value("col1TextColor").toString() != " " && (QColor(settings.value("col1TextColor").toString()) == QColor("#000000") || QColor(settings.value("col1TextColor").toString()) == QColor("#ffffff")))
            setCol1Color(QColor(settings.value("col1Color").toString()), QColor(settings.value("col1TextColor").toString()));
        else{
            setCol1Color(QColor(settings.value("col1Color").toString()), Qt::black);
            settings.setValue("col1TextColor", QColor().name());
        }

        if(!(settings.value("col2Color").toString() != " " && QColor(settings.value("col2Color").toString()).isValid()))
            settings.setValue("col2Color", "#16825e");

        if(settings.value("col2TextColor").toString() != " " && (QColor(settings.value("col2TextColor").toString()) == QColor("#000000") || QColor(settings.value("col2TextColor").toString()) == QColor("#ffffff")))
            setCol2Color(QColor(settings.value("col2Color").toString()), QColor(settings.value("col2TextColor").toString()));
        else{
            setCol2Color(QColor(settings.value("col2Color").toString()), Qt::black);
            settings.setValue("col2TextColor", QColor().name());
        }

        if(!(settings.value("col3Color").toString() != " " && QColor(settings.value("col3Color").toString()).isValid()))
            settings.setValue("col3Color", "#0000ff");

        if(settings.value("col3TextColor").toString() != " " && (QColor(settings.value("col3TextColor").toString()) == QColor("#000000") || QColor(settings.value("col3TextColor").toString()) == QColor("#ffffff")))
            setCol3Color(QColor(settings.value("col3Color").toString()), QColor(settings.value("col3TextColor").toString()));
        else{
            setCol3Color(QColor(settings.value("col3Color").toString()), Qt::black);
            settings.setValue("col3TextColor", QColor().name());
        }

        if(!(settings.value("col4Color").toString() != " " && QColor(settings.value("col4Color").toString()).isValid()))
            settings.setValue("col4Color", "#00ff00");

        if(settings.value("col4TextColor").toString() != " " && (QColor(settings.value("col4TextColor").toString()) == QColor("#000000") || QColor(settings.value("col4TextColor").toString()) == QColor("#ffffff")))
            setCol4Color(QColor(settings.value("col4Color").toString()), QColor(settings.value("col4TextColor").toString()));
        else{
            setCol4Color(QColor(settings.value("col4Color").toString()), Qt::black);
            settings.setValue("col4TextColor", QColor().name());
        }

        if(!(settings.value("col5TextColor").toString() != " " && QColor(settings.value("col5TextColor").toString()).isValid()))
            settings.setValue("col5Color", "#ffff00");

        if(settings.value("col5TextColor").toString() != " " && (QColor(settings.value("col5TextColor").toString()) == QColor("#000000") || QColor(settings.value("col5TextColor").toString()) == QColor("#ffffff")))
            setCol5Color(QColor(settings.value("col5Color").toString()), QColor(settings.value("col5TextColor").toString()));
        else{
            setCol5Color(QColor(settings.value("col5Color").toString()), Qt::black);
            settings.setValue("col5TextColor", QColor().name());
        }

        if(!(settings.value("col6Color").toString() != " " && QColor(settings.value("col6Color").toString()).isValid()))
            settings.setValue("col6Color", "#ff7f00");

        if(settings.value("col6TextColor").toString() != " " && (QColor(settings.value("col6TextColor").toString()) == QColor("#000000") || QColor(settings.value("col6TextColor").toString()) == QColor("#ffffff")))
            setCol6Color(QColor(settings.value("col6Color").toString()), QColor(settings.value("col6TextColor").toString()));
        else{
            setCol6Color(QColor(settings.value("col6Color").toString()), Qt::black);
            settings.setValue("col6TextColor", QColor().name());
        }

        if(!(settings.value("col7Color").toString() != " " && QColor(settings.value("col7Color").toString()).isValid()))
            settings.setValue("col7Color", "#ff0000");

        if(settings.value("col7TextColor").toString() != " " && (QColor(settings.value("col7TextColor").toString()) == QColor("#000000") || QColor(settings.value("col7TextColor").toString()) == QColor("#ffffff")))
            setCol7Color(QColor(settings.value("col7Color").toString()), QColor(settings.value("col7TextColor").toString()));
        else{
            setCol7Color(QColor(settings.value("col7Color").toString()), Qt::black);
            settings.setValue("col7TextColor", QColor().name());
        }

        if(!(settings.value("col8Color").toString() != " " && QColor(settings.value("col8Color").toString()).isValid()))
            settings.setValue("col8Color", "#ffcc00");

        if(settings.value("col8TextColor").toString() != " " && (QColor(settings.value("col8TextColor").toString()) == QColor("#000000") || QColor(settings.value("col8TextColor").toString()) == QColor("#ffffff")))
            setCol8Color(QColor(settings.value("col8Color").toString()), QColor(settings.value("col8TextColor").toString()));
        else{
            setCol8Color(QColor(settings.value("col8Color").toString()), Qt::black);
            settings.setValue("col8TextColor", QColor().name());
        }

        if(!(settings.value("col9Color").toString() != " " && QColor(settings.value("col9Color").toString()).isValid()))
            settings.setValue("col9Color", "#92f6b2");

        if(settings.value("col9TextColor").toString() != " " && (QColor(settings.value("col9TextColor").toString()) == QColor("#000000") || QColor(settings.value("col9TextColor").toString()) == QColor("#ffffff")))
            setCol9Color(QColor(settings.value("col9Color").toString()), QColor(settings.value("col9TextColor").toString()));
        else{
            setCol9Color(QColor(settings.value("col9Color").toString()), Qt::black);
            settings.setValue("col9TextColor", QColor().name());
        }

        if(!(settings.value("automate").toString() == "true" || settings.value("automate").toString() == "false"))
            settings.setValue("automate", "true");

        if(settings.value("boy").toString() == "true")
            gender = "boy";
        else if(settings.value("boy").toString() != "false")
                settings.setValue("boy", "false");

        if(settings.value("girl").toString() == "true" && gender != "boy")
            gender = "girl";
        else if(settings.value("girl").toString() != "false")
                settings.setValue("girl", "false");

        if(!(settings.value("none").toString() == "true" || settings.value("none").toString() == "false")){
            if(gender == "none")
                settings.setValue("none", "true");
            else
                settings.setValue("none", "false");
        }
        else
            if(gender == "none")
                settings.setValue("none", "true");
            else
                settings.setValue("none", "false");

        if(!(settings.value("capColor").toString() != " " && QColor(settings.value("capColor").toString()).isValid()))
            settings.setValue("capColor", "#ED1C24");

        changeImageColor(QColor(settings.value("capColor").toString()), "cap");

        if(!(settings.value("knotColor").toString() != " " && QColor(settings.value("knotColor").toString()).isValid()))
            settings.setValue("knotColor", "#F52887");

        changeImageColor(QColor(settings.value("knotColor").toString()), "knot");

        settings.endGroup();
        update();
    }
    else {
        QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
        settings.setValue("DO_NOT_MODIFY_THIS_FILE_IF_YOU_DO_NOT_KNOW_HOW_TO_DO_IT_OR_FOR_WHAT_THIS_FILE_IS___YOUR_SETTINGS_WILL_BE_LOST", "");
        settings.beginGroup("mainwindow");
        settings.setValue("backColor", "#ca732c");
        settings.setValue("displayColor", "#40E0D0");
        settings.setValue("col1Color", "#8b58d3");
        settings.setValue("col2Color", "#16825e");
        settings.setValue("col3Color", "#0000ff");
        settings.setValue("col4Color", "#00ff00");
        settings.setValue("col5Color", "#ffff00");
        settings.setValue("col6Color", "#ff7f00");
        settings.setValue("col7Color", "#ff0000");
        settings.setValue("col8Color", "#ffcc00");
        settings.setValue("col9Color", "#92f6b2");

        settings.setValue("displayTextColor","#000000");
        settings.setValue("col1TextColor", "#000000");
        settings.setValue("col2TextColor", "#000000");
        settings.setValue("col3TextColor", "#000000");
        settings.setValue("col4TextColor", "#000000");
        settings.setValue("col5TextColor", "#000000");
        settings.setValue("col6TextColor", "#000000");
        settings.setValue("col7TextColor", "#000000");
        settings.setValue("col8TextColor", "#000000");
        settings.setValue("col9TextColor", "#000000");

        settings.setValue("boy", "false");
        settings.setValue("girl", "false");
        settings.setValue("none", "true");
        settings.setValue("capColor", "#ED1C24");
        settings.setValue("knotColor", "#F52887");

        settings.setValue("automate", "true");
        settings.endGroup();

        setBackgroundColor(QColor("#ca732c"));
        setDisplayColor(QColor("#40E0D0"));
        setCol1Color(QColor("#8b58d3"));
        setCol2Color(QColor("#16825e"));
        setCol3Color(QColor("#0000ff"));
        setCol4Color(QColor("#00ff00"));
        setCol5Color(QColor("#ffff00"));
        setCol6Color(QColor("#ff7f00"));
        setCol7Color(QColor("#ff0000"));
        setCol8Color(QColor("#ffcc00"));
        setCol9Color(QColor("#92f6b2"));

        update();
    }
}

void mainWindow::verifyFile(const QString &path){
    if(!fileWatcher->files().contains(path)){
        QMessageBox dial(this);
        dial.setWindowTitle("File is missing");
        dial.setText("\nERROR: \"settings.ini\" file cannot be found on \nyour computer! The application will be restarted\nto solve this.\n");
        dial.setIcon(QMessageBox::Critical);
        dial.setStandardButtons(QMessageBox::Ok);
        dial.setModal(true);
        dial.move(pos().x() + 150, pos().y() + 200);
        QApplication::beep();
        dial.exec();

        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\set_click.wav"));
        thread->start();

        QObject::disconnect(fadeOutAnimation, &QPropertyAnimation::finished, this, &mainWindow::showMinimized);
        QObject::connect(fadeOutAnimation, &QPropertyAnimation::finished, this, []{QApplication::exit(10635);});
        fadeOutAnimation->start();
    }
    else if(property("cFile").toBool()){
        QMessageBox dial(this);
        dial.setWindowTitle("Settings File Modified");
        dial.setText("Settings file was modified. Restart now to apply changes?");
        dial.setIcon(QMessageBox::Question);
        dial.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        dial.setModal(true);
        dial.move(pos().x() + 120, pos().y() + 220);
        QApplication::beep();
        int code = dial.exec();

        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\set_click.wav"));
        thread->start();

        if(code == QMessageBox::Yes){
            QObject::disconnect(fadeOutAnimation, &QPropertyAnimation::finished, this, &mainWindow::showMinimized);
            QObject::connect(fadeOutAnimation, &QPropertyAnimation::finished, this, []{QApplication::exit(10635);});
            fadeOutAnimation->start();
        }      
    }
    else
        setProperty("cFile", true);
}

void mainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setClipRect(event->rect());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(Qt::transparent)));

    if(!mini){
        drawBear(painter);
        painter.setBrush(QBrush(QColor(63, 63, 63, 130)));
        painter.drawRect(QRect(280, 83, 63, 17));

        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(QRect(60, 102, 501, 201), 100, 100);
        path.addEllipse(QPoint(173, 306), 3, 6);
        path.addEllipse(QPoint(473, 306), 3, 6);
        painter.drawPath(path);

        painter.drawRoundedRect(QRect(60, 312, 501, 201), 100, 100);

         //QRect(349, 0, 60, 20)

        painter.setBrush(QBrush(back_color));//QColor(246, 121, 38)
        painter.setPen(QPen(Qt::black, 0.4));

        painter.drawRoundedRect(QRect(60, 100, 500, 200), 100, 100);
        painter.drawRoundedRect(QRect(60, 310, 500, 200), 100, 100);

        painter.setBrush(QBrush(Qt::black));
        painter.drawEllipse(QPoint(160, 150), 20, 20);
        painter.drawEllipse(QPoint(460, 150), 20, 20);

        painter.setBrush(QBrush(Qt::white));
        painter.drawEllipse(QPoint(160, 150), 7, 7);
        painter.drawEllipse(QPoint(460, 150), 7, 7);

        QPolygon poli2;
        poli2 << QPoint(210, 100) << QPoint(410, 100) << QPoint(460, 300) << QPoint(160, 300);

        painter.setBrush(QBrush(display_color)); //QColor(64, 224, 208)
        painter.drawPolygon(poli2);
        painter.drawImage(QRect(260, 170, 100, 100), bot, QRect(0, 0, 100, 100));

        if(gender == "boy"){
            painter.drawImage(QRect(160, -10, 200, 150), cap, QRect(0, 0, 200, 150));

            painter.setBrush(QBrush(QColor(Qt::white)));
            painter.setPen(QPen());
            painter.drawRect(QRect(420, 100, 60, 20));

            minimize->move(421, 101);
            settings->move(439, 101);
            close->move(461, 101);
        }

        if(gender == "girl"){
            painter.drawImage(QRect(235, 40, 150, 80), knot, QRect(0, 0, 150, 80));

            painter.setBrush(QBrush(QColor(Qt::white)));
            painter.setPen(QPen());
            painter.drawRect(QRect(420, 100, 60, 20));

            minimize->move(421, 101);
            settings->move(439, 101);
            close->move(461, 101);
        }

        if(gender == "none"){
            painter.setBrush(QBrush(QColor(Qt::white)));
            painter.setPen(QPen());
            painter.drawRect(QRect(280, 80, 60, 20));

            minimize->move(281, 81);
            settings->move(299, 81);
            close->move(321, 81);
        }

        painter.setBrush(QColor(192, 192, 192));
        painter.drawRect(QRect(150, 300, 20, 12));
        painter.drawEllipse(QPoint(150, 306), 3, 6);
        painter.drawEllipse(QPoint(170, 306), 3, 6);

        painter.drawRect(QRect(450, 300, 20, 12));
        painter.drawEllipse(QPoint(450, 306), 3, 6);
        painter.drawEllipse(QPoint(470, 306), 3, 6);

    }
    else{
        windowImage = renderer->renderImage(windowImage);
        painter.drawImage(QRect(QPoint(0, 0 ), QSize(823, 580)), windowImage, QRect(QPoint(0, 0 ), QSize(823, 580)));
    }
}

void mainWindow::drawBear(QPainter& painter){
    QRadialGradient gradient(QPoint(160, 100), 40);
    gradient.setColorAt(0, QColor(30, 19, 5));
    gradient.setColorAt(1, QColor(136, 88, 24));

    painter.setBrush(gradient);
    painter.drawEllipse(QPoint(160, 100), 50, 40);

    QRadialGradient gradient2(QPoint(460, 100), 40);
    gradient2.setColorAt(0, QColor(30, 19, 5));
    gradient2.setColorAt(1, QColor(136, 88, 24));

    painter.setBrush(gradient2);
    painter.drawEllipse(QPoint(460, 100), 50, 40);

    painter.setBrush(QColor(104, 67, 18));
    painter.drawRect(QRect(40, 360, 40, 100));
    painter.drawEllipse(QPoint(40, 410), 40, 50);

    painter.drawRect(QRect(540, 360, 40, 100));
    painter.drawEllipse(QPoint(580, 410), 40, 50);

    painter.drawRect(QRect(140, 510, 100, 20));
    painter.drawEllipse(QPoint(190, 530), 50, 40);

    painter.drawRect(QRect(380, 510, 100, 20));
    painter.drawEllipse(QPoint(430, 530), 50, 40);

    painter.setBrush(QColor(196, 152, 127));
    painter.drawEllipse(QPoint(35, 410), 10, 10);
    painter.drawEllipse(QPoint(13, 387), 7, 5);
    painter.drawEllipse(QPoint(8, 410), 7, 5);
    painter.drawEllipse(QPoint(13, 432), 7, 5);

    painter.drawEllipse(QPoint(190, 535), 10, 10);
    painter.drawEllipse(QPoint(167, 557), 5, 7);
    painter.drawEllipse(QPoint(190, 562), 5, 7);
    painter.drawEllipse(QPoint(213, 557), 5, 7);

    painter.drawEllipse(QPoint(430, 535), 10, 10);
    painter.drawEllipse(QPoint(407, 557), 5, 7);
    painter.drawEllipse(QPoint(430, 562), 5, 7);
    painter.drawEllipse(QPoint(453, 557), 5, 7);

    painter.drawPixmap(QRect(560, 300, 200, 200), honey, QRect(0, 0, 200, 200));
}

QIcon mainWindow::createClearButtonIcon(QColor color){
    QImage image(but_del->size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 2));

    QPolygon poligon;
    poligon << QPoint(30, 8) << QPoint(45, 8) << QPoint(45, 22) << QPoint(30, 22) << QPoint(15, 15);

    painter.drawPolygon(poligon);

    QPoint start1 = QPoint(32, 17);
    QPoint end1 = QPoint(38, 13);
    QPoint start2 = QPoint(38, 17);
    QPoint end2 = QPoint(32, 13);

    painter.drawLine(start1, end1);
    painter.drawLine(start2, end2);

    return QIcon(QPixmap::fromImage(image));
}

void mainWindow::buttonClosePressed(){
    SoundThread *thread = new SoundThread;
    thread->setFileName(TEXT("sounds\\set_click.wav"));
    thread->start();

    QObject::disconnect(fadeOutAnimation, &QPropertyAnimation::finished, this, &mainWindow::showMinimized);
    QObject::connect(fadeOutAnimation, &QPropertyAnimation::finished, this, []{QApplication::exit();});
    fadeOutAnimation->start();
}

void mainWindow::buttonMinPressed(){
    SoundThread *thread = new SoundThread;
    thread->setFileName(TEXT("sounds\\set_click.wav"));
    thread->start();

    windowImage = grab(QRect(QPoint(0, 0 ), QSize(823, 580))).toImage();

    but_e->hide();
    but_par_l->hide();
    but_par_r->hide();
    but_mod->hide();
    but_ce->hide();
    but_c->hide();
    but_del->hide();
    but7->hide();
    but8->hide();
    but9->hide();
    but_add->hide();
    but_decr->hide();
    but_mult->hide();
    but_div->hide();
    but4->hide();
    but5->hide();
    but6->hide();
    but_pow_2->hide();
    but_pow_3->hide();
    but_pow_y->hide();
    but_inv->hide();
    but_log->hide();
    but_ln->hide();
    but_rad->hide();
    but_fact->hide();
    but_equ->hide();
    but1->hide();
    but2->hide();
    but3->hide();
    but_minoradd->hide();
    but0->hide();
    but_dot->hide();
    display->hide();
    ec_display->hide();
    label1->hide();
    label2->hide();
    close->hide();
    settings->hide();

    mini = true;
    timer->start();

}

void mainWindow::animateWindow(int frame){
    if(frame >= 8900 && nr){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\bula-inceput.wav"));
        thread->start();
        nr --;
    }
    repaint();
}

void mainWindow::showSettingsDialog(){
    SoundThread *thread = new SoundThread;
    thread->setFileName(TEXT("sounds\\set_click.wav"));
    thread->start();

    dialog->move(pos().x() + 90, pos().y() + 45);
    dialog->show();
}

void mainWindow::genderChanged(QString g){
    setProperty("cFile", false);

    QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    settings.setValue(QString("mainwindow/%1").arg(g), "true");
    settings.setValue(QString("mainwindow/%1").arg(gender), "false");

    gender = g;
    repaint();
}

void mainWindow::setNewColor(QColor color, QColor textColor, QString name){
    if(name == "back"){
        setBackgroundColor(color);
        update();
    }
    else
        if(name == "display"){
            setDisplayColor(color, textColor);
            update();
        }
        else
            if(name == "col1")
                setCol1Color(color, textColor);
            else
                if(name == "col2")
                    setCol2Color(color, textColor);
                else
                    if(name == "col3")
                        setCol3Color(color, textColor);
                    else
                        if(name == "col4")
                            setCol4Color(color, textColor);
                        else
                            if(name == "col5")
                                setCol5Color(color, textColor);

                            else
                                if(name == "col6")
                                    setCol6Color(color, textColor);
                                else
                                    if(name == "col7")
                                        setCol7Color(color, textColor);
                                    else
                                        if(name == "col8")
                                            setCol8Color(color, textColor);
                                        else
                                            if(name == "col9")
                                                setCol9Color(color, textColor);
                                            else
                                                changeImageColor(color, name);
}

void mainWindow::restoreDefaults(){
    setProperty("cFile", false);
    fileWatcher->blockSignals(true);

    setCol1Color(QColor("#8b58d3"));
    setCol2Color(QColor("#16825e"));
    setCol3Color(QColor("#0000ff"));
    setCol4Color(QColor("#00ff00"));
    setCol5Color(QColor("#ffff00"));
    setCol6Color(QColor("#ff7f00"));
    setCol7Color(QColor("#ff0000"));
    setCol8Color(QColor("#ffcc00"));
    setCol9Color(QColor("#92f6b2"));

    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    settings.setValue("DO_NOT_MODIFY_THIS_FILE_IF_YOU_DO_NOT_KNOW_HOW_TO_DO_IT_OR_FOR_WHAT_THIS_FILE_IS___YOUR_SETTINGS_WILL_BE_LOST", "");
    settings.beginGroup("mainwindow");
    settings.setValue("backColor", "#ca732c");
    settings.setValue("displayColor", "#40E0D0");
    settings.setValue("col1Color", "#8b58d3");
    settings.setValue("col2Color", "#16825e");
    settings.setValue("col3Color", "#0000ff");
    settings.setValue("col4Color", "#00ff00");
    settings.setValue("col5Color", "#ffff00");
    settings.setValue("col6Color", "#ff7f00");
    settings.setValue("col7Color", "#ff0000");
    settings.setValue("col8Color", "#ffcc00");
    settings.setValue("col9Color", "#92f6b2");

    settings.setValue("backTextColor", "#000000");
    settings.setValue("displayTextColor","#000000");
    settings.setValue("col1TextColor", "#000000");
    settings.setValue("col2TextColor", "#000000");
    settings.setValue("col3TextColor", "#000000");
    settings.setValue("col4TextColor", "#000000");
    settings.setValue("col5TextColor", "#000000");
    settings.setValue("col6TextColor", "#000000");
    settings.setValue("col7TextColor", "#000000");
    settings.setValue("col8TextColor", "#000000");
    settings.setValue("col9TextColor", "#000000");

    settings.setValue(QString("%1").arg(gender), "false");
    settings.setValue("none", "true");
    settings.setValue("capColor", "#ED1C24");
    settings.setValue("knotColor", "#F52887");

    setBackgroundColor(QColor("#ca732c"));
    setDisplayColor(QColor("#40E0D0"));
    gender = "none";
    update();

    settings.setValue("automate", "true");
    settings.endGroup();

    changeImageColor(QColor("#ED1C24"), "cap");
    changeImageColor(QColor("#F52887"), "knot");

    QTimer::singleShot(2000,this, [this]{fileWatcher->blockSignals(false); setProperty("cFile", true);});
}

void mainWindow::changeTextColor(QString name, QColor textColor){
    setProperty("cFile", false);

    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    if(name == "display"){
        display->setStyleSheet(QString("QLineEdit {border: none; background: transparent; color: %1}").arg(textColor.name()));
        settings.setValue("mainwindow/displayTextColor", textColor.name());
    }
    else
        if(name == "col1"){
            setCol1Color(QColor(settings.value("mainwindow/col1Color").toString()), textColor);
            settings.setValue("mainwindow/col1TextColor", textColor.name());
        }
        else
            if(name == "col2"){
                setCol2Color(QColor(settings.value("mainwindow/col2Color").toString()), textColor);
                settings.setValue("mainwindow/col2TextColor", textColor.name());
            }
            else
                if(name == "col3"){
                    setCol3Color(QColor(settings.value("mainwindow/col3Color").toString()), textColor);
                    settings.setValue("mainwindow/col3TextColor", textColor.name());
                }
                else
                    if(name == "col4"){
                        setCol4Color(QColor(settings.value("mainwindow/col4Color").toString()), textColor);
                        settings.setValue("mainwindow/col4TextColor", textColor.name());
                    }
                    else
                        if(name == "col5"){
                            setCol5Color(QColor(settings.value("mainwindow/col5Color").toString()), textColor);
                            settings.setValue("mainwindow/col5TextColor", textColor.name());
                        }
                        else
                            if(name == "col6"){
                                setCol6Color(QColor(settings.value("mainwindow/col6Color").toString()), textColor);
                                settings.setValue("mainwindow/col6TextColor", textColor.name());
                            }
                            else
                                if(name == "col7"){
                                    setCol7Color(QColor(settings.value("mainwindow/col7Color").toString()), textColor);
                                    settings.setValue("mainwindow/col7TextColor", textColor.name());
                                }
                                else{
                                    if(name == "col8"){
                                        setCol8Color(QColor(settings.value("mainwindow/col8Color").toString()), textColor);
                                        settings.setValue("mainwindow/col8TextColor", textColor.name());
                                    }
                                    else{
                                        setCol9Color(QColor(settings.value("mainwindow/col9Color").toString()), textColor);
                                        settings.setValue("mainwindow/col9TextColor", textColor.name());
                                    }

                                }
}

void mainWindow::updateDisplay(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(NaN){
        ec_display->setText("");
        operationUsed = true;
        NaN = false;
        po = false;
    }

    if(!po || !isnan(base)){
        if(operationUsed || display->text() == "0"){
            display->setQLEText(qobject_cast<QPushButton *>(sender())->text());
            operationUsed = false;
        }
        else
            display->setQLEText(display->text() + qobject_cast<QPushButton *>(sender())->text());
    }
}
//^[+-]?\\d*\\.0*[1-9][0-9]*$

void mainWindow::operationButtonPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN){
        if(display->text().endsWith(".")){
            QString text = display->text();
            text.chop(1);
            display->setQLEText(text);
        }

        if(!isnan(base)){
            ec_display->setPlainText(ec_display->toPlainText() + display->text());
            display->setQLEText(QString::number(pow(base, display->text().remove(',').toDouble())));
            base = static_cast<double>(NAN);
        }

        QString text = qobject_cast<QPushButton *>(sender())->text();
        if(text == "÷")
            if(!par)
                equationStr += display->text().remove(',') + "/";
            else{
                equationStr += "/";
                par = false;
            }
        else if(!par)
                equationStr += display->text().remove(',') + text;
             else{
                equationStr += text;
                par = false;
            }

        if(!po){
            ec_display->setText(ec_display->toPlainText() + display->text() + text);
            ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());
        }
        else{
            po = false;
            ec_display->setText(ec_display->toPlainText() + text);
            ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

        }
        operationUsed = true;
    }
}

void mainWindow::buttonXPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN && !po && display->text() != "inf" && display->text() != "-inf"){
        QString text = display->text();
        text.chop(1);
        if(text != ""){
            display->setQLEText(text.remove(','));
        }
        else{
            display->setText("0");
        }
    }
}

void mainWindow::buttonCEPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!po){
        if(NaN){
            ec_display->setText("");
            NaN = false;
        }
        display->setText("0");
    }
}

void mainWindow::buttonCPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    display->setText("0");
    ec_display->setText("");
    po = false;
    par = false;
    equationStr = "";
    operationUsed = false;
}

void mainWindow::buttonPlusMinusPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN && !po && display->text() != "inf" && display->text() != "-inf"){
       if(display->text().startsWith("-") && display->text() != "0"){
           display->setQLEText(display->text().remove(0, 1));
       }
       else{
           if(display->text() != "0"){
               display->setQLEText("-" + display->text());
           }
       }
    }
}

void mainWindow::buttonDotPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!display->text().contains(".") && !NaN && display->text() != "inf" && display->text() != "-inf" && !po){
        QString text;
        if(display->text() == "")
            text = "0.";
        else
            text = display->text() + ".";

        display->setQLEText(text, true);
    }
}

void mainWindow::buttonPower2Pressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN){
        if(display->text().endsWith(".")){
            QString text = display->text();
            text.chop(1);
            display->setQLEText(text);
        }

        if(po){
            if(ch == ' '){
                int pos = ec_display->toPlainText().lastIndexOf(QRegularExpression("[\\+\\-x%÷]")) + 1;
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("sqr(%1)").arg(display->text()));
            }
            else{
                int pos = ec_display->toPlainText().lastIndexOf(ch);
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("sqr(%1)").arg(display->text()));

            }
        }
        else
            ec_display->setText(ec_display->toPlainText() + QString("sqr(%1)").arg(display->text()));
        ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

        operationUsed = true;
        double res = pow(display->text().remove(',').toDouble(), 2);

        po = true;
        ch = 's';
        display->setQLEText(QString::number(res).remove(','));
    }
}

void mainWindow::buttonPower3Pressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN){
        if(display->text().endsWith(".")){
            QString text = display->text();
            text.chop(1);
            display->setQLEText(text);
        }

        if(po){
            if(ch == ' '){
                int pos = ec_display->toPlainText().lastIndexOf(QRegularExpression("[\\+\\-x%÷]")) + 1;
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("cube(%1)").arg(display->text()));
            }
            else{
                int pos = ec_display->toPlainText().lastIndexOf(ch);
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("cube(%1)").arg(display->text()));

            }
        }
        else
            ec_display->setText(ec_display->toPlainText() + QString("cube(%1)").arg(display->text()));
        ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

        operationUsed = true;
        po = true;
        ch = 'c';
        display->setQLEText(QString::number(pow(display->text().remove(',').toDouble(), 3)));
    }
}

void mainWindow::buttonPowerYPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN){
        if(display->text().endsWith(".")){
            QString text = display->text();
            text.chop(1);
            display->setQLEText(text);
        }

        if(ec_display->toPlainText().endsWith("^"))
            base = pow(base, display->text().remove(',').toDouble());
        else
            base = display->text().remove(',').toDouble();

        if(po){
            if(ch == ' '){
                int pos = ec_display->toPlainText().lastIndexOf(QRegularExpression("[\\+\\-x%÷]"));
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("%1^").arg(display->text()));
            }
            else{
                int pos = ec_display->toPlainText().lastIndexOf(ch);
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("%1^").arg(display->text()));

            }
        }
        else
            ec_display->setText(ec_display->toPlainText() + display->text() + "^");
        ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

        po = true;
        operationUsed = true;
    }
}

void mainWindow::buttonInvPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN){
        if(display->text().endsWith(".")){
            QString text = display->text();
            text.chop(1);
            display->setQLEText(text);
        }

        if(po){
            if(ch == ' '){
                int pos = ec_display->toPlainText().lastIndexOf(QRegularExpression("[\\+\\-x%÷]"));
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("1/%1").arg(display->text()));
            }
            else{
                int pos = ec_display->toPlainText().lastIndexOf(ch);
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("1/%1").arg(display->text()));

            }
        }
        else
            ec_display->setText(ec_display->toPlainText() + QString("1/%1").arg(display->text()));
        ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

        double res = 1/display->text().remove(',').toDouble();
        if(isinf(res)){
            display->setText("Invalid input");
            NaN = true;
        }
        else{
            po = true;
            operationUsed = true;
            ch = '1';
            display->setQLEText(QString::number(res));
        }
    }
}

void mainWindow::buttonLogPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN){
        if(display->text().endsWith(".")){
            QString text = display->text();
            text.chop(1);
            display->setQLEText(text);
        }

        QString text = qobject_cast<QPushButton*>(sender())->text();
        double res;

        if(text == "log"){
            res = log10(display->text().remove(',').toDouble());
            if(po){
                if(ch == ' '){
                    int pos = ec_display->toPlainText().lastIndexOf(QRegularExpression("[\\+\\-x%÷]")) + 1;
                    ec_display->setText(ec_display->toPlainText().left(pos) + QString("log(%1)").arg(display->text()));
                }
                else{
                    int pos = ec_display->toPlainText().lastIndexOf(ch);
                    ec_display->setText(ec_display->toPlainText().left(pos) + QString("log(%1)").arg(display->text()));

                }
            }
            else
                ec_display->setText(ec_display->toPlainText() + QString("log(%1)").arg(display->text()));
            ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());
        }
        else{
            res = log(display->text().remove(',').toDouble());
            if(po){
                if(ch == ' '){
                    int pos = ec_display->toPlainText().lastIndexOf(QRegularExpression("[\\+\\-x%÷]")) + 1;
                    ec_display->setText(ec_display->toPlainText().left(pos) + QString("ln(%1)").arg(display->text()));
                }
                else{
                    int pos = ec_display->toPlainText().lastIndexOf(ch);
                    ec_display->setText(ec_display->toPlainText().left(pos) + QString("ln(%1)").arg(display->text()));

                }
            }
            else
                ec_display->setText(ec_display->toPlainText() + QString("ln(%1)").arg(display->text()));
            ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());
        }

        if(isnan(res)){
            display->setText("Invalid input");
            NaN = true;
        }
        else{
            display->setQLEText(QString::number(res));
            po = true;
            ch = 'l';
            operationUsed = true;
        }
    }
}

void mainWindow::buttonRadPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN){
        if(display->text().endsWith(".")){
            QString text = display->text();
            text.chop(1);
            display->setQLEText(text);
        }

        double res = sqrt(display->text().remove(',').toDouble());

        if(po){
            if(ch == ' '){
                int pos = ec_display->toPlainText().lastIndexOf(QRegularExpression("[\\+\\-x%÷]")) + 1;
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("√%1").arg(display->text()));
            }
            else{
                int pos = ec_display->toPlainText().lastIndexOf(ch);
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("√%1").arg(display->text()));

            }
        }
        else
            ec_display->setText(ec_display->toPlainText() + QString("√%1").arg(display->text()));
        ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

        if(isnan(res)){
            display->setText("Invalid input");
            NaN = true;
        }
        else{
            po = true;
            operationUsed = true;
            ch = "√";
            display->setQLEText(QString::number(res));
        }
    }
}

void mainWindow::buttonFactPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN){
        if(display->text().endsWith(".")){
            QString text = display->text();
            text.chop(1);
            display->setQLEText(text);
        }

        if(po){
            if(ch == ' '){
                int pos = ec_display->toPlainText().lastIndexOf(QRegularExpression("[\\+\\-x%÷]")) + 1;
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("fact(%1)").arg(display->text()));
            }
            else{
                int pos = ec_display->toPlainText().lastIndexOf(ch);
                ec_display->setText(ec_display->toPlainText().left(pos) + QString("fact(%1)").arg(display->text()));
            }
        }
        else
            ec_display->setText(ec_display->toPlainText() + QString("fact(%1)").arg(display->text()));
        ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

        double res = tgamma(display->text().remove(',').toDouble() + 1);
        if(isnan(res)){
            display->setText("Number too large");
            NaN = true;
        }
        else
            if(isinf(res)) {
                if(display->text().remove(',').toDouble() < 0)
                    display->setText("Invalid input");
                else
                    display->setText("Number too large");
                NaN = true;
            }
            else{
                display->setQLEText(QString::number(res));
                po = true;
                operationUsed = true;
                ch = "f";
            }
    }
}

void mainWindow::buttonePressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(NaN){
        ec_display->setText("");
        operationUsed = true;
        NaN = false;
    }

    display->setQLEText(QString::number(M_E));
}

void mainWindow::buttonParLPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(NaN){
        ec_display->setText("(");
        equationStr = "(";
        operationUsed = true;
        NaN = false;
        display->setText("0");
    }
    else{
        if(ec_display->toPlainText().endsWith(")") && !po){
            equationStr += "x(";
            ec_display->setText(ec_display->toPlainText() + "x(");
            ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

            par = false;
            po = false;
            operationUsed = true;
        }
        else{
            if(po){
                equationStr += display->text().remove(',') + "x(";
                ec_display->setText(ec_display->toPlainText() + "x(");
                ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

                po = false;
            }
            else{
                equationStr += "(";
                ec_display->setText(ec_display->toPlainText() + "(");
                ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

            }
        }
    }
    parNr++;
}

void mainWindow::buttonParRPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(parNr && !NaN){
        ec_display->setText(ec_display->toPlainText() + display->text() +  ")");
        ec_display->verticalScrollBar()->setValue(ec_display->verticalScrollBar()->maximum());

        equationStr += display->text() + ")";
        parNr--;
        po = true;
        par = true;
    }
}

void mainWindow::buttonEqualPressed(){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\click.wav"));
        thread->start();

        plSound = false;
        QTimer::singleShot(200, this, [this]{plSound = true;});
    }

    if(!NaN){
        if(!equationStr.endsWith(")")){
            if(!isnan(base))
                equationStr += QString::number(pow(base, display->text().remove(',').toDouble()));
        else
            equationStr += display->text().remove(',');
        }

        while(parNr){
            equationStr += ")";
            parNr--;
        }

        try{
            Calculator c;
            c.setEquation(equationStr);
            display->setQLEText(QString::number(c.calculate()));
        }
        catch(std::exception){
            display->setText("Error");
       }
    }

    else
        display->setText("0");



    ec_display->setText("");
    equationStr = "";  
    po = false;
    par = false;
    NaN = false;
    base = static_cast<double>(NAN);
    operationUsed = true;
}

void mainWindow::setBackgroundColor(QColor color){
    back_color = color;
    update();
}

void mainWindow::setDisplayColor(QColor color, QColor textColor){
    display_color = color;
    display->setStyleSheet(QString("QLineEdit {border: none; background: transparent; color: %1;}").arg(textColor.name()));
    update();
}

void mainWindow::setCol1Color(QColor color, QColor textColor){
    but_pow_2->setIcon(QIcon(QString(":/icons/x-2-") + textColor.name() + ".png"));
    but_pow_3->setIcon(QIcon(QString(":/icons/x-3-") + textColor.name() + ".png"));
    but_pow_y->setIcon(QIcon(QString(":/icons/x-y-") + textColor.name() + ".png"));
    but_inv->setIcon(QIcon(QString(":/icons/1x-") + textColor.name() + ".png"));

    but_pow_2->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background-color: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_pow_3->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background-color: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_pow_y->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background-color: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_inv->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background-color: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
}

void mainWindow::setCol2Color(QColor color, QColor textColor){
    but_ce->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_add->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_log->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_fact->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
}

void mainWindow::setCol3Color(QColor color, QColor textColor){
    but_ln->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_par_l->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
}

void mainWindow::setCol4Color(QColor color, QColor textColor){
    but_c->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_decr->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_e->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_par_r->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
}

void mainWindow::setCol5Color(QColor color, QColor textColor){
    but_mult->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_rad->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
}

void mainWindow::setCol6Color(QColor color, QColor textColor){
    but_del->setIcon(createClearButtonIcon(textColor));
    but_del->setIconSize(but_del->size());

    but_del->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_div->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_mod->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_equ->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
}

void mainWindow::setCol7Color(QColor color, QColor textColor){
    but7->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but4->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but1->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_minoradd->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
}

void mainWindow::setCol8Color(QColor color, QColor textColor){
    but8->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but5->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but2->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but0->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
 }

void mainWindow::setCol9Color(QColor color, QColor textColor){
    but9->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but6->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but3->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
    but_dot->setStyleSheet(QString("QPushButton {border: 1px solid black; border-radius: 6px; background: rgb(%1, %2, %3); color: %4; font: 20px sans-serif;} QPushButton::pressed {background: rgba(208, 206, 206, 75%);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(textColor.name()));
}

void mainWindow::changeImageColor(QColor color, QString name){
    if(color != QColor(0, 0, 0) && color != QColor("#040001") && color != QColor("#080002")){
        QColor pColor;
        if(name == "cap"){
            for(int i = 0; i <= cap.width() - 1; i++)
                for(int j = 0; j <= cap.height() - 1; j++){
                    pColor = cap.pixelColor(i, j);
                    if(pColor.alpha() != 0 && pColor != QColor(0, 0, 0) && pColor != QColor("#040001") && pColor != QColor("#080002"))
                        cap.setPixelColor(i, j, color);
            }
        }
        else{
            for(int i = 0; i <= knot.width() - 1; i++)
                for(int j = 0; j <= knot.height() - 1; j++){
                    pColor = knot.pixelColor(i, j);
                    if(pColor.alpha() != 0 && pColor != QColor(0, 0, 0))
                        knot.setPixelColor(i, j, color);
                }
        }
    }

    repaint();
}

void mainWindow::handleShortcut(){
    QMessageBox dial(this);
    dial.setWindowTitle("Close app");
    dial.setText("Are you sure?");
    dial.setIcon(QMessageBox::Question);
    dial.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    dial.setModal(true);
    dial.move(pos().x() + 230, pos().y() + 200);
    QApplication::beep();
    int code = dial.exec();
    if(code == QMessageBox::Yes){
        buttonClosePressed();
        return;
    }

    SoundThread *thread = new SoundThread;
    thread->setFileName(TEXT("sounds\\set_click.wav"));
    thread->start();
}

void mainWindow::setProcessId(DWORD pId){
    ProcessThread *thread = new ProcessThread(pId, this);
    thread->start();
}

void mainWindow::mousePressEvent(QMouseEvent *event){
    if(display->selectionLength() && !display->underMouse())
        display->deselect();

    if(event->buttons() == Qt::LeftButton && !label1->underMouse() && !label2->underMouse()){
        lastPosition = event->pos();
        isValid = true;
    }
}

void mainWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons().testFlag(Qt::LeftButton) && isValid)
        move(event->globalX()-lastPosition.x(), event->globalY()-lastPosition.y());
}

void mainWindow::mouseReleaseEvent(QMouseEvent *){
    isValid = false;

    if(display->selectionLength() && !display->underMouse())
        display->deselect();
}

void mainWindow::showEvent(QShowEvent *event){
    QWidget::showEvent(event);
    setWindowOpacity(0.0);
    fadeInAnimation->start();
}

void mainWindow::hideEvent(QHideEvent *event){
    QWidget::hideEvent(event);
    fadeOutAnimation->start();
}

bool mainWindow::nativeEvent(const QByteArray &, void *message, long *){
    MSG* msg = reinterpret_cast<MSG *>(message);
    if(msg->message == WM_SYSCOMMAND && msg->wParam == SC_MINIMIZE){
        buttonMinPressed();
        return true;
    }

    if(msg->message == WM_SYSKEYDOWN  && (GetKeyState(VK_F4) & 0x8000)){
        handleShortcut();
        return true;
    }

    return false;
}

void mainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
        case Qt::Key_0:
            but0->animateClick();
            break;
        case Qt::Key_1:
            but1->animateClick();
            break;
        case Qt::Key_2:
            but2->animateClick();
            break;
        case Qt::Key_3:
            but3->animateClick();
            break;
        case Qt::Key_4:
            but4->animateClick();
            break;
        case Qt::Key_5:
            but5->animateClick();
            break;
        case Qt::Key_6:
            but6->animateClick();
            break;
        case Qt::Key_7:
            but7->animateClick();
            break;
        case Qt::Key_8:
            but8->animateClick();
            break;
        case Qt::Key_9:
            but9->animateClick();
            break;
        case Qt::Key_Backspace:
            but_del->animateClick();
            break;
        case Qt::Key_Delete:
            but_c->animateClick();
            break;
        case Qt::Key_Plus:
            but_add->animateClick();
            break;
        case Qt::Key_Minus:
            but_decr->animateClick();
            break;
        case Qt::Key_Asterisk:
            but_mult->animateClick();
            break;
        case Qt::Key_Slash:
            but_div->animateClick();
            break;
        case Qt::Key_Percent:
            but_mod->animateClick();
            break;
        case Qt::Key_Equal:
            but_equ->animateClick();
            break;
        case Qt::Key_E:
            but_e->animateClick();
            break;
        case Qt::Key_Period:
            but_dot->animateClick();
            break;
        case Qt::Key_ParenLeft:
            but_par_l->animateClick();
            break;
        case Qt::Key_ParenRight:
            but_par_r->animateClick();
            break;
        case Qt::Key_Return:
            but_equ->animateClick();
            break;
    }

    QWidget::keyPressEvent(event);
}

MinimizeButton::MinimizeButton(QString text, QWidget *parent) : QPushButton(text, parent){

}

void MinimizeButton::mouseReleaseEvent(QMouseEvent *ev){
    QPushButton::mouseReleaseEvent(ev);
    QTimer::singleShot(100, this, [this]{
        emit buttonClicked();
    });
}

LineEdit::LineEdit(QString text, QWidget *parent) : QLineEdit(text, parent){
    setContextMenuPolicy(Qt::CustomContextMenu);

    menu = new QMenu(this);

    copyAct = new QAction("Copy");
    copyAct->setIcon(QIcon(":/icons/copy.png"));
    QObject::connect(copyAct, &QAction::triggered, this, &LineEdit::copy);
    menu->addAction(copyAct);
    menu->addSeparator();

    selAll = new QAction("Select All");
    selAll->setIcon(QIcon(":/icons/select-all.png"));
    QObject::connect(selAll, &QAction::triggered, this, &LineEdit::selectAll);
    menu->addAction(selAll);

    QObject::connect(this, &QTextEdit::customContextMenuRequested, this, &LineEdit::showContextMenu);
}

void LineEdit::showContextMenu(const QPoint &point){
    if(selectedText() == "")
        copyAct->setEnabled(false);
    else if(!copyAct->isEnabled())
        copyAct->setEnabled(true);

    if(selectedText() == text())
        selAll->setEnabled(false);
    else if(!selAll->isEnabled())
        selAll->setEnabled(true);

    menu->exec(mapToGlobal(point));
}

void LineEdit::setQLEText(QString text, bool virg){
    QString txt = "";
    if(!virg && text.size() < 20 && text != "inf" && text != "-inf"){
        QString sub = "";
        bool minus = false;

        if(text.startsWith('-')){
            minus = true;
            text.remove(0, 1);
        }

        if(text.contains(".")){
            int pos = text.lastIndexOf(".");
            sub = text.mid(pos, text.size() - 1);
            text.remove(pos, text.size() -  pos);
        }

        if(!text.contains('e')){
            int i = 0;
            for(int j = text.size() - 1; j >= 0; j--){
                if(text[j] != ',')
                    i++;
                else{
                    continue;
                }

                if(i%4 == 0 && i != 0){
                    txt = text[j] + "," + txt;
                    i = 1;
                }
                else
                    txt = text[j] + txt;
            }

            if(minus)
                txt = "-" + txt;

            if(sub != " ")
                txt += sub;
        }
    }

    if(txt == "")
        txt = text;

    setText(txt);
}

void LineEdit::mousePressEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton && event->pos().x() < (rect().width() - fontMetrics().horizontalAdvance(text()) - 10)){
        lastPosition = mapToParent(event->pos());
        isValid = true;
    }

    QLineEdit::mousePressEvent(event);
}

void LineEdit::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons().testFlag(Qt::LeftButton) && isValid)
        parentWidget()->move(event->globalX()-lastPosition.x(), event->globalY()-lastPosition.y());

    QLineEdit::mouseMoveEvent(event);
}

void LineEdit::mouseReleaseEvent(QMouseEvent *event){
    isValid = false;
    QLineEdit::mouseReleaseEvent(event);
}

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent){
    viewport()->setCursor(Qt::ArrowCursor);
    setTextInteractionFlags(Qt::NoTextInteraction);
    setContextMenuPolicy(Qt::CustomContextMenu);

    menu = new QMenu(this);

    copy = new QAction("Copy");
    copy->setIcon(QIcon(":/icons/copy.png"));
    QObject::connect(copy, &QAction::triggered, this, &TextEdit::copyText);
    menu->addAction(copy);

    QObject::connect(this, &QTextEdit::customContextMenuRequested, this, &TextEdit::showContextMenu);
}

void TextEdit::showContextMenu(const QPoint &point){
    if(toPlainText() == "")
        copy->setEnabled(false);
    else if(!copy->isEnabled())
        copy->setEnabled(true);

    menu->exec(mapToGlobal(point));
}

void TextEdit::copyText(){
    QApplication::clipboard()->setText(toPlainText());
}

void TextEdit::mousePressEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton){
        lastPosition = mapToParent(event->pos());
        isValid = true;
    }
}

void TextEdit::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons().testFlag(Qt::LeftButton) && isValid)
        parentWidget()->move(event->globalX()-lastPosition.x(), event->globalY()-lastPosition.y());
}

void TextEdit::mouseReleaseEvent(QMouseEvent *event){
    isValid = false;
    QTextEdit::mouseReleaseEvent(event);
}
