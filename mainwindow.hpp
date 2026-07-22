#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Declare a slot for each button
    void onFirstButtonClicked();
    void onSecondButtonClicked();
    void onThirdButtonClicked();

private:
    Ui::MainWindow *ui; // The 1:1 pointer to your UI components
};

