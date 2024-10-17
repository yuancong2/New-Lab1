#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 数字按钮连接
    connect(ui->btnNum0, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum1, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum2, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum3, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum4, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum5, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum6, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum7, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum8, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum9, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    // 二元操作符连接
    connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMultiple, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));

    // 一元操作符连接
    connect(ui->btnPercentage, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 计算函数
QString MainWindow::calculation(bool *ok)
{
    double result = operands.front().toDouble(); // 获取第一个操作数
    operands.pop_front();

    if (!operands.isEmpty() && !opcodes.isEmpty()) {

        QString op = opcodes.front();           // 获取操作符
        opcodes.pop_front();

        double operand2 = operands.front().toDouble(); // 获取第二个操作数
        operands.pop_front();

        if (op == "+") {
            result += operand2;
        } else if (op == "-") {
            result -= operand2;
        } else if (op == "×") {
            result *= operand2;
        } else if (op == "/") {
            if (operand2 != 0) {
                result /= operand2;
            } else {
                // 处理除以 0 的情况
                ui->statusbar->showMessage("Error: Division by zero");
                return "Error";
            }
        }
    }

    return QString::number(result);
}


// 数字按钮点击
void MainWindow::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton *>(sender())->text();

    // 处理重复的0
    if (digit == "0" && operand == "0")
        return;

    operand += digit;

    ui->display->setText(operand);
}

// 小数点处理
void MainWindow::on_btnPeriod_clicked()
{
    if (!operand.contains(".")) {
        operand += ".";
    }
    ui->display->setText(operand);
}

// 删除按钮
void MainWindow::on_btnDel_clicked()
{
    if (!operand.isEmpty()) {
        operand.chop(1); // 删除最后一位
    }
    ui->display->setText(operand);
}

// 清除所有输入
void MainWindow::on_btnClearAll_clicked()
{
    operand.clear();
    operands.clear();
    opcodes.clear();
    ui->display->clear();
}

// 二元操作符点击
void MainWindow::btnBinaryOperatorClicked()
{
    QString opcode = qobject_cast<QPushButton *>(sender())->text(); // 获取操作符
    qDebug() << opcode;

    if (operand != "") {
        operands.push_back(operand);  // 将当前操作数存入操作数队列
        operand = "";                 // 清空操作数以便输入下一个操作数

        if (operands.size() == 2) {
            // 如果已经有两个操作数，执行计算
            QString result = calculation(); // 调用计算函数

            ui->display->setText(result);  // 显示计算结果

            operands.clear();              // 清空操作数队列
            operands.push_back(result);    // 将计算结果作为新的第一个操作数
        }

        // 将操作符存入操作符队列
        opcodes.clear(); // 清空旧操作符，确保只存当前操作符
        opcodes.push_back(opcode);
    }
}

// 一元操作符点击
void MainWindow::btnUnaryOperatorClicked()
{
    // 如果当前操作数为空，使用显示屏上的数值作为操作数
    if (operand.isEmpty()) {
        operand = ui->display->text();
    }

    if (operand != "") {
        double result = operand.toDouble();  // 转换操作数
        operand = "";  // 清空操作数

        QString op = qobject_cast<QPushButton *>(sender())->text();  // 获取操作符
        qDebug() << op;

        if (op == "%")
            result /= 100.0;
        else if (op == "1/x")
            result = 1 / result;
        else if (op == "x^2")
            result *= result;
        else if (op == "√")
            result = sqrt(result);

        ui->display->setText(QString::number(result));  // 显示结果
        operand = QString::number(result);              // 将结果存入 operand，以便后续继续使用
    }
}


// 等号按钮点击
void MainWindow::on_btnEqual_clicked()
{
    if (operand != "") {
        operands.push_back(operand);  // 将最后一个操作数加入队列
        operand = "";
    }

    if (operands.size() == 2) {
        // 如果有两个操作数，执行计算
        QString result = calculation();
        if (result != "Error") {
            ui->display->setText(result);  // 显示最终结果
            operand = result;              // 将结果存储为新的操作数
        }

        // 清空操作数和操作符队列
        operands.clear();
        opcodes.clear();
    }
}


// 符号切换
void MainWindow::on_btnSign_clicked()
{
    if (!operand.isEmpty()) {
        double value = operand.toDouble();
        value = -value;
        operand = QString::number(value);
        ui->display->setText(operand);
    }
}
