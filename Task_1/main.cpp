#include <SDL2/SDL.h>
#include <vector>
#include <array>
#include <cmath>
#include <iostream>

struct vec2
{
    float x, y;
    vec2 operator+(const vec2 & v2 )const
    {
        return vec2{x + v2.x, y + v2.y};
    }
};

struct triangle
{
    std::array<vec2, 3> points;
    std::vector< SDL_Vertex > verts;

    triangle(vec2 a, vec2 b, vec2 c, SDL_Color color = {255, 0, 0, 255}) : points{a,b,c}
    {
         verts =
                {
                        { SDL_FPoint{ a.x, a.y}, color, SDL_FPoint{ 0 }, },
                        { SDL_FPoint{ b.x, b.y }, color, SDL_FPoint{ 0 }, },
                        { SDL_FPoint{ c.x, c.y }, color, SDL_FPoint{ 0 }, }
                };
    }

    void copy_to_render(SDL_Renderer * rend)const
    {
        SDL_RenderGeometry( rend, nullptr, verts.data(), verts.size(), nullptr, 0 );
    }

    void move(int x, int y, int xrelative, int yrelative)
    {
        float xsum = 0;
        float ysum = 0;
        for(const vec2 & v : points)
        {
            xsum += v.x;
            ysum += v.y;
        }

        vec2 diff{x - (xsum/3), y - (ysum/3)};

        *this = triangle(points[0] + diff, points[1] + diff, points[2] + diff, {0, 255, 0, 255});
    }

    void rotate(int angle)
    {
        float xsum = 0;
        float ysum = 0;

        for(const vec2 & v : points)
        {
            xsum += v.x;
            ysum += v.y;
        }

        xsum /= 3;
        ysum /= 3;

       double c = cos(angle * M_PI /180);
       double s = sin(angle * M_PI /180);

        for(vec2 & v : points)
        {
            float tx = v.x - xsum;
            float ty = v.y - ysum;

            v.x = floor(tx * c + ty * s) + xsum;
            v.y = floor(-tx * s + ty * c) + ysum;
        }
        *this = triangle(points[0] , points[1] , points[2] , {0, 255, 0, 255});
    }

    void setcolor(SDL_Color color)
    {
        for(auto & v :verts)
            v.color = color;
    }
};

bool lines_collision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    float uA = ((x4-x3) * (y1-y3) - (y4-y3) * (x1-x3)) / ((y4-y3) * (x2-x1) - (x4-x3) * (y2-y1));
    float uB = ((x2-x1) * (y1-y3) - (y2-y1) * (x1-x3)) / ((y4-y3) * (x2-x1) - (x4-x3) * (y2-y1));

    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
        return true;

    return false;
}

bool triangleLine_collision(const triangle & t1, float x1, float y1, float x2, float y2)
{
    for (int i = 0, next = 0; i < 3; i++)
    {
        next = i + 1;
        if (next >= 3)
            next = 0;

        if (lines_collision(x1, y1, x2, y2, t1.points[i].x, t1.points[i].y, t1.points[next].x, t1.points[next].y))
            return true;
    }
    return false;
}

bool isColliding( triangle & t1, const triangle & t2)
{
    for (int i = 0, next = 0; i < 3; i++)
    {
        next = i + 1;
        if (next >= 3)
            next = 0;

        if (triangleLine_collision(t2, t1.points[i].x, t1.points[i].y, t1.points[next].x, t1.points[next].y))
        {
            t1.setcolor({0, 0, 255, 255});
            return true;
        }
        else
        {
            double x1 = t1.points[0].x;
            double x2 = t1.points[1].x;
            double x3 = t1.points[2].x;

            double y1 = t1.points[0].y;
            double y2 = t1.points[1].y;
            double y3 = t1.points[2].y;

            double denominator = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
            double a = ((y2 - y3)*(t2.points[0].x - x3) + (x3 - x2)*(t2.points[0].y - y3)) / denominator;
            double b = ((y3 - y1)*(t2.points[0].x - x3) + (x1 - x3)*(t2.points[0].y - y3)) / denominator;
            double c = 1.0 - a - b;

            if(0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1)
            {
                t1.setcolor({0, 0, 255, 255});
                return true;
            }
        }
    }

    t1.setcolor({255, 0, 0, 255});
    return false;
}

int main(int argc, char* argv[])
{
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window* window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN );
    SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    triangle t1{{400,150},{200,450},{600,450}};
    triangle t2{{250,400},{300,450},{400,400}};

    bool running = true;
    while( running )
    {
        SDL_Event ev;
        while( SDL_PollEvent( &ev ) )
        {
            if( ( SDL_QUIT == ev.type ) ||
                ( SDL_KEYDOWN == ev.type && SDL_SCANCODE_ESCAPE == ev.key.keysym.scancode ) )
            {
                running = false;
                break;
            }
            if (ev.type == SDL_MOUSEMOTION)
                t2.move(ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel );

            if(ev.type == SDL_MOUSEWHEEL)
            {
                if(ev.wheel.y > 0) // scroll up
                {
                    t2.rotate(5);
                }
                else if(ev.wheel.y < 0) // scroll down
                {
                    t2.rotate(-5);
                }
            }
        }

        SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
        SDL_RenderClear( renderer );

        std::cout<< isColliding(t1, t2)<<std::endl;

        t1.copy_to_render(renderer);
        t2.copy_to_render(renderer);

        SDL_RenderPresent( renderer );
    }

    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}