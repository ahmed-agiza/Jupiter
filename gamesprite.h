#ifndef GAMESPRITE_H
#define GAMESPRITE_H
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class GameSprite
{
public:
    GameSprite();
    ~GameSprite();
    char loadByte(unsigned int) const;
    void storeByte(unsigned int, char);
private:
    //Image image;
    //Texture texture;
    //Sprite sprite;
    vector<unsigned char> memoryBytes;
};

#endif // GAMESPRITE_H
