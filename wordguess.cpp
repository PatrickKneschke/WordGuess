
#include "wordguess.h"
#include "ui_wordguess.h"

#include <QDebug>
#include <QTextStream>

#include <algorithm>
#include <random>


WordGuess::WordGuess(QWidget *parent) :
	QWidget(parent),
	ui(new UI::WordGuess),
	secretWord(""),
	attempt(0),
	currLetter(0),
	gameOver(false)
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
		if(gameOver)
			reset();
		else
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
	
	// hide answer frame
	ui->answerFrame->hide();
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
	if(currLetter < UI::wordLength) {
		ui->boardLabel[attempt*UI::wordLength + currLetter]->setText(s);
		++currLetter;
	}
}


void WordGuess::removeLetterFromBoard() {
	if(currLetter > 0)
		--currLetter;
	ui->boardLabel[attempt*UI::wordLength + currLetter]->setText("");
}


void WordGuess::evaluateBoard() {
	// check valid word length	
	if(currLetter < UI::wordLength)
		return;
		
	// check if word is allowed
	QString guessWord = "";
	for(int i=0; i<UI::wordLength; i++) {
		guessWord += ui->boardLabel[attempt*UI::wordLength + i]->text();
	}	
	if( !std::binary_search(allowed.begin(), allowed.end(), guessWord.toLower()) &&
		!std::binary_search(answers.begin(), answers.end(), guessWord.toLower()) ) {
		return;
	}
	
	// check for letter matches
	int matches = 0;	
	for(size_t i=0; i<UI::wordLength; i++) {
		if(guessWord.at(i) == secretWord.at(i)) {
			++matches;
			ui->boardLabel[attempt*UI::wordLength + i]->setStyleSheet(UI::matchStyle);
			ui->letterLabel[guessWord.at(i).unicode()-'A']->setStyleSheet(UI::matchStyle);
		}
		else if(secretWord.contains(guessWord.at(i))) { 
			ui->boardLabel[attempt*UI::wordLength + i]->setStyleSheet(UI::presentStyle);
			ui->letterLabel[guessWord.at(i).unicode()-'A']->setStyleSheet(UI::presentStyle);
		}
		else {
			ui->boardLabel[attempt*UI::wordLength + i]->setStyleSheet(UI::notPresentStyle);
			ui->letterLabel[guessWord.at(i).unicode()-'A']->setStyleSheet(UI::notPresentStyle);
		}
	}
	
	currLetter = 0;	
	++attempt;
	if(matches == UI::wordLength || attempt >= UI::maxAttempts) {
		revealWord();
		gameOver = true;
	}
}


void WordGuess::revealWord() {
	ui->answerFrame->show();
}


void WordGuess::reset() {
	for(int i=0; i<26; i++)
		ui->letterLabel[i]->setStyleSheet(UI::defaultStyle);
	for(int i=0; i<UI::maxAttempts*UI::wordLength; i++) {
		ui->boardLabel[i]->setText("");
		ui->boardLabel[i]->setStyleSheet(UI::defaultStyle);
	}
	ui->answerFrame->hide();
	
	attempt = 0;
	currLetter = 0;
	gameOver = false;
	
	nextSecretWord();
}
