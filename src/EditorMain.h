#ifndef EDITOR_MAIN_H_
#define EDITOR_MAIN_H_

#include <SFML/Graphics.hpp>

void initialiseEditor();

void editorMouseClick(int x, int y, bool leftClick);

void editorKeyPress(sf::Keyboard::Key keyCode);

void drawEditorFrame();


void updateEditor();

void tryReturnToEditor();

void editorRestartStage();

#endif