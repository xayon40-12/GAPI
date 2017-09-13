#include "Fenetre.hpp"
#include "Fen2.hpp"
#include <iostream>
#include <memory>

#include "Tools/Thread.hpp"

class P: public Thread{
private:
    int i;
public:
    int max = 100;
    P(){
        
    }
    
    void inc(){
        lockMutex("i");
        i++;
        sleepMillis(10);
        unlockMutex("i");
    }
    
    void run(){
        i = 0;
        while(i<max){
            lockMutex("i");
            std::cout << "i = " << i << std::endl;
            unlockMutex("i");
        }
    }
};

int main(int argc, char* argv[])
{
    /*P p;
    for(int j = 0;j<2;j++){
        p.start();
        for(int i=0;i<p.max+1;i++){
            p.inc();
        }
        p.join();
    }*/
    
    
    //Event event;
    
    /*std::vector<std::shared_ptr<Fen2>> tab;
    tab.push_back(std::shared_ptr<Fen2>(new Fen2("test", 200, 200)));
    tab[0]->setPosition(100, 100);
    auto toRemove = tab.end();
    while(tab.size()>0){
        for(auto fen = tab.begin();fen != tab.end();fen++){
            if(fen->get()->isClosed())
                toRemove = fen;
            else{
                fen->get()->run();
            }
        }
        
        if(toRemove != tab.end()){ tab.erase(toRemove); toRemove = tab.end(); }
        if(Keyboard::isKeyHeld(SDLK_LCTRL) && Keyboard::isKeyPressed(SDLK_p)){
            tab.push_back(std::shared_ptr<Fen2>(new Fen2("test", 200, 200)));
            tab[tab.size()-1]->setPosition(Mouse::xOnScreen(), Mouse::yOnScreen());
            toRemove = tab.end();
        }
        std::cout << Keyboard::isKeyHeld(SDLK_LCTRL) << std::endl;
    }*/
    
    
    Fenetre f("Test", 800, 600, 3, 3);
    f.setDefaultCloseOperation(EXIT_ON_CLOSE);
    f.setVisible(true);
    f.run();
    
	return 0;
}
