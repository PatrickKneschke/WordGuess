
#include "wordguess.h"
#include "ui_wordguess.h"


WordGuess::WordGuess(QWidget *parent) :
	QWidget(parent),
	ui(new UI::WordGuess),
	secretWord(""),
	attempt(0),
	currLetter(0)
{
	ui->setupUI(this);
}


WordGuess::~WordGuess() {
	delete ui;
}


void WordGuess::keyPressEvent(QKeyEvent *event) {
	int key = event->key();
	if(key >= Qt::Key_A && key <= Qt::Key_Z) {
		addLetterToBoard(event->text().toUpper());
		return;
	}
	if(key == Qt::Key_Backspace) {
		removeLetterFromBoard();
		return;
	}
	if(key == Qt::Key_Return || key == Qt::Key_Enter) {
		evaluateBoard();
		return;
	}
	
	QWidget::keyPressEvent(event);
}


void WordGuess::nextSecretWord() {
	
}


void WordGuess::addLetterToBoard(QString s) {
	if(currLetter < ui->wordLength) {
		ui->boardLabel[attempt*ui->wordLength + currLetter]->setText(s);
		++currLetter;
	}
}


void WordGuess::removeLetterFromBoard() {
	if(currLetter > 0)
		--currLetter;
	ui->boardLabel[attempt*ui->wordLength + currLetter]->setText("");
}


void WordGuess::evaluateBoard() {

}


void WordGuess::revealWord() {

}


void WordGuess::reset() {

}
