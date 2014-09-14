#include "TicTacToeWidget.h"
#include "ui_TicTacToeWidget.h"

#include <QDebug>

TicTacToeWidget::TicTacToeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_buttonMapper(new QSignalMapper(this))
{
    ui->setupUi(this);
    m_playerString.first = "X";
    m_playerString.second = "O";

    m_buttons.insert(0, ui->pushButton);
    m_buttons.insert(1, ui->pushButton_2);
    m_buttons.insert(2, ui->pushButton_3);
    m_buttons.insert(3, ui->pushButton_4);
    m_buttons.insert(4, ui->pushButton_5);
    m_buttons.insert(5, ui->pushButton_6);
    m_buttons.insert(6, ui->pushButton_7);
    m_buttons.insert(7, ui->pushButton_8);
    m_buttons.insert(8, ui->pushButton_9);

    foreach(QPushButton *button, m_buttons)
    {
        if(!button)
        {
            continue;
        }

        QObject::connect(button, SIGNAL(clicked()), m_buttonMapper, SLOT(map()));

        m_buttonMapper->setMapping(button, m_buttons.key(button));
    }

    this->connect(m_buttonMapper, SIGNAL(mapped(int)), SLOT(updateButton(int)));
    this->reset();
}

TicTacToeWidget::~TicTacToeWidget()
{
    delete ui;
}

void TicTacToeWidget::reset()
{
    foreach(QPushButton *button, m_buttons)
    {
        button->setText("");
        button->setStyleSheet("");
        button->setEnabled(true);
    }

    ui->winnerLabel->hide();
    ui->restartButton->hide();
}

void TicTacToeWidget::switchPlayer()
{
    ::qSwap(m_playerString.first, m_playerString.second);
}

void TicTacToeWidget::setDraw()
{
    ui->winnerLabel->setText("Draw!");
    ui->restartButton->show();
}

void TicTacToeWidget::setWon(const QList<QPushButton*> &winningButtons)
{
    foreach(QPushButton *button, winningButtons)
    {
        button->setStyleSheet("background: lightblue;");
    }

    ui->restartButton->show();
    ui->winnerLabel->show();
    ui->winnerLabel->setText(QString("%1 won!").arg(winningButtons.first()->text()));

    foreach(QPushButton *button, m_buttons)
    {
        button->setEnabled(false);
    }
}

void TicTacToeWidget::updateButton(int buttonId)
{
    QPushButton *pushedButton = m_buttons.value(buttonId);
    if(!pushedButton)
    {
        return;
    }

    QPushButton &button = (*pushedButton);

    static int combinations[8][3] =
    {
        {0,1,2},
        {3,4,5},
        {6,7,8},

        {0,3,6},
        {1,4,7},
        {2,5,8},

        {0,4,8},
        {6,4,2}
    };

    if(button.text().isEmpty())
    {
        button.setText(m_playerString.first);
    }

    for(int combination = 0; combination < 8; ++combination)
    {
        const int firstKey = combinations[combination][0];
        const int secondKey = combinations[combination][1];
        const int thirdKey = combinations[combination][2];

        const QString first = m_buttons[firstKey]->text();
        const QString second = m_buttons[secondKey]->text();
        const QString third = m_buttons[thirdKey]->text();

        if(first.isEmpty() || second.isEmpty() || third.isEmpty())
        {
            continue;
        }

        if(first != second || second != third)
        {
            continue;
        }

        QList<QPushButton*> winningButtons;
        winningButtons << m_buttons[combinations[combination][0]];
        winningButtons << m_buttons[combinations[combination][1]];
        winningButtons << m_buttons[combinations[combination][2]];

        this->setWon(winningButtons);

        return;
    }

    if( std::all_of(m_buttons.cbegin(), m_buttons.cend(), [](const QPushButton * button) { return !button->text().isEmpty(); }) )
    {
        this->setDraw();
        return;
    }

    this->switchPlayer();
}

void TicTacToeWidget::on_restartButton_clicked()
{
    this->reset();
    this->switchPlayer();
}
