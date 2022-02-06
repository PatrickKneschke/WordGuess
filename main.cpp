

#include "wordguess.h"
#include <QApplication>


int main(int argc, char * argv[]) {
	QApplication app(argc, argv);
	
	WordGuess game;
	game.show();
	
	return app.exec();
}
