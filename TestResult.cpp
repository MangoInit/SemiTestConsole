#include "TestResult.h"
#include "ui_dialog_test_result.h"

TestResult::TestResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_test_result)
{
    ui->setupUi(this);
    this->setWindowTitle("测试结果汇总");

}

TestResult::~TestResult()
{
    delete ui;
}

void TestResult::accept()
{
    emit signal_save_result(ui->lineEdit->text());
    this->reject();
}
