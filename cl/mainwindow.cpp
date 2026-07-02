#include "mainwindow.h"
#include <open_ai.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this -> resize(600, 400);
    this -> setWindowTitle("Client");

    m_centralWidger = new QWidget;
    this -> setCentralWidget(m_centralWidger);

    m_grid = new QGridLayout(m_centralWidger);

    m_btnEnter = new QPushButton("Search");
    m_btnEnter -> setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed, QSizePolicy::ToolButton));

    m_request = new QTextEdit("");
    m_answer = new QTextEdit("");

    m_grid -> addWidget(m_request, 0, 0);
    m_grid -> addWidget(m_btnEnter, 0, 1, Qt::AlignTop);
    m_grid -> addWidget(m_answer, 1, 0, 1, 2);

    connect(m_btnEnter, &QPushButton::clicked, this, &MainWindow::onClick);
}

MainWindow::~MainWindow() {}

void MainWindow::onClick()
{
    // MainWindow::m_answer -> setText(MainWindow::m_request -> toPlainText());

    std::string apiKey = getApiKeyFromEnv();
    std::string model = "gpt-5.4-mini";
    // std::vector<std::string> chatHistory;
    std::string userInput;

    // std::cout << "Welcome to ChatGPT CLI! Type 'exit' to quit.\n";
    std::cout << "Step_01\n";

    // std::cout << "You: ";
    // std::getline(std::cin, userInput);
    userInput = (MainWindow::m_request -> toPlainText()).toStdString();
    std::cout << userInput + "\n";

    if (userInput == "exit") {
        return;
    }

    MainWindow::chatHistory.push_back("User: " + userInput);

    std::string prompt;
    for (const auto& message : MainWindow::chatHistory) {
        prompt += message + "\n";
    }

    std::string response = makeOpenAIRequest(prompt, apiKey, model);

    std::istringstream responseStream(response);

    Json::CharReaderBuilder readerBuilder;
    Json::Value jsonResponse;
    std::string errs;

    if (Json::parseFromStream(readerBuilder, responseStream, &jsonResponse, &errs)) {
        std::string reply = jsonResponse["choices"][0]["message"]["content"].asString();
        MainWindow::chatHistory.push_back("Assistant: " + reply);
        // std::cout << "Assistant: " << reply << "\n";
        MainWindow::m_answer -> setText(QString::fromStdString(reply));
    } else {
        // std::cerr << "Error parsing JSON response: " << errs << "\n";
        return;
    }

}
