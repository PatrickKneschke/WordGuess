
#include "wordguess.h"
#include "ui_wordguess.h"


WordGuess::WordGuess(QWidget *parent) :
	QWidget(parent),
	ui(new UI::WordGuess)
{
	ui->setupUI(this);
}


WordGuess::~WordGuess() {
	delete ui;
}

