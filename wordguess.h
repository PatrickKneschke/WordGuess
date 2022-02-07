#ifndef WORDGUESS_H
#define WORDGUESS_H


#include <QKeyEvent>
#include <QWidget>


namespace UI {
	class WordGuess;
}

class WordGuess : public QWidget {
public:
	WordGuess(QWidget *parent = nullptr);
	~WordGuess();
	
	void keyPressEvent(QKeyEvent *event) override;

private:
	void nextSecretWord();
	void addLetterToBoard(QString s);
	void removeLetterFromBoard();
	void evaluateBoard();
	void revealWord();
	void reset();

	UI::WordGuess *ui;	
	QString 	  secretWord;
	unsigned int  attempt,
				  currLetter;
};


#endif
