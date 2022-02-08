
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
	// check valid word length	
	if(currLetter < ui->wordLength)
		return;
		
	// check if word is allowed
	QString guessWord = "";
	for(int i=0; i<ui->wordLength; i++) {
		guessWord += ui->boardLabel[attempt*ui->wordLength + i]->text();
	}
	if( !std::binary_search(allowed.begin(), allowed.end(), guessWord.toLower()) &&
		!std::binary_search(allowed.begin(), allowed.end(), guessWord.toLower()) ) {
		return;
	}
	
	// check for letter matches
	int matches = 0;	
	for(size_t i=0; i<ui->wordLength; i++) {
		if(guessWord.at(i) == secretWord.at(i)) {
			++matches;
			ui->boardLabel[attempt*ui->wordLength + i]->setStyleSheet("QLabel { background-color : rgb(70, 210, 70)}");
			ui->letterLabel[guessWord.at(i).unicode()-'A']->setStyleSheet("QLabel { background-color : rgb(70, 210, 70)}");
		}
		else if(secretWord.contains(guessWord.at(i))) { 
			ui->boardLabel[attempt*ui->wordLength + i]->setStyleSheet("QLabel { background-color : rgb(210, 210, 70)}");
			ui->letterLabel[guessWord.at(i).unicode()-'A']->setStyleSheet("QLabel { background-color : rgb(210, 210, 70)}");
		}
		else {
			ui->boardLabel[attempt*ui->wordLength + i]->setStyleSheet("QLabel { background-color : rgb(100, 100, 100)}");
			ui->letterLabel[guessWord.at(i).unicode()-'A']->setStyleSheet("QLabel { background-color : rgb(100, 100, 100)}");
		}
	}
	
	currLetter = 0;	
	++attempt;
	if(matches == ui->wordLength || attempt >= ui->maxAttempts) {
		revealWord();
		gameOver = true;
	}
}


void WordGuess::revealWord() {
	ui->answerFrame->show();
}


void WordGuess::reset() {
	for(int i=0; i<26; i++)
		ui->letterLabel[i]->setStyleSheet("QLabel { background-color : rgb(200, 200, 200)}");
	for(int i=0; i<ui->maxAttempts; i++) {
		for(int j=0; j<ui->wordLength; j++) {
			ui->boardLabel[i*ui->wordLength + j]->setText("");
			ui->boardLabel[i*ui->wordLength + j]->setStyleSheet("QLabel { background-color : rgb(200, 200, 200)}");
		}
	}
	ui->answerFrame->hide();
	
	attempt = 0;
	currLetter = 0;
	gameOver = false;
	
	nextSecretWord();
}
