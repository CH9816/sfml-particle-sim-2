#include "Includes.hpp"
#include "integratorh.h"
#include "simulator.hpp"
#include "defines.hpp"
#include "random.hpp"
#include "forcegetter.hpp"
#include "sfmlextras.h"
#include "timer.hpp"


struct TpsClock {
    Timer timer;
    Timer tpsTimer;
    int tps;
    double timePerTick;
    double delay;
    float actualTps;
    long long index;

    TpsClock(int tps) {
        this->tps = tps;
        actualTps = 0;
        timer.start();
        tpsTimer.start();
        timePerTick = 1.f / tps;
        delay = 0;
        index = 0;
    }

    void start() {
        timer.start();
        index++;
    }

    void waitIfNeeded() {
        delay = timePerTick - timer.get();
        if (delay > 0)
            sf::sleep(sf::seconds(delay));
        actualTps = 1.f / tpsTimer.start();
    }
};



void simThread(sf::RenderWindow & window, Simulation & sim, abool & run, int tps) {

    TpsClock clock(tps);

    while (window.isOpen())
    {

        clock.start();



        int i = 0;
        for (K::Key key : {
            K::Num1, K::Num2, K::Num3
        }) {
            if (isPressed(K::LShift) ? isPressed(key) : isPressedOnce(key)) {
                sim.addBall(Ball(
                    getMousePos(window),
                    10,
                    i,
                    1
                ));
            }
            i++;
        }






        sim.simulate();
        //sim.draw();

        if (not run) {
            break;
        }

        clock.waitIfNeeded();

        //print(clock.actualTps);


    }

    window.close();


}


void joinThList(vectorList<thread>& thList, float *progress = nullptr) {
    float i = 0;
    for (thread& th : thList) {
        if (th.joinable()) {
            th.join();
        }
        if (progress != nullptr) {
            (*progress) = i++ / thList.size();
        }
    }
}













int main()
{


    //return -2;
    
    sf::RenderWindow window(sf::VideoMode({ scrw, scrh }), "SFML works!");
    ForceGetter fGetter = ForceGetter();
	Simulation sim = Simulation(window, IntegratorType::Verlet, &fGetter, Dt);
	RandomGen rand = RandomGen();

    window.setFramerateLimit(FPS);


    vectorList<thread> thList;

    abool run = true;
    thList.push_back(
        thread(simThread, ref(window), ref(sim), ref(run), TPS)
    );




    //forcount(0) {
    //    Ball ball = Ball(
    //        Vector(100 + rand.get() * 100, 100 * rand.get()),
    //        10,          // radius
    //        0          // colour
    //    );
    //    ball.setPosition(100, 100);
    //    ball.setFillColor(sf::Color::Green);

    //    sim.addBall(ball);

    //}

    TpsClock clock(FPS);

    while (run) {

        clock.start();

        if (not handleEvents(window))
            break;

        sim.draw();

        if (isPressed(K::R)) {
            fGetter.randomiseForceMaxMatrix(10, 0);
        }
        if (isPressed((K::Key(2)))) // C taken
            sim.clearObjects();


        if (clock.index % 30 == 0) {
            for (std::string& string : sim.timeList) {
                if (string != "NULL")
                    print(string);
            }
        }
        
        clock.waitIfNeeded();



    }

    run = false;
    window.close();


    joinThList(thList);

    

}