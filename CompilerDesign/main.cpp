#include "lexer.h"
#include "parser.h"
#include "semanticanalysis.h"
#include "optimize.h"
#include "codegen.h"
#include "header.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

using namespace std;

QString readFileContent(const QString& filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        return in.readAll();
    }
    return "无法打开文件";
}

void init()
{
    delete w;
    w = new WordChart[MAXSIZE];
    len=-1;
    nlen=-1;

    delete IDs;
    IDs = new ID[MAXSIZE];

    delete Q;
    Q= new Quaternion[MAXSIZE];
    qlen=-1;
    nowQamount=0;
    nowNum=0;

    tlen=-1;
    e_len=-1;

    delete curr_idtype;
    curr_idtype = new char[10];

    // 使用 memset 将数组的所有元素设置为 '\0'
    memset(tree, '\0', sizeof(tree));

    regNum = 0;
    lbNum = 0;

    flag = 1;
}

void reCompile(QTextEdit *textEdit)
{
    init();

    // 步骤分析
    analy();
    textEdit->append("词法分析完毕.\n");

    Grammer();
    textEdit->append("语法分析完毕.\n");

    GraAnalyer();
    textEdit->append("递推式产生完毕.\n");

    yuyiAnalyer();
    textEdit->append("原四元式.\n");
    QString yuyiContent = readFileContent("D://d5.txt");
    textEdit->append(yuyiContent + "\n");

    optimizeQuaternion();
    textEdit->append("优化完毕.\n");

    yuyiAnalyer();
    textEdit->append("优化后四元式.\n");

    yuyiContent = readFileContent("D://d5.txt");
    textEdit->append(yuyiContent + "\n");

    huibian();
    textEdit->append("目标汇编语言代码生成完毕.\n");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 主窗口
    QWidget *mainWindow = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWindow);
    QWidget *widget1 = new QWidget;
    QWidget *widget2 = new QWidget;
    QHBoxLayout *layout1 = new QHBoxLayout(widget1);
    QHBoxLayout *layout2 = new QHBoxLayout(widget2);

    // 文本编辑区
    QTextEdit *textEdit1 = new QTextEdit;
    QTextEdit *textEdit = new QTextEdit;
    textEdit->setWindowTitle("File Analyzer");
    textEdit1->setWindowTitle("源代码");
    layout1->addWidget(textEdit1);
    layout1->addWidget(textEdit);

    mainLayout->addWidget(widget1);
    mainLayout->addWidget(widget2);

    // 读取源代码文件内容
    QString fileName = "D:/code.txt"; // 替换为你的文件路径
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString fileContent = in.readAll();
        textEdit1->setText(fileContent);
        file.close();
    } else {
        textEdit1->setText("无法打开文件");
    }

    // 添加 Ctrl + S 快捷键保存功能
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+S"), textEdit1);
    QObject::connect(shortcut, &QShortcut::activated, [=]() {
        QFile outFile(fileName);
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outFile);
            out << textEdit1->toPlainText();
            outFile.close();
            textEdit->append("File saved successfully.\n");
        } else {
            textEdit->append("Failed to save file.\n");
        }
    });


    // 步骤分析
    analy();
    textEdit->append("词法分析完毕.\n");

    Grammer();
    textEdit->append("语法分析完毕.\n");

    GraAnalyer();
    textEdit->append("递推式产生完毕.\n");

    yuyiAnalyer();
    textEdit->append("原四元式.\n");
    QString yuyiContent = readFileContent("D://d5.txt");
    textEdit->append(yuyiContent + "\n");

    optimizeQuaternion();
    textEdit->append("优化完毕.\n");

    yuyiAnalyer();
    textEdit->append("优化后四元式.\n");

    yuyiContent = readFileContent("D://d5.txt");
    textEdit->append(yuyiContent + "\n");

    huibian();
    textEdit->append("目标汇编语言代码生成完毕.\n");

    // 创建按钮
    QPushButton *btnRecompile = new QPushButton("重新编译");
    QPushButton *btnTokens = new QPushButton("显示tokens");
    QPushButton *btnIntermediateCode = new QPushButton("显示语法错误,准备中间代码生成");
    QPushButton *btnGraAnalyer = new QPushButton("打印递推式");
    QPushButton *btnYuyi = new QPushButton("打印四元式");
    QPushButton *btnHuibian = new QPushButton("打印汇编代码");
    QPushButton *btnResult = new QPushButton("打印模拟运行结果");

    // 将按钮添加到布局
    layout2->addWidget(btnRecompile);
    layout2->addWidget(btnTokens);
    layout2->addWidget(btnIntermediateCode);
    layout2->addWidget(btnGraAnalyer);
    layout2->addWidget(btnYuyi);
    layout2->addWidget(btnHuibian);
    layout2->addWidget(btnResult);

    // 按钮事件连接
    QObject::connect(btnRecompile, &QPushButton::clicked, [=]() {
        textEdit->clear();
        textEdit->append("重新编译.\n");
        reCompile(textEdit);
        textEdit->append("重编译完毕.\n");
    });

    QObject::connect(btnTokens, &QPushButton::clicked, [=]() {
        textEdit->append("显示tokens.\n");
        QString tokensContent = readFileContent("D://d2.txt");
        textEdit->append("Tokens:\n" + tokensContent + "\n");
    });

    QObject::connect(btnIntermediateCode, &QPushButton::clicked, [=]() {
        textEdit->append("显示具体错误.\n");
        QString intermediateContent = readFileContent("D://d3.txt");
        textEdit->append(intermediateContent + "\n");
    });

    QObject::connect(btnGraAnalyer, &QPushButton::clicked, [=]() {
        textEdit->append("打印递推式.\n");
        QString graAnalyerContent = readFileContent("D://d4.txt");
        textEdit->append(graAnalyerContent + "\n");
    });

    QObject::connect(btnYuyi, &QPushButton::clicked, [=]() {
        textEdit->append("打印四元式.\n");
        QString yuyiContent = readFileContent("D://d5.txt");
        textEdit->append(yuyiContent + "\n");
    });

    QObject::connect(btnHuibian, &QPushButton::clicked, [=]() {
        textEdit->append("打印汇编代码.\n");
        QString huibianContent = readFileContent("D://d6.txt");
        textEdit->append(huibianContent + "\n");
    });

    QObject::connect(btnResult, &QPushButton::clicked, [=]() {
        textEdit->append("打印运行结果.\n");
        textEdit->append(QString::fromStdString(emuRunning()));
    });

    // 设置主窗口布局和显示
    mainWindow->setLayout(mainLayout);
    mainWindow->resize(800, 600);
    mainWindow->show();

    return app.exec();
}
