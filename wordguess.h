#ifndef WORDGUESS_H
#define WORDGUESS_H


#include <QSize>
#include <QWidget>


namespace UI {
	class WordGuess;
}

class WordGuess : public QWidget {
public:
	WordGuess(QWidget *parent = nullptr);
	~WordGuess();

private:
	UI::WordGuess *ui;
};


#endif
