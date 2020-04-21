#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QMainWindow>

namespace Ui {
class NewWindow;
}

class NewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewWindow(QWidget *parent = 0);
    ~NewWindow();
    void paintEvent(QPaintEvent *);

    virtual NewWindow& operator<<(const std::string&);
    virtual NewWindow& operator<<(const char*);
    virtual NewWindow& operator<<(int);
    virtual NewWindow& operator<<(long);
    virtual NewWindow& operator<<(float);
    virtual NewWindow& operator<<(double);



private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_yes_clicked();

    void on_no_clicked();

    void mousePressEvent(QMouseEvent *);

    void on_pushButton_4_clicked();

    void on_ok_clicked();

    void on_interior_clicked();

    void on_exterior_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::NewWindow *ui;
};

#endif // NEWWINDOW_H
