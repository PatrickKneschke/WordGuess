
#include "wordguess.h"
#include "ui_wordguess.h"

#include <QDebug>
#include <QTextStream>

#include <random>


WordGuess::WordGuess(QWidget *parent) :
	QWidget(parent),
	ui(new UI::WordGuess),
	secretWord(""),
	attempt(0),
	currLetter(0)
{
	ui->setupUI(this);
	init();
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


void WordGuess::init() {
	// load list of possible answers
	readWordList("wordlist_answers.txt", answers);	
	
	// load list of other allowed words
	readWordList("wordlist_allowed.txt", allowed);
	
	// pull secret word from list
	nextSecretWord();
}


void WordGuess::readWordList(const QString &filename, QVector<QString> &list) {
	QFile file(filename);
	if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&file);
		QString line = in.readLine();
		while(!line.isNull()) {
			list.push_back(line);
			line = in.readLine();
		}
	}
}


void WordGuess::nextSecretWord() {
	// setup RNG
	std::random_device generator;
	std::uniform_int_distribution<int> distribution(0, answers.size()-1);
		
	// pull random word and transfer to answer labels in ui
	secretWord = answers[distribution(generator)].toUpper();
	for(int i=0; i<secretWord.length(); i++)
		ui->answerLabel[i]->setText(QString(secretWord[i]).toUpper());
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
	if(currLetter < ui->wordLength)
		return;
	
	int matches = 0;	
	for(size_t i=0; i<ui->wordLength; i++) {
		QChar curr = ui->boardLabel[attempt*ui->wordLength + i]->text().at(0);	
		if(curr == secretWord.at(i)) {
			++matches;
			ui->boardLabel[attempt*ui->wordLength + i]->setStyleSheet("QLabel { background-color : rgb(70, 210, 70)}");
		}
		else if(secretWord.contains(curr)) {
			ui->boardLabel[attempt*ui->wordLength + i]->setStyleSheet("QLabel { background-color : rgb(210, 210, 70)}");
		}
		ui->letterLabel[curr.unicode()-'A']->setStyleSheet("QLabel { background-color : rgb(100, 100, 100)}");
	}
	
	currLetter = 0;	
	++attempt;
	if(attempt >= ui->maxAttempts)
		revealWord();
	
}


void WordGuess::revealWord() {

}


void WordGuess::reset() {

}
