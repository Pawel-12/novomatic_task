#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#if _WIN32
#include <windows.h>
#endif


using namespace std;

class Screen
{
public:
        Screen()
        {
#ifdef _WIN32
                _hConsole= GetStdHandle(STD_OUTPUT_HANDLE);
#endif
                hideCursor();
                saveCursorPosition();
        }


        void clear()
        {
                for(int i = 0; i < height; i++)
                {
                        for(int j = 0; j < width; j++)
                        {
                                _screen[i][j]='x';
                        }
                }
        }


        void draw(char value, float x, float y)
        {
                int xPos = lround(x);
                int yPos = lround(y);
                if(xPos < width && yPos < height)
                {
                        _screen[yPos][xPos]=value;
                }
        }


        void flush()
        {
                restoreCursorPosition();

            cout<<'-';
                for(int i = 0; i < width; i++)
                    cout<<i%10;
                cout<<endl;

                for(int i = 0; i < height; i++)
                {
                        cout<<i%10;
                        for(int j = 0; j < width; j++)
                        {
                                if(_screen[i][j]!='x')
                                {
                                        setRedFont();
                                }
                                else
                                {
                                        setWhiteFont();
                                }
                                cout << _screen[i][j];
                        }
                        cout << "\n";
                }
        }


private:
        void hideCursor()
        {
#ifdef _WIN32
                CONSOLE_CURSOR_INFO cursorInfo; 
                GetConsoleCursorInfo(_hConsole, &cursorInfo);
                cursorInfo.dwSize = 1;
                cursorInfo.bVisible = FALSE;
                SetConsoleCursorInfo(_hConsole, &cursorInfo);
#else
                cout << "\33[?25l";//disable cursor
#endif


        }


        void saveCursorPosition()
        {
#ifdef _WIN32
                CONSOLE_SCREEN_BUFFER_INFO coninfo;
                GetConsoleScreenBufferInfo(_hConsole, &coninfo);
                _prevPos = coninfo.dwCursorPosition;
#else
                cout << "\33[s";
#endif
                
        }


        void restoreCursorPosition()
        {
#ifdef _WIN32
                SetConsoleCursorPosition(_hConsole, _prevPos);
#else
                cout << "\338";
#endif
        }


        void setRedFont()
        {
#ifdef _WIN32
                WORD attributes = FOREGROUND_RED;
                SetConsoleTextAttribute(_hConsole, attributes);
#else
                cout << "\33[31m";//red color
#endif
        }


        void setWhiteFont()
        {
#ifdef _WIN32
                WORD attributes = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
                SetConsoleTextAttribute(_hConsole, attributes);
#else
                cout << "\33[37m";//white color
#endif
        }


        static constexpr int width{60};
        static constexpr int height{20};
        char _screen[height][width];
#ifdef _WIN32
        HANDLE _hConsole;
        COORD _prevPos;
#endif
};


struct Position
{
        float x;
        float y;
};


using Pixel = char;


class Engine
{
public:
        void move(double xpersec, double ypersec, const std::chrono::duration<double, std::milli> & deltaTime)
        {
            if(xpersec != 0.0)
            {
                position.x += (xpersec/1000.0) * (deltaTime.count());
            }
            if(ypersec != 0.0)
            {
                position.y += (ypersec/1000.0) * (deltaTime.count());
            }
        }

        bool walkpath(const vector<Position> & points, const vector<double> & times, const std::chrono::duration<double, std::milli> & deltaTime)
        {
            static unsigned int step = 0;
            static double xs = points[step].x - position.x;
            static double ys = points[step].y - position.y;

            static std::chrono::duration<double, std::milli> time = 0.0ms;
            time += deltaTime;

            move( xs/times[step], ys/times[step], deltaTime);

            if(xs < 0)
            {
                if(position.x < 0.0)
                    position.x = 0.0;
            }
            else if(position.x > points[step].x)
            {
                    position.x = points[step].x;
            }

            if(ys < 0)
            {
                if(position.y < 0.0)
                    position.y = 0.0;
            }
            else if(position.y > points[step].y)
            {
                    position.y = points[step].y;
            }


            if(position.x == points[step].x && position.y == points[step].y)
            {
                for( unsigned int i = 0; i <= step; i++)
                    cout<<endl;

                cout<<endl<< position.x << " " << position.y << " "<< time.count()/1000.0<< endl;

                step++;

                if (step >= points.size()) {
                    step = 0;
                    xs = points[step].x - position.x;
                    ys = points[step].y - position.y;
                    time = 0.0ms;
                    return true;
                }

                xs = points[step].x - position.x;
                ys = points[step].y - position.y;
                time = 0.0ms;
            }
            return false;
        }

        void update(const std::chrono::duration<double, std::milli> & deltaTime )
        {
            static int loops = 0;

            if(loops < 5)
            if(walkpath({{40,0},{40,10},{0,0}},{2,3,1}, deltaTime))
                std::cout<<++loops;
        }


        void render(Screen& screen)const
        {
                screen.draw(pixel, position.x, position.y);
        }
private:
        Pixel pixel{1};
        Position position{0.0f, 0.0f};
};


int main()
{
        Engine engine;
        Screen screen;

        auto startTime = chrono::high_resolution_clock::now();

        while(true)
        {
                auto now = chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli>deltaTime(now - startTime);

                if( deltaTime >= 20ms )
                {
                        screen.clear();
                        engine.update(deltaTime);
                        engine.render(screen);
                        screen.flush();
                        startTime = now;
                }
        }


        return 0;
}