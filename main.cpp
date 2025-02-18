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

    RandomGen rgen;

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

        if (isPressed(K::LShift) ? isPressed(K::Q) : isPressedOnce(K::Q)) {
            
            const fpoint r = 300;
            auto mp = getMousePos(window);

            forcount(100) {

                const fpoint currentR = rgen.randint(0, r);
                const fpoint theta = rgen.randint(0, 360) * 3.1415 / 180.f;

                sim.addBall(Ball(
                    mp + sf::Vector2f(currentR * cos(theta), currentR * sin(theta)),
                    10, i % colourIdMap.size()
                ));



            }
        }






        //sim.simulate_single_threaded();
        sim.simulate_threaded();
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







//void threadTemp(abool& run, bool& go, vectorList<int>& arr, int index) {
//    while (run) {
//        if (go) {
//            go = false;
//            arr[index] += 1;
//        }
//        else {
//            // AAAAAA
//        }
//
//
//    }
//}





int main()
{

    //vectorList<bool*> goList;
    //abool runtemp = true;

    //int n = 5;
    //forcount(n) {
    //    goList.push_back(new bool(false));
    //}

    //vectorList<thread> thListtemp;
    //vectorList<int> arr;
    //forcount(n)
    //    arr.push_back(0);


    //forcount(n) {
    //    thListtemp.emplace_back(thread(
    //        threadTemp, ref(runtemp), ref(*goList[i]),
    //        ref(arr), i
    //    ));
    //}

    //while (runtemp) {
    //    forcount(n)
    //        std::cout << arr[i] << ",";
    //    print("");
    //    if (isPressed(K::A)) {
    //        *goList[2] = true;
    //    }
    //    if (isPressed(K::Q)) {
    //        runtemp = false;
    //    }
    //}


    //for (bool* b : goList)
    //    delete b;






    //return -2;
    
    sf::RenderWindow window(sf::VideoMode({ scrw, scrh }), "SFML works!");
    ForceGetter *fGetterPtr = new ForceGetter();
	Simulation sim = Simulation(window, IntegratorType::Verlet, fGetterPtr, Dt);
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
            fGetterPtr->randomiseForceMaxMatrix(1, 0);
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

    delete fGetterPtr;

}