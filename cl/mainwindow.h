#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>
#include <QWidget>
#include <QDebug>

// #include "open_ai.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *m_btnEnter;
    QTextEdit *m_request;
    QTextEdit *m_answer;
    QGridLayout *m_grid;
    QWidget *m_centralWidger;

private slots:
    void onClick();

private:
    std::vector<std::string> chatHistory;
};

#endif // MAINWINDOW_H
