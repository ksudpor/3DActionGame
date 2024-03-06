#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include <gslib.h>
#include "BoundingRectangle.h"

class UIManager{
public:
	static void DrawRectangleByPosition(float left, float top, float right, float bottom, GScolor* color);
	static void DrawRectangleBySize(float left, float top, float width, float height, GScolor* color);
	static void DrawSprite(GSuint id, float left, float top, GScolor* color = NULL);
    static void DrawSpriteRect(GSuint id, float left, float top, GSrect* rect, GScolor* color = NULL);
	static BoundingRectangle& CreateButtonByPosition(float left, float top, float right, float bottom);
	static BoundingRectangle& CreateButtonBySize(float left, float top, float width, float height);

    template<typename T>
    static void IncrementEnum(T& enum_variable, int min, int max) {
        int cast = static_cast<int>(enum_variable);
        cast++;
        cast = CLAMP(cast, min, max);
        enum_variable = static_cast<T>(cast);
    }

    template<typename T>
    static void DecrementEnum(T& enum_variable, int min, int max) {
        int cast = static_cast<int>(enum_variable);
        cast--;
        cast = CLAMP(cast, min, max);
        enum_variable = static_cast<T>(cast);
    }

    template<typename T>
    static GScolor& SwitchColor(T enum_variable, T enum_value, GScolor color_default, GScolor color_switch) {
        if (enum_variable == enum_value) {
            return color_switch;
        }
        return color_default;
    }
};

#endif


