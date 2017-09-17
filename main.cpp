#include "Fenetre.hpp"
#include "Fen2.hpp"
#include "P.hpp"

void multiFenetreTest();
void fenetre3DTest();
void ThreadTest();

int main(int argc, char* argv[])
{
    //ThreadTest();
    //fenTest();
    
    fenetre3DTest();
    
	return 0;
}

void multiFenetreTest(){
    //Event event;

    std::vector<std::shared_ptr<Fen2>> tab;
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
    }
}

void fenetre3DTest(){
    Fenetre f("Test", 800, 600, 3, 3);
    f.setDefaultCloseOperation(EXIT_ON_CLOSE);
    f.setVisible(true);
    f.run();
}

void ThreadTest(){
    P p;
    for(int j = 0;j<2;j++){
        p.start();
        for(int i=0;i<p.max+1;i++){
            p.inc();
        }
        p.join();
    }
}