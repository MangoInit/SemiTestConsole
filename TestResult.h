#ifndef TESTRESULT_H
#define TESTRESULT_H

#include <QDialog>
#include <QDebug>
#include "ui_dialog_test_result.h"
#include <QString>
namespace Ui {
class Dialog_test_result;
}

class TestResult : public QDialog
{
    Q_OBJECT

public:
    explicit TestResult(QWidget *parent = 0);
    ~TestResult();

    Ui::Dialog_test_result *ui;//private

signals:
    void signal_save_result(QString);

private slots:

private:
    void accept();
};

#endif // DIALOG_TEST_RESULT_H
