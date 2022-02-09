#ifndef UI_WORDGUESS_H
#define UI_WORDGUESS_H


#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QString>


namespace UI {

const int wordLength  = 5;
const int maxAttempts = 6;


const QString defaultStyle = 
	"QLabel { background-color : rgb(200, 200, 200)}";
const QString matchStyle =
	"QLabel { background-color : rgb( 70, 210,  70)}";
const QString presentStyle =
	"QLabel { background-color : rgb(210, 210,  70)}";
const QString notPresentStyle =
	"QLabel { background-color : rgb(100, 100, 100)}";


class WordGuess {
public:
	void setupUI(QWidget *parent = nullptr) {
		mainLayout   = new QGridLayout();
		mainLayout->setColumnStretch(0, 1);
		mainLayout->setColumnStretch(1, 1);
		
		boardFrame 	 = new QFrame(parent);
		boardFrame->setFrameStyle(QFrame::Box);
		boardFrame->setLineWidth(3);
		boardLayout  = new QGridLayout(boardFrame);
		for(int i=0; i<maxAttempts; i++)
			boardLayout->setRowStretch(i, 1);
		for(int i=0; i<wordLength; i++)
			boardLayout->setColumnStretch(i, 1);
		
		letterFrame  = new QFrame(parent);
		letterLayout = new QGridLayout(letterFrame);
		for(int i=0; i<8; i++)
			letterLayout->setRowStretch(i, 1);
		for(int i=0; i<16; i++)
			letterLayout->setColumnStretch(i, 1);
		
		answerFrame  = new QFrame(parent);
		answerLayout = new QGridLayout(answerFrame);
		for(int i=0; i<2; i++)
			letterLayout->setRowStretch(i, 1);
		for(int i=0; i<5; i++)
			letterLayout->setColumnStretch(i, 1);
		
		// create and place board labels
		for(int i=0; i<wordLength*maxAttempts; i++) {
			boardLabel[i] = makeLabel(boardFrame, "", QFrame::Panel | QFrame::Sunken, 100, 30);
			boardLayout->addWidget(boardLabel[i], i/wordLength, i%wordLength);
		}
		
		// create and place letter labels
		int row = 0, col = 0;
		for(int i=0; i<26; i++) {
			letterLabel[i] = makeLabel(letterFrame, QString('A'+i), QFrame::Panel | QFrame::Sunken, 50, 15);
			letterLayout->addWidget(letterLabel[i], 2*row, 2*col + row, 2, 2);
			++col;
			if(col >= 8-row) {
				++row;
				col = 0;
			}
		}
		
		//create and place answer labels
		for(int i=0; i<wordLength; i++) {
			answerLabel[i] = makeLabel(answerFrame, "", QFrame::Panel | QFrame::Raised, 100, 30);
			answerLayout->addWidget(answerLabel[i], 1, i);
		}
		messageLabel = makeLabel(answerFrame, "Correct Answer:", QFrame::Plain, 75, 20);
		answerLayout->addWidget(messageLabel, 0, 0, 1, 5);
		
		// place frames in main layout
		parent->setLayout(mainLayout);
		mainLayout->addWidget(boardFrame,  0, 0, 12, 1, Qt::AlignCenter);
		mainLayout->addWidget(letterFrame, 1, 1,  5, 1, Qt::AlignCenter);
		mainLayout->addWidget(answerFrame, 7, 1,  4, 1, Qt::AlignCenter);
		
		// set default style
		parent->setStyleSheet(defaultStyle);
		
	}
	
		
	QGridLayout *mainLayout,
				*boardLayout,
				*letterLayout,
				*answerLayout;
	QFrame *boardFrame,
		   *letterFrame,
		   *answerFrame;
	QLabel *boardLabel[wordLength*maxAttempts],
		   *letterLabel[26],
		   *answerLabel[wordLength],
		   *messageLabel;

private:
	QLabel* makeLabel(QWidget *parent, const QString &text = "", int frameStyle = QFrame::Plain, int labelSize = 50, int fontSize = 15) 
	{
		QLabel *label = new QLabel(text, parent);
		
		label->setFrameStyle(frameStyle);
		label->setFont(QFont("Arial", fontSize, QFont::Bold));
		label->setAlignment(Qt::AlignCenter);
		label->setMinimumSize(labelSize, labelSize);
		
		return label;
	}
};

} //UI


#endif
