#include "settingswindow.h"
#include "soundthread.h"
#include <QPainter>
#include <QMessageBox>
#include <QWhatsThis>
#include <QPushButton>
#include <QApplication>
#include <QSettings>
#include <QRadioButton>
#include <QTimer>

QSettingsDialog::QSettingsDialog(QWidget *parent) : QDialog(parent){
    createUI();
    newSett = true;
    createTextSettings();
    resize(445, 410);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    installEventFilter(this);

    dial = new QtColorDialog(this);
    dial->setModal(true);
    dial->setOption(QtColorDialog::NoButtons);
    QObject::connect(dial, &QtColorDialog::currentColorChanged, this, &QSettingsDialog::colorSelected);
    QObject::connect(dial, &QtColorDialog::aboutToClose, this, &QSettingsDialog::saveColor);

    cap = new QLabel("13. Cap color:       Color", this);
    cap->hide();

    cap_color = new QColorLabel(this);
    cap_color->setObjectName("cap");
    QObject::connect(cap_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);
    cap_color->hide();

    knot = new QLabel("13. Bow color:      Color", this);
    knot->hide();
    \
    knot_color = new QColorLabel(this);
    knot_color->setObjectName("knot");
    QObject::connect(knot_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);
    knot_color->hide();

    readSettingsFile();
    newSett = false;
}

void QSettingsDialog::createUI(){
    QLabel *back = new QLabel("1. Background:      Color", this);
    back->setGeometry(15, 10, 146, 20);

    back_color = new QColorLabel(this);
    back_color->setGeometry(176, 10, 50, 20);
    back_color->setObjectName("back");
    QObject::connect(back_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *displ = new QLabel("2. Display:            Color", this);
    displ->setGeometry(15, 40, 146, 20);

    displ_color = new QColorLabel(this);
    displ_color->setGeometry(176, 40, 50, 20);
    displ_color->setObjectName("display");
    QObject::connect(displ_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col1 = new QLabel("3. Column 1:         Color", this);
    col1->setGeometry(15, 70, 146, 20);

    col1_color = new QColorLabel(this);
    col1_color->setGeometry(176, 70, 50, 20);
    col1_color->setObjectName("col1");
    QObject::connect(col1_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col2 = new QLabel("4. Column 2:         Color", this);
    col2->setGeometry(15, 100, 146, 20);

    col2_color = new QColorLabel(this);
    col2_color->setGeometry(176, 100, 50, 20);
    col2_color->setObjectName("col2");
    QObject::connect(col2_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col3 = new QLabel("5. Column 3:         Color", this);
    col3->setGeometry(15, 130, 146, 20);
    QObject::connect(back_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    col3_color = new QColorLabel(this);
    col3_color->setGeometry(176, 130, 50, 20);
    col3_color->setObjectName("col3");
    QObject::connect(col3_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col4 = new QLabel("6. Column 4:         Color", this);
    col4->setGeometry(15, 160, 146, 20);

    col4_color = new QColorLabel(this);
    col4_color->setGeometry(176, 160, 50, 20);
    col4_color->setObjectName("col4");
    QObject::connect(col4_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col5 = new QLabel("7. Column 5:         Color", this);
    col5->setGeometry(15, 190, 146, 20);

    col5_color = new QColorLabel(this);
    col5_color->setGeometry(176, 190, 50, 20);
    col5_color->setObjectName("col5");
    QObject::connect(col5_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col6 = new QLabel("8. Column 6:         Color", this);
    col6->setGeometry(15, 220, 146, 20);

    col6_color = new QColorLabel(this);
    col6_color->setGeometry(176, 220, 50, 20);
    col6_color->setObjectName("col6");
    QObject::connect(col6_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col7 = new QLabel("9. Column 7:         Color", this);
    col7->setGeometry(15, 250, 146, 20);

    col7_color = new QColorLabel(this);
    col7_color->setGeometry(176, 250, 50, 20);
    col7_color->setObjectName("col7");
    QObject::connect(col7_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col8 = new QLabel("10. Column 8:       Color", this);
    col8->setGeometry(15, 280, 146, 20);

    col8_color = new QColorLabel(this);
    col8_color->setGeometry(176, 280, 50, 20);
    col8_color->setObjectName("col8");
    QObject::connect(col8_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col9 = new QLabel("11. Column 9:       Color", this);
    col9->setGeometry(15, 310, 146, 20);

    col9_color = new QColorLabel(this);
    col9_color->setGeometry(176, 310, 50, 20);
    col9_color->setObjectName("col9");
    QObject::connect(col9_color, &QColorLabel::clicked, this, &QSettingsDialog::labelClicked);

    QLabel *col10 = new QLabel("12. Gender:", this);
    col10->setGeometry(15, 340, 70, 20);

    QObject::connect(g_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, &QSettingsDialog::buttonChanged);

    QRadioButton *boy = new QRadioButton("boy", this);
    boy->move(100, 342);
    g_group->addButton(boy, 1);

    QRadioButton *girl = new QRadioButton("girl", this);
    girl->move(155, 342);
    g_group->addButton(girl, 2);

    QRadioButton *none = new QRadioButton("none", this);
    none->click();
    none->move(210, 342);
    g_group->addButton(none, 3);

    about_qt = new QPushButton("About Qt", this);
    QObject::connect(about_qt, &QPushButton::clicked, this, [this]{
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\set_click.wav"));
        thread->start();

        QMessageBox::aboutQt(this);
        thread->start();});

    restore_default = new QPushButton("Restore defaults", this);
    QObject::connect(restore_default, &QPushButton::clicked, this, &QSettingsDialog::restoreDefaults);
}

void QSettingsDialog::createTextSettings(){
    QLabel *back_text = new QLabel("Text Color:", this);
    back_text->setGeometry(246, 10, 116, 20);

    QRadioButton *back = new QRadioButton("N/A", this);
    back->move(320, 12);
    back->click();
    back->setDisabled(true);

    QLabel *displ_text = new QLabel("Text Color:", this);
    displ_text->setGeometry(246, 40, 116, 20);

    QRadioButton *displ1 = new QRadioButton("black", this);
    displ1->move(320, 42);
    displ_group->addButton(displ1, 1);

    QRadioButton *displ2 = new QRadioButton("white", this);
    displ2->move(380, 42);
    displ_group->addButton(displ2, 2);

    QLabel *col1_text = new QLabel("Text Color:", this);
    col1_text->setGeometry(246, 70, 116, 20);

    QRadioButton *col11 = new QRadioButton("black", this);
    col11->move(320, 72);
    col1_group->addButton(col11, 1);

    QRadioButton *col12 = new QRadioButton("white", this);
    col12->move(380, 72);
    col1_group->addButton(col12, 2);

    QLabel *col2_text = new QLabel("Text Color:", this);
    col2_text->setGeometry(246, 100, 116, 20);

    QRadioButton *col21 = new QRadioButton("black", this);
    col21->move(320, 102);
    col2_group->addButton(col21, 1);

    QRadioButton *col22 = new QRadioButton("white", this);
    col22->move(380, 102);
    col2_group->addButton(col22, 2);

    QLabel *col3_text = new QLabel("Text Color:", this);
    col3_text->setGeometry(246, 130, 116, 20);

    QRadioButton *col31 = new QRadioButton("black", this);
    col31->move(320, 132);
    col3_group->addButton(col31, 1);

    QRadioButton *col32 = new QRadioButton("white", this);
    col32->move(380, 132);
    col3_group->addButton(col32, 2);

    QLabel *col4_text = new QLabel("Text Color:", this);
    col4_text->setGeometry(246, 160, 116, 20);

    QRadioButton *col41 = new QRadioButton("black", this);
    col41->move(320, 162);
    col4_group->addButton(col41, 1);

    QRadioButton *col42 = new QRadioButton("white", this);
    col42->move(380, 162);
    col4_group->addButton(col42, 2);

    QLabel *col5_text = new QLabel("Text Color:", this);
    col5_text->setGeometry(246, 190, 116, 20);

    QRadioButton *col51 = new QRadioButton("black", this);
    col51->move(320, 192);
    col5_group->addButton(col51, 1);

    QRadioButton *col52 = new QRadioButton("white", this);
    col52->move(380, 192);
    col5_group->addButton(col52, 2);

    QLabel *col6_text = new QLabel("Text Color:", this);
    col6_text->setGeometry(246, 220, 116, 20);

    QRadioButton *col61 = new QRadioButton("black", this);
    col61->move(320, 222);
    col6_group->addButton(col61, 1);

    QRadioButton *col62 = new QRadioButton("white", this);
    col62->move(380, 222);
    col6_group->addButton(col62, 2);

    QLabel *col7_text = new QLabel("Text Color:", this);
    col7_text->setGeometry(246, 250, 116, 20);

    QRadioButton *col71 = new QRadioButton("black", this);
    col71->move(320, 252);
    col7_group->addButton(col71, 1);

    QRadioButton *col72 = new QRadioButton("white", this);
    col72->move(380, 252);
    col7_group->addButton(col72, 2);

    QLabel *col8_text = new QLabel("Text Color:", this);
    col8_text->setGeometry(246, 280, 116, 20);

    QRadioButton *col81 = new QRadioButton("black", this);
    col81->move(320, 282);
    col8_group->addButton(col81, 1);

    QRadioButton *col82 = new QRadioButton("white", this);
    col82->move(380, 282);
    col8_group->addButton(col82, 2);

    QLabel *col9_text = new QLabel("Text Color:", this);
    col9_text->setGeometry(246, 310, 116, 20);

    QRadioButton *col91 = new QRadioButton("black", this);
    col91->move(320, 312);
    col9_group->addButton(col91, 1);

    QRadioButton *col92 = new QRadioButton("white", this);
    col92->move(380, 312);
    col9_group->addButton(col92, 2);

    enable = new QCheckBox("Disable auto changing color \n               for text", this);
    QObject::connect(enable, &QCheckBox::stateChanged, this, &QSettingsDialog::enableButtons);

    QObject::connect(displ_group, QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("display", Qt::black); else emit textColorChanged("display", Qt::white);} else cFile = true;});
    QObject::connect(col1_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("col1", Qt::black); else emit textColorChanged("col1", Qt::white);} else cFile = true;});
    QObject::connect(col2_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("col2", Qt::black); else emit textColorChanged("col2", Qt::white);} else cFile = true;});
    QObject::connect(col3_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("col3", Qt::black); else emit textColorChanged("col3", Qt::white);} else cFile = true;});
    QObject::connect(col4_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("col4", Qt::black); else emit textColorChanged("col4", Qt::white);} else cFile = true;});
    QObject::connect(col5_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("col5", Qt::black); else emit textColorChanged("col5", Qt::white);} else cFile = true;});
    QObject::connect(col6_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("col6", Qt::black); else emit textColorChanged("col6", Qt::white);} else cFile = true;});
    QObject::connect(col7_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("col7", Qt::black); else emit textColorChanged("col7", Qt::white);} else cFile = true;});
    QObject::connect(col8_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("col8", Qt::black); else emit textColorChanged("col8", Qt::white);} else cFile = true;});
    QObject::connect(col9_group,  QOverload<int>::of(&QButtonGroup::buttonReleased), this, [=](int id){if(plSound){SoundThread *thread = new SoundThread;thread->setFileName(TEXT("sounds\\c_click.wav"));thread->start();plSound = false;QTimer::singleShot(300, this, [this]{plSound = true;});}if(cFile&&!newSett){if(id==1) emit textColorChanged("col9", Qt::black); else emit textColorChanged("col9", Qt::white);} else cFile = true;});
}

QString QSettingsDialog::getRandomString() const
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 5;

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

void QSettingsDialog::readSettingsFile(){
     QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
     settings.beginGroup("mainwindow");
     back_color->setColor(QColor(settings.value("backColor").toString()));
     displ_color->setColor(QColor(settings.value("displayColor").toString()));
     col1_color->setColor(QColor(settings.value("col1Color").toString()));
     col2_color->setColor(QColor(settings.value("col2Color").toString()));
     col3_color->setColor(QColor(settings.value("col3Color").toString()));
     col4_color->setColor(QColor(settings.value("col4Color").toString()));
     col5_color->setColor(QColor(settings.value("col5Color").toString()));
     col6_color->setColor(QColor(settings.value("col6Color").toString()));
     col7_color->setColor(QColor(settings.value("col7Color").toString()));
     col8_color->setColor(QColor(settings.value("col8Color").toString()));
     col9_color->setColor(QColor(settings.value("col9Color").toString()));
     cap_color->setColor(QColor(settings.value("capColor").toString()));
     knot_color->setColor(QColor(settings.value("knotColor").toString()));

     if(settings.value("automate").toString() == "true")
         automate = true;
     else
         automate = false;

     if(settings.value("displayTextColor").toString() == "#000000")
         displ_group->button(1)->click();
     else
         displ_group->button(2)->click();

     if(settings.value("col1TextColor").toString() == "#000000")
         col1_group->button(1)->click();
     else
         col1_group->button(2)->click();

     if(settings.value("col2TextColor").toString() == "#000000")
         col2_group->button(1)->click();
     else
         col2_group->button(2)->click();

     if(settings.value("col3TextColor").toString() == "#000000")
         col3_group->button(1)->click();
     else
         col3_group->button(2)->click();

     if(settings.value("col4TextColor").toString() == "#000000")
         col4_group->button(1)->click();
     else
         col4_group->button(2)->click();

     if(settings.value("col5TextColor").toString() == "#000000")
         col5_group->button(1)->click();
     else
         col5_group->button(2)->click();

     if(settings.value("col6TextColor").toString() == "#000000")
         col6_group->button(1)->click();
     else
         col6_group->button(2)->click();

     if(settings.value("col7TextColor").toString() == "#000000")
         col7_group->button(1)->click();
     else
         col7_group->button(2)->click();

     if(settings.value("col8TextColor").toString() == "#000000")
         col8_group->button(1)->click();
     else
         col8_group->button(2)->click();

     if(settings.value("col9TextColor").toString() == "#000000")
         col9_group->button(1)->click();
     else
         col9_group->button(2)->click();

     if(automate){
         displ_group->button(1)->setDisabled(true);
         displ_group->button(2)->setDisabled(true);

         col1_group->button(1)->setDisabled(true);
         col1_group->button(2)->setDisabled(true);

         col2_group->button(1)->setDisabled(true);
         col2_group->button(2)->setDisabled(true);

         col3_group->button(1)->setDisabled(true);
         col3_group->button(2)->setDisabled(true);

         col4_group->button(1)->setDisabled(true);
         col4_group->button(2)->setDisabled(true);

         col5_group->button(1)->setDisabled(true);
         col5_group->button(2)->setDisabled(true);

         col6_group->button(1)->setDisabled(true);
         col6_group->button(2)->setDisabled(true);

         col7_group->button(1)->setDisabled(true);
         col7_group->button(2)->setDisabled(true);

         col8_group->button(1)->setDisabled(true);
         col8_group->button(2)->setDisabled(true);

         col9_group->button(1)->setDisabled(true);
         col9_group->button(2)->setDisabled(true);
     }
     else
         enable->setCheckState(Qt::Checked);

     if(settings.value("boy").toString() == "true"){
         g_group->button(1)->click();
         buttonChanged(1);
     }
     else if(settings.value("girl").toString() == "true"){
         g_group->button(2)->click();
         buttonChanged(2);
     }
     else{
         g_group->button(3)->click();
         buttonChanged(3);
     }


     settings.endGroup();
     update();
}

void QSettingsDialog::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.fillRect(QRect(0, 0, rect().width(), rect().height() - 40), QColor(Qt::white));
}

void QSettingsDialog::labelClicked(){
    QColorLabel *label = qobject_cast<QColorLabel *>(sender());
    name = label->objectName();

    tempColor1 = label->color();
    tempColor = tempColor1;
    dial->setCurrentColor(label->color());
    dial->move(parentWidget()->pos().x() + 20, parentWidget()->pos().y() + 20);
    dial->show();
}

void QSettingsDialog::buttonChanged(int id){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\c_click.wav"));
        thread->start();
    }

   if(id == 1){
       emit genderChanged("boy");
       resize(445, 440);
       about_qt->move(147, 410);
       restore_default->move(10, 410);
       enable->move(260, 405);

       if(knot_color->isVisible()){
           knot->hide();
           knot_color->hide();
       }

       cap->setGeometry(15, 370, 166, 20);
       cap->show();

       cap_color->setGeometry(176, 370, 50, 20);
       cap_color->show();
   }
   else if(id == 2){
       emit genderChanged("girl");
       resize(445, 440);
       about_qt->move(147, 410);
       restore_default->move(10, 410);
       enable->move(260, 405);

       if(cap_color->isVisible()){
           cap->hide();
           cap_color->hide();
       }

       knot->setGeometry(15, 370, 166, 20);
       knot->show();

       knot_color->setGeometry(176, 370, 50, 20);
       knot_color->show();
   }
   else{
       emit genderChanged("none");

       if(cap_color->isVisible()){
            cap->hide();
            cap_color->hide();
       }

       if(knot_color->isVisible()){
            knot->hide();
            knot_color->hide();
       }

       resize(445, 410);
       about_qt->move(147, 380);
       restore_default->move(10, 380);
       enable->move(260, 375);
   }

}

void QSettingsDialog::enableButtons(int state){
    if(plSound){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\ch_click.wav"));
        thread->start();
    }

    parentWidget()->setProperty("cFile", false);
    QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    if(state == 2){
        automate = false;
        if(!newSett)
            settings.setValue("mainwindow/automate", "false");

        foreach (QAbstractButton *button, displ_group->buttons()) {
            button->setDisabled(false);
        }

        foreach (QAbstractButton *button, col1_group->buttons()) {
            button->setDisabled(false);
        }

        foreach (QAbstractButton *button,col2_group->buttons()) {
            button->setDisabled(false);
        }

        foreach (QAbstractButton *button, col3_group->buttons()) {
            button->setDisabled(false);
        }

        foreach (QAbstractButton *button, col4_group->buttons()) {
            button->setDisabled(false);
        }

        foreach (QAbstractButton *button, col5_group->buttons()) {
            button->setDisabled(false);
        }

        foreach (QAbstractButton *button, col6_group->buttons()) {
            button->setDisabled(false);
        }

        foreach (QAbstractButton *button, col7_group->buttons()) {
            button->setDisabled(false);
        }

        foreach (QAbstractButton *button, col8_group->buttons()) {
            button->setDisabled(false);

        }

        foreach (QAbstractButton *button, col9_group->buttons()) {
            button->setDisabled(false);
        }

    }
    else{
        automate = true;
        if(!newSett)
            settings.setValue("mainwindow/automate", "true");

        foreach (QAbstractButton *button, displ_group->buttons()) {
            button->setDisabled(true);
        }

        foreach (QAbstractButton *button, col1_group->buttons()) {
            button->setDisabled(true);
        }

        foreach (QAbstractButton *button,col2_group->buttons()) {
            button->setDisabled(true);
        }

        foreach (QAbstractButton *button, col3_group->buttons()) {
            button->setDisabled(true);
        }

        foreach (QAbstractButton *button, col4_group->buttons()) {
            button->setDisabled(true);
        }

        foreach (QAbstractButton *button, col5_group->buttons()) {
            button->setDisabled(true);
        }

        foreach (QAbstractButton *button, col6_group->buttons()) {
            button->setDisabled(true);
        }

        foreach (QAbstractButton *button, col7_group->buttons()) {
            button->setDisabled(true);
        }

        foreach (QAbstractButton *button, col8_group->buttons()) {
            button->setDisabled(true);
        }

        foreach (QAbstractButton *button, col9_group->buttons()) {
            button->setDisabled(true);
        }
    }
}

QColor QSettingsDialog::checkColor(QColor color){
    int lightness = color.lightness();
    if (lightness >= 135) {
        return Qt::black;
    }
    else {
        return Qt::white;
        }
}

void QSettingsDialog::colorSelected(const QColor &color){
    if(!dial->isHidden()){
        if(automate)
            emit colorReady(color, checkColor(color), name);

        tempColor = color;

        if(name == "back"){
            back_color->setColor(color);
            emit colorReady(color, QColor(), name);
        }
        else
            if(name == "display"){
                displ_color->setColor(color);
                if(!automate){
                    QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                    emit colorReady(color, QColor(settings.value("mainwindow/displayTextColor").toString()), name);
                }
            }
            else
                if(name == "col1"){
                    col1_color->setColor(color);
                    if(!automate){
                        QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                        emit colorReady(color, QColor(settings.value("mainwindow/col1TextColor").toString()), name);
                    }
                }
                else
                    if(name == "col2"){
                        col2_color->setColor(color);
                        if(!automate){
                            QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                            emit colorReady(color, QColor(settings.value("mainwindow/col2TextColor").toString()), name);
                        }
                    }
                    else
                        if(name == "col3"){
                            col3_color->setColor(color);
                            if(!automate){
                                QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                                emit colorReady(color, QColor(settings.value("mainwindow/col3TextColor").toString()), name);
                            }
                        }
                        else
                            if(name == "col4"){
                                col4_color->setColor(color);
                                if(!automate){
                                    QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                                    emit colorReady(color, QColor(settings.value("mainwindow/mainwindow/col4TextColor").toString()), name);
                                }
                            }
                            else
                                if(name == "col5"){
                                    col5_color->setColor(color);
                                    if(!automate){
                                        QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                                        emit colorReady(color, QColor(settings.value("mainwindow/col5TextColor").toString()), name);
                                    }
                                }
                                else
                                    if(name == "col6"){
                                        col6_color->setColor(color);
                                        if(!automate){
                                            QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                                            emit colorReady(color, QColor(settings.value("mainwindow/col6TextColor").toString()), name);
                                        }
                                    }
                                    else
                                        if(name == "col7"){
                                            col7_color->setColor(color);
                                            if(!automate){
                                                QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                                                emit colorReady(color, QColor(settings.value("mainwindow/col7TextColor").toString()), name);
                                            }
                                        }
                                        else
                                            if(name == "col8"){
                                                col8_color->setColor(color);
                                                if(!automate){
                                                    QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                                                    emit colorReady(color, QColor(settings.value("mainwindow/col8TextColor").toString()), name);
                                                }
                                            }
                                            else
                                                if(name == "col9"){
                                                    col9_color->setColor(color);
                                                    if(!automate){
                                                        QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
                                                        emit colorReady(color, QColor(settings.value("mainwindow/col9TextColor").toString()), name);
                                                    }
                                                }
                                                else
                                                    if(name == "cap"){
                                                        cap_color->setColor(color);
                                                        if(!automate)
                                                            emit colorReady(color, QColor(), name);
                                                    }
                                                    else{
                                                        knot_color->setColor(color);
                                                        if(!automate)
                                                            emit colorReady(color, QColor(), name);
                                                    }

        update();
    }
}

void QSettingsDialog::saveColor(){
    plSound = false;

    if(tempColor.isValid() && tempColor != tempColor1){
        parentWidget()->setProperty("cFile", false);
        cFile = false;

        QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
        if(name == "back")
            settings.setValue("mainwindow/backColor", tempColor.name());
        else
            if(name == "display"){
                settings.setValue("mainwindow/displayColor", tempColor.name());
                if(automate){
                    if(checkColor(tempColor) == Qt::black){
                        displ_group->button(1)->setDisabled(false);
                        displ_group->button(1)->click();
                        displ_group->button(1)->setDisabled(true);
                    }
                    else{
                        displ_group->button(2)->setDisabled(false);
                        displ_group->button(2)->click();
                        displ_group->button(2)->setDisabled(true);
                    }
                }
            }
            else
                if(name == "col1"){
                    settings.setValue("mainwindow/col1Color", tempColor.name());
                    if(automate){
                        if(checkColor(tempColor) == Qt::black){
                            col1_group->button(1)->setDisabled(false);
                            col1_group->button(1)->click();
                            col1_group->button(1)->setDisabled(true);
                        }
                        else{
                            col1_group->button(2)->setDisabled(false);
                            col1_group->button(2)->click();
                            col1_group->button(2)->setDisabled(true);
                        }
                    }
                }
                else
                    if(name == "col2"){
                        settings.setValue("mainwindow/col2Color", tempColor.name());
                        if(automate){
                            if(checkColor(tempColor) == Qt::black){
                                col2_group->button(1)->setDisabled(false);
                                col2_group->button(1)->click();
                                col2_group->button(1)->setDisabled(true);
                            }
                            else{
                                col2_group->button(2)->setDisabled(false);
                                col2_group->button(2)->click();
                                col2_group->button(2)->setDisabled(true);
                            }
                        }
                    }
                    else
                        if(name == "col3"){
                            settings.setValue("mainwindow/col3Color", tempColor.name());
                            if(automate){
                                if(checkColor(tempColor) == Qt::black){
                                    col3_group->button(1)->setDisabled(false);
                                    col3_group->button(1)->click();
                                    col3_group->button(1)->setDisabled(true);
                                }
                                else{
                                    col3_group->button(2)->setDisabled(false);
                                    col3_group->button(2)->click();
                                    col3_group->button(2)->setDisabled(true);
                                }
                            }
                        }
                        else
                            if(name == "col4"){
                                settings.setValue("mainwindow/col4Color", tempColor.name());
                                if(automate){
                                    if(checkColor(tempColor) == Qt::black){
                                        col4_group->button(1)->setDisabled(false);
                                        col4_group->button(1)->click();
                                        col4_group->button(1)->setDisabled(true);
                                    }
                                    else{
                                        col4_group->button(2)->setDisabled(false);
                                        col4_group->button(2)->click();
                                        col4_group->button(2)->setDisabled(true);
                                    }
                                }
                            }
                            else
                                if(name == "col5"){
                                    settings.setValue("mainwindow/col5Color", tempColor.name());
                                    if(automate){
                                        if(checkColor(tempColor) == Qt::black){
                                            col5_group->button(1)->setDisabled(false);
                                            col5_group->button(1)->click();
                                            col5_group->button(1)->setDisabled(true);
                                        }
                                        else{
                                            col5_group->button(2)->setDisabled(false);
                                            col5_group->button(2)->click();
                                            col5_group->button(2)->setDisabled(true);
                                        }
                                    }
                                }
                                else
                                    if(name == "col6"){
                                        settings.setValue("mainwindow/col6Color", tempColor.name());
                                        if(automate){
                                            if(checkColor(tempColor) == Qt::black){
                                                col6_group->button(1)->setDisabled(false);
                                                col6_group->button(1)->click();
                                                col6_group->button(1)->setDisabled(true);
                                            }
                                            else{
                                                col6_group->button(2)->setDisabled(false);
                                                col6_group->button(2)->click();
                                                col6_group->button(2)->setDisabled(true);
                                            }
                                        }
                                    }
                                    else
                                        if(name == "col7"){
                                            settings.setValue("mainwindow/col7Color", tempColor.name());
                                            if(automate){
                                                if(checkColor(tempColor) == Qt::black){
                                                    col7_group->button(1)->setDisabled(false);
                                                    col7_group->button(1)->click();
                                                    col7_group->button(1)->setDisabled(true);
                                                }
                                                else{
                                                    col7_group->button(2)->setDisabled(false);
                                                    col7_group->button(2)->click();
                                                    col7_group->button(2)->setDisabled(true);
                                                }
                                            }
                                        }
                                        else{
                                            if(name == "col8"){
                                                settings.setValue("mainwindow/col8Color", tempColor.name());
                                                if(automate){
                                                    if(checkColor(tempColor) == Qt::black){
                                                        col8_group->button(1)->setDisabled(false);
                                                        col8_group->button(1)->click();
                                                        col8_group->button(1)->setDisabled(true);
                                                    }
                                                    else{
                                                        col8_group->button(2)->setDisabled(false);
                                                        col8_group->button(2)->click();
                                                        col8_group->button(2)->setDisabled(true);
                                                    }
                                                }
                                            }
                                            else
                                                if(name == "col9"){
                                                    settings.setValue("mainwindow/col9Color", tempColor.name());
                                                    if(automate){
                                                        if(checkColor(tempColor) == Qt::black){
                                                            col9_group->button(1)->setDisabled(false);
                                                            col9_group->button(1)->click();
                                                            col9_group->button(1)->setDisabled(true);
                                                        }
                                                        else{
                                                            col9_group->button(2)->setDisabled(false);
                                                            col9_group->button(2)->click();
                                                                col9_group->button(2)->setDisabled(true);
                                                        }
                                                    }
                                                }
                                                else
                                                    if(name == "cap")
                                                        settings.setValue("mainwindow/capColor", tempColor.name());
                                                    else
                                                        settings.setValue("mainwindow/knotColor", tempColor.name());
                                        }
    }

    plSound = true;
}

bool QSettingsDialog::eventFilter(QObject *obj, QEvent *ev){
    if(obj == this && ev->type() == QEvent::EnterWhatsThisMode){
        SoundThread *thread = new SoundThread;
        thread->setFileName(TEXT("sounds\\set_click.wav"));
        thread->start();

        QWhatsThis::leaveWhatsThisMode();
        showHelpDialog();
        return true;
    }

    return QObject::eventFilter(obj, ev);
}

void QSettingsDialog::showHelpDialog(){
    QMessageBox::information(this, "Help", "\nHere you can choose your color preferences \nfor your calculator. Click the rectangle from the \nright of every name and choose your preffered color. \nEvery \"column color\" corresponds to a column of \nbuttons. Settings are saved automatically on your \ncomputer! \n\nMade with💖 by Emanuel Boatcă.");

    SoundThread *thread = new SoundThread;
    thread->setFileName(TEXT("sounds\\set_click.wav"));
    thread->start();
}

void QSettingsDialog::restoreDefaults(){
    SoundThread *thread = new SoundThread;
    thread->setFileName(TEXT("sounds\\set_click.wav"));
    thread->start();

    plSound = false;
    cFile = false;
    newSett = true;
    emit timeToRestore();
    back_color->setColor(QColor("#ca732c"));
    displ_color->setColor(QColor("#40E0D0"));
    col1_color->setColor(QColor("#8b58d3"));
    col2_color->setColor(QColor("#16825e"));
    col3_color->setColor(QColor("#0000ff"));
    col4_color->setColor(QColor("#00ff00"));
    col5_color->setColor(QColor("#ffff00"));
    col6_color->setColor(QColor("#ff7f00"));
    col7_color->setColor(QColor("#ff0000"));
    col8_color->setColor(QColor("#ffcc00"));
    col9_color->setColor(QColor("#92f6b2"));
    cap_color->setColor(QColor("#ED1C24"));
    knot_color->setColor(QColor("#F52887"));
    update();

    if(automate){
        displ_group->button(1)->setDisabled(false);
        displ_group->button(1)->click();
        displ_group->button(1)->setDisabled(true);
    }
    else
        displ_group->button(1)->click();

    if(automate){
        col1_group->button(1)->setDisabled(false);
        col1_group->button(1)->click();
        col1_group->button(1)->setDisabled(true);

        col2_group->button(1)->setDisabled(false);
        col2_group->button(1)->click();
        col2_group->button(1)->setDisabled(true);

        col3_group->button(1)->setDisabled(false);
        col3_group->button(1)->click();
        col3_group->button(1)->setDisabled(true);

        col4_group->button(1)->setDisabled(false);
        col4_group->button(1)->click();
        col4_group->button(1)->setDisabled(true);

        col5_group->button(1)->setDisabled(false);
        col5_group->button(1)->click();
        col5_group->button(1)->setDisabled(true);

        col6_group->button(1)->setDisabled(false);
        col6_group->button(1)->click();
        col6_group->button(1)->setDisabled(true);

        col7_group->button(1)->setDisabled(false);
        col7_group->button(1)->click();
        col7_group->button(1)->setDisabled(true);

        col8_group->button(1)->setDisabled(false);
        col8_group->button(1)->click();
        col8_group->button(1)->setDisabled(true);

        col9_group->button(1)->setDisabled(false);
        col9_group->button(1)->click();
        col9_group->button(1)->setDisabled(true);
    }
    else{
        automate = true;

        col1_group->button(1)->click();

        col2_group->button(1)->click();

        col3_group->button(1)->click();

        col4_group->button(1)->click();

        col5_group->button(1)->click();

        col6_group->button(1)->click();

        col7_group->button(1)->click();

        col8_group->button(1)->click();

        col9_group->button(1)->click();
    }

    enable->setCheckState(Qt::Unchecked);

    g_group->button(3)->click();
    buttonChanged(3);
    plSound = true;
    cFile = true;
    newSett = false;
}

void QSettingsDialog::showEvent(QShowEvent *ev){
    plSound = true;
    QDialog::showEvent(ev);
}

void QSettingsDialog::closeEvent(QCloseEvent *ev){
    SoundThread *thread = new SoundThread;
    thread->setFileName(TEXT("sounds\\set_click.wav"));
    thread->start();

    QDialog::closeEvent(ev);
}

QColorLabel::QColorLabel(QWidget *parent) : QLabel(parent){

}

void QColorLabel::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawRect(rect());
    painter.fillRect(rect() - QMargins(1, 1, 0, 0), col);
}

QColor QColorLabel::color(){
    return col;
}

void QColorLabel::setColor(QColor color){
    col = color;
}

void QColorLabel::mousePressEvent(QMouseEvent *){
    emit clicked();
}

QtColorDialog::QtColorDialog(QWidget *parent) : QColorDialog(parent){

}

void QtColorDialog::closeEvent(QCloseEvent *e){
    SoundThread *thread = new SoundThread;
    thread->setFileName(TEXT("sounds\\set_click.wav"));
    thread->start();

    QColorDialog::closeEvent(e);
    emit aboutToClose();
}
