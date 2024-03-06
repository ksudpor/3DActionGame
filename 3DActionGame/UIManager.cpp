#include "UIManager.h"
#include "Screen.h"

void UIManager::DrawRectangleByPosition(float left, float top, float right, float bottom, GScolor* color){
    gsDrawRectangle2D(new GSrect(Screen::Width * left, Screen::Height * top,
        Screen::Width * right, Screen::Height * bottom), color);
}

void UIManager::DrawRectangleBySize(float left, float top, float width, float height, GScolor* color){
    gsDrawRectangle2D(new GSrect(Screen::Width * left, Screen::Height * top,
        Screen::Width * (left + width / 1920.0f), Screen::Height * (top + height / 1080.0f)), color);
}

void UIManager::DrawSprite(GSuint id, float left, float top, GScolor* color){
    gsDrawSprite2D(id, new GSvector2(Screen::Width * left, Screen::Height * top), NULL, NULL, color, &Screen::Scale, 0.0f);
}

void UIManager::DrawSpriteRect(GSuint id, float left, float top, GSrect* rect, GScolor* color) {
    gsDrawSprite2D(id, new GSvector2(Screen::Width * left, Screen::Height * top), rect, NULL, color, &Screen::Scale, 0.0f);
}

BoundingRectangle& UIManager::CreateButtonByPosition(float left, float top, float right, float bottom){
    BoundingRectangle B{ GSvector2{Screen::Width * left, Screen::Height * top}, GSvector2{Screen::Width * right, Screen::Height * bottom} };
    return B;
}

BoundingRectangle& UIManager::CreateButtonBySize(float left, float top, float width, float height){
    BoundingRectangle B{ GSvector2{Screen::Width * left, Screen::Height * top}, GSvector2{Screen::Width * (left + width / 1920.0f), Screen::Height * (top + height / 1080.0f)} };
    return B;
}