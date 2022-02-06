#ifndef UI_WORDGUESS_H
#define UI_WORDGUESS_H


#include <QFrame>
#include <QLabel>
#include <QGridLayout>


namespace UI {

class WordGuess {
public:
	void setupUI(QWidget *parent = nullptr) {
		mainLayout   = new QGridLayout();
		
		boardFrame 	 = new QFrame(parent);
		boardFrame->setFrameStyle(QFrame::Box);
		boardFrame->setLineWidth(3);
		boardLayout  = new QGridLayout(boardFrame);
		letterFrame  = new QFrame(parent);
		letterLayout = new QGridLayout(letterFrame);
		answerFrame  = new QFrame(parent);
		answerLayout = new QGridLayout(answerFrame);
		
		// create and place board labels
		for(int i=0; i<6*5; i++) {
			boardLabel[i] = makeLabel(boardFrame, "", QFrame::Panel | QFrame::Sunken, 100, 30);
			boardLayout->addWidget(boardLabel[i], i/5, i%5);
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
		for(int i=0; i<5; i++) {
			answerLabel[i] = makeLabel(answerFrame, "", QFrame::Panel | QFrame::Raised, 100, 30);
			answerLayout->addWidget(answerLabel[i], 1, 2*i, 2, 2);
		}
		messageLabel = makeLabel(answerFrame, "Correct Answer:", QFrame::Plain, 100, 30);
		answerLayout->addWidget(messageLabel, 0, 0, 1, 10);
		
		// place frames in main layout
		parent->setLayout(mainLayout);
		mainLayout->addWidget(boardFrame,  0, 0, 12, 1);
		mainLayout->addWidget(letterFrame, 1, 1, 6, 1);
		mainLayout->addWidget(answerFrame, 8, 1, 4, 1);
		
	}
	
	
	QGridLayout *mainLayout,
				*boardLayout,
				*letterLayout,
				*answerLayout;
	QFrame *boardFrame,
		   *letterFrame,
		   *answerFrame;
	QLabel *boardLabel[6*5],
		   *letterLabel[26],
		   *answerLabel[5],
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
