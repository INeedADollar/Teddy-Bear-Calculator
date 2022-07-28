#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QLabel>
#include <QDialog>
#include <QColorDialog>
#include <QButtonGroup>
#include <QCheckBox>

class QtColorDialog : public QColorDialog{
    Q_OBJECT

public:
    QtColorDialog(QWidget *parent = nullptr);

private:
    void closeEvent(QCloseEvent *e) override;

signals:
    void aboutToClose();
};

class QColorLabel : public QLabel{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    QColorLabel(QWidget *parent = nullptr);
    QColor color();
    void setColor(QColor);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void clicked();

private:
    QColor col;
};

class QSettingsDialog : public QDialog{
    Q_OBJECT

public:
    QSettingsDialog(QWidget *parent = nullptr);

private:
    void createUI();
    void createTextSettings();
    void showHelpDialog();
    void readSettingsFile();
    QColor checkColor(QColor);
    QString getRandomString() const;

    QtColorDialog *dial;
    QColorLabel *back_color;
    QColorLabel *displ_color;
    QColorLabel *col1_color;
    QColorLabel *col2_color;
    QColorLabel *col3_color;
    QColorLabel *col4_color;
    QColorLabel *col5_color;
    QColorLabel *col6_color;
    QColorLabel *col7_color;
    QColorLabel *col8_color;
    QColorLabel *col9_color;
    QColorLabel *cap_color;
    QColorLabel *knot_color;
    QCheckBox *enable;

    QPushButton *about_qt;
    QPushButton *restore_default;

    QLabel *cap;
    QLabel *knot;

    QString name;
    QColor tempColor;
    QColor tempColor1;

    bool automate;
    bool plSound = false;
    bool cFile = true;
    bool newSett = false;

    QButtonGroup *displ_group = new QButtonGroup(this);
    QButtonGroup *col1_group = new QButtonGroup(this);
    QButtonGroup *col2_group = new QButtonGroup(this);
    QButtonGroup *col3_group = new QButtonGroup(this);
    QButtonGroup *col4_group = new QButtonGroup(this);
    QButtonGroup *col5_group = new QButtonGroup(this);
    QButtonGroup *col6_group = new QButtonGroup(this);
    QButtonGroup *col7_group = new QButtonGroup(this);
    QButtonGroup *col8_group = new QButtonGroup(this);
    QButtonGroup *col9_group = new QButtonGroup(this);
    QButtonGroup *g_group = new QButtonGroup(this);

protected:
    void paintEvent(QPaintEvent *) override;
    virtual bool eventFilter(QObject *, QEvent *) override;
    void showEvent(QShowEvent *) override;
    void closeEvent(QCloseEvent *) override;

private slots:
    void labelClicked();
    void colorSelected(const QColor &);
    void restoreDefaults();
    void saveColor();
    void enableButtons(int);
    void buttonChanged(int);

signals:
    void colorReady(QColor, QColor, QString);
    void timeToRestore();
    void textColorChanged(QString, QColor);
    void genderChanged(QString);
};

#endif // SETTINGSWINDOW_H
