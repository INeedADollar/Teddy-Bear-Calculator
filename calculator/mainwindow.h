#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingswindow.h"
#include "imagerenderer.h"
#include "soundthread.h"
#include <math.h>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMouseEvent>
#include <QFileSystemWatcher>
#include <QTextEdit>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QTimer>
#include <QMenu>

class MinimizeButton : public QPushButton{
    Q_OBJECT

public:
    MinimizeButton(QString text, QWidget *parent = nullptr);

private:
    void mouseReleaseEvent(QMouseEvent *ev) override;

signals:
    void buttonClicked();
};

class LineEdit : public QLineEdit{
    Q_OBJECT

public:
    LineEdit(QString text = "", QWidget *parent = nullptr);
    void setQLEText(QString, bool virg = false);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    void showContextMenu(const QPoint&);

    QMenu *menu;
    QAction *copyAct;
    QAction *selAll;

    QPoint lastPosition;
    bool isValid = false;
};

class TextEdit : public QTextEdit{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    void showContextMenu(const QPoint&);
    void copyText();

    QMenu *menu;
    QAction *copy;

    QPoint lastPosition;
    bool isValid = false;
};


class mainWindow :  public QWidget
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    void setBackgroundColor(QColor);
    void setProcessId(DWORD);

private:
    void createUI();
    void drawBear(QPainter&);
    QIcon createClearButtonIcon(QColor color = QColor());
    void readSettingsFile();

    void setDisplayColor(QColor, QColor textColor = QColor());
    void setCol1Color(QColor, QColor textColor = QColor());
    void setCol2Color(QColor, QColor textColor = QColor());
    void setCol3Color(QColor, QColor textColor = QColor());
    void setCol4Color(QColor, QColor textColor = QColor());
    void setCol5Color(QColor, QColor textColor = QColor());
    void setCol6Color(QColor, QColor textColor = QColor());
    void setCol7Color(QColor, QColor textColor = QColor());
    void setCol8Color(QColor, QColor textColor = QColor());
    void setCol9Color(QColor, QColor textColor = QColor());

    MinimizeButton *minimize;
    QPushButton *settings;
    QPushButton *close;
    QPushButton *but_e;
    QPushButton *but_par_l;
    QPushButton *but_par_r;
    QPushButton *but_mod;
    QPushButton *but_ce;
    QPushButton *but_c;
    QPushButton *but_del;
    QPushButton *but7;
    QPushButton *but8;
    QPushButton *but9;
    QPushButton *but_add;
    QPushButton *but_decr;
    QPushButton *but_mult;
    QPushButton *but_div;
    QPushButton *but4;
    QPushButton *but5;
    QPushButton *but6;
    QPushButton *but_pow_2;
    QPushButton *but_pow_3;
    QPushButton *but_pow_y;
    QPushButton *but_inv;
    QPushButton *but_log;
    QPushButton *but_ln;
    QPushButton *but_rad;
    QPushButton *but_fact;
    QPushButton *but_equ;
    QPushButton *but1;
    QPushButton *but2;
    QPushButton *but3;
    QPushButton *but_minoradd;
    QPushButton *but0;
    QPushButton *but_dot;
    LineEdit *display;
    TextEdit *ec_display;
    QLabel *label1;
    QLabel *label2;

    bool operationUsed = false;
    bool po = false;
    bool mini = false;
    bool NaN = false;
    bool par = false;
    bool plSound = true;
    bool fKeyReal = false;
    bool cFile = true;

    double base = static_cast<double>(NAN);
    int nr = 1;
    int parNr = 0;

    QString equationStr = "";
    QString ch = " ";
    QString gender = "none";

    QColor back_color;
    QColor display_color;
    QSettingsDialog *dialog;
    QStringList colorList;

    QFileSystemWatcher *fileWatcher;
    QPropertyAnimation* fadeInAnimation;
    QPropertyAnimation* fadeOutAnimation;

    QImage windowImage;
    QPixmap honey;
    QImage knot;
    QImage cap;
    QImage bot;
    ImageRenderer *renderer;

    QTimeLine *timer;
    QTimer s_timer;

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void showEvent(QShowEvent *) override;
    void hideEvent(QHideEvent *) override;
    bool nativeEvent(const QByteArray&, void*, long*) override;

    bool isValid = false;
    QPoint lastPosition;

private slots:
    void updateDisplay();
    void buttonClosePressed();
    void buttonMinPressed();
    void operationButtonPressed();
    void buttonEqualPressed();
    void buttonDotPressed();
    void buttonXPressed();
    void buttonCPressed();
    void buttonCEPressed();
    void buttonPower2Pressed();
    void buttonPower3Pressed();
    void buttonPowerYPressed();
    void buttonInvPressed();
    void buttonLogPressed();
    void buttonRadPressed();
    void buttonFactPressed();
    void buttonePressed();
    void buttonParLPressed();
    void buttonParRPressed();
    void buttonPlusMinusPressed();
    void showSettingsDialog();
    void setNewColor(QColor, QColor, QString);
    void restoreDefaults();
    void verifyFile(const QString &);
    void changeTextColor(QString, QColor);
    void animateWindow(int);
    void handleShortcut();
    void genderChanged(QString);
    void changeImageColor(QColor, QString);
};

#endif // MAINWINDOW_H
