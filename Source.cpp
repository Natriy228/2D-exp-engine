#include <SFML/Graphics.hpp>
#include "Objects.h"

//engine
bool stopped = false;
float LowestFPS = 999;
float HigestFPS = 0;

//global settings
double FPS = 60;
long resolutionWidth = 800;
long resolutionHeight = 600;

//animation settings
bool Animate = false;
unsigned long CurrentFrame = 1;
int breakpoint = 300;

//gameplay settings
bool VSync = true;

//camera
double CameraSpeed = 0;
Vector CameraPosition = Vector(0, 0);

//threads
void UpdateTimelines() {
    while (stopped == false) {
        sf::Clock g;
        for (Timeline* i : tms) {
            i->update(g.getElapsedTime().asSeconds());
        }
        g.restart();
    }
}

sf::Vector2f Convert(Vector v) {
    return sf::Vector2f(v.X, v.Y);
}

int main()
{
    srand(time(NULL));
    sf::RenderWindow window;
    window.create(sf::VideoMode(resolutionWidth, resolutionHeight), "0");
    e_log.write("window created");
    /*
    ObjectPointer a = ActiveWorld.spawn<Character>(CreateRect(-20, -20, 20, 20, 0, 255, 0, 255), 250, 539, 0, 1, "phys", "chr");

    Mesh m = CreateRect(-12, -12, -3, 0, 255, 0, 0, 255);
    m.CollisionEnabled = false;
    a.get()->AddMesh(m);

    m = CreateRect(12, -12, 3, 0, 255, 0, 0, 255);
    m.CollisionEnabled = false;
    a.get()->AddMesh(m);

    m = CreateRect(-12, 8, 12, 12, 255, 0, 0, 255);
    m.CollisionEnabled = false;
    a.get()->AddMesh(m);

    ActiveWorld.spawn<Object>(CreateRect(-400, -20, 400, 20, 255, 0, 0, 255), 400, 580, 0, 1, "phys", "mpl");
    for (int i = 0; i < 6; i++) {
        ActiveWorld.spawn<Object>(CreateRect(-60, -20, 60, 20, 255, 0, 0, 255), 70 + rand() % 530, 60 + 85 * i, 0, 1, "phys", "pl");
    }
    */
    sf::Thread thr(UpdateTimelines);
    if (not(Animate)) {
        thr.launch();
    }

    /*
    std::string n;

    std::ifstream qtr;
    qtr.open("sys//options.txt");
    std::getline(qtr, n);
    CameraSpeed = std::stod(n);

    std::ifstream str;
    str.open("data//Qlist.txt");

    getline(str, n);
    int r = stoi(n);
    for (int i = 0; i < r; i++) {
        getline(str, n);
        double x = stod(n);
        getline(str, n);
        double y = stod(n);
        getline(str, n);
        double q = stod(n);
        ActiveWorld.spawn<Charge>(CreateNangle(8, 10, 0, 0, 255, 255), x, y, 0, 1.4, "phys", std::to_string(i)).get()->update(0, "setq", std::to_string(q));
    }
    */

    //ActiveWorld.spawn<Object>(CreateRect(-20, -20, 20, 20, 255, 0, 0, 255), 40, 40, 0, 1, "phys", "lox1");
    //ActiveWorld.spawn<Object>(CreateRect(-20, -20, 20, 20, 0, 0, 255, 255), 90, 90, 0, 1, "phys", "lox2");

    /*
    for (int i = 0; i < 80; i++) {
        ActiveWorld.spawn<Object>(CreateNangle(rand() % 9 + 1, 40, rand() % 255, rand() % 255, rand() % 255, 255), rand() % 1024, rand() % 1024, rand() % 360, 1, "phys", "lox2");
    }
    */

    Mesh m = ReadMesh("C://Users//Drive//Desktop//CPPInterface//Result.txt");
    ActiveWorld.spawn<Object>(m, 100, 300, 0, 0.4, "phys", "1");

    //ChangeWorld("data/w1.txt");

    sf::ConvexShape sh;
    double l_timer = 0;
    bool once = false;
    sf::Clock cl;
    if (not(Animate) and VSync) {
        window.setVerticalSyncEnabled(true);
    }
    else {
        window.setFramerateLimit(FPS);
    }
    
    while (window.isOpen() and not(Animate))
    {
        double tick =  1 / cl.getElapsedTime().asSeconds();
        for (int i = 0; i < ActiveWorld.objects.size(); i++) {
            if (i < ActiveWorld.objects.size()) ActiveWorld.objects[i].get()->update(cl.getElapsedTime().asSeconds());
        }
        if (!once) {
            l_timer += cl.getElapsedTime().asSeconds();
            window.setTitle(std::to_string(l_timer) + " seconds");
        }
        cl.restart();

        //int xa = 0, ya = 1;
        sf::Event event;
        ObjectPointer sel;
        sel = nullptr;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::K) {
                    sf::Vector2u windowSize = window.getSize();
                    sf::Texture texture;
                    texture.create(windowSize.x, windowSize.y);
                    auto& windowe = static_cast<sf::RenderWindow&>(window);
                    texture.update(windowe);
                    sf::Image screenshot = texture.copyToImage();
                    screenshot.saveToFile("C://Users//Drive//Desktop//screenshot.png");
                }
                /*
                if (event.key.code == sf::Keyboard::R) {
                    bool trg = true;
                    while (trg) {
                        trg = false;
                        for (std::string& i : ActiveWorld.names) {
                            if (i == "pl") {
                                trg = true;
                            }
                        }
                        if (trg) {
                            ActiveWorld.remove("pl");
                        }
                    }
                    for (int i = 0; i < 6; i++) {
                        ActiveWorld.spawn<Object>(CreateRect(-60, -20, 60, 20, 255, 0, 0, 255), 70 + rand() % 530, 60 + 90 * i, 0, 1, "phys", "pl");
                    }
                }
                */
            }
            if (event.type == sf::Event::MouseMoved) {
                
            }
        }

        /*
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) and !once) {
            for (int i = 0; i < ActiveWorld.objects.size(); i++) {
                ActiveWorld.objects[i].get()->update(0, "toggle", "");
            }
            once = true;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P) and once) {
            for (int i = 0; i < ActiveWorld.objects.size(); i++) {
                ActiveWorld.objects[i].get()->update(0, "toggle", "");
            }
            once = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            CameraPosition.X -= CameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            CameraPosition.X += CameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            CameraPosition.Y += CameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            CameraPosition.Y -= CameraSpeed;
        }
        */

        /*
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            xa -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            xa += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) or sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            ya -= 2;
        }
        ActiveWorld.FindByName("chr").get()->update(0, "control", std::to_string(xa) + " " + std::to_string(ya));
        */
        //ActiveWorld.FindByName("lox2").get()->ChangeLocation(sf::Mouse::getPosition().x - window.getPosition().x, sf::Mouse::getPosition().y - window.getPosition().y);
        //SimpleBoxCollision(ActiveWorld.objects[0].get()->GetPoints(0), ActiveWorld.objects[1].get()->GetPoints(0));
        /*
        if (ActiveWorld.FindByName("chr").get()->GetLocation().Y < -10) {
            bool trg = true;
            while (trg) {
                trg = false;
                for (std::string& i : ActiveWorld.names) {
                    if (i == "pl") {
                        trg = true;
                    }
                }
                if (trg) {
                    ActiveWorld.remove("pl");
                }
            }
            for (int i = 0; i < 6; i++) {
                ActiveWorld.spawn<Object>(CreateRect(-60, -20, 60, 20, 255, 0, 0, 255), 70 + rand() % 530, 60 + 90 * i, 0, 1, "phys", "pl");
            }
            ActiveWorld.FindByName("chr").get()->ChangeLocation(250, 540);
        }

        if (ActiveWorld.FindByName("chr").get()->GetLocation().Y > 700) {
            ActiveWorld.FindByName("chr").get()->ChangeLocation(250, 540);
        }
        */

        ActiveWorld.FindByName("1").get()->ChangeLocation(0.5, 0, true);

        window.clear();
        for (ObjectPointer o : ActiveWorld.objects) {
            for (int b = 0; b < o.get()->GetMeshesCount(); b++) {
                int cu = 0;
                sh.setPointCount(o.get()->GetPoints(b).size());
                sh.setFillColor(sf::Color(o.get()->GetMesh(b)->color[0], o.get()->GetMesh(b)->color[1], o.get()->GetMesh()->color[2], o.get()->GetMesh()->color[3]));
                for (Vector &c : o.get()->GetPoints(b)) {
                    sh.setPoint(cu, Convert(c + CameraPosition));
                    cu++;
                }
                window.draw(sh);
            }
        }
        window.display();
    }
    //animation mode
    while (window.isOpen() and Animate) {
        window.clear();

        for (ObjectPointer o : ActiveWorld.objects) {
            for (int b = 0; b < o.get()->GetMeshesCount(); b++) {
                int cu = 0;
                sh.setPointCount(o.get()->GetPoints(b).size());
                sh.setFillColor(sf::Color(o.get()->GetMesh(b)->color[0], o.get()->GetMesh(b)->color[1], o.get()->GetMesh()->color[2], o.get()->GetMesh()->color[3]));
                for (Vector& c : o.get()->GetPoints(b)) {
                    sh.setPoint(cu, Convert(c));
                    cu++;
                }
            }
            window.draw(sh);
        }

        sf::Vector2u windowSize = window.getSize();
        sf::Texture texture;
        texture.create(windowSize.x, windowSize.y);
        auto& windowe = static_cast<sf::RenderWindow&>(window);
        texture.update(windowe);
        sf::Image screenshot = texture.copyToImage();
        screenshot.saveToFile("C://Users//Drive//Desktop//Anim//" + std::to_string(CurrentFrame) + ".png");
        CurrentFrame += 1;
        for (int i = 0; i < ActiveWorld.objects.size(); i++) {
            ActiveWorld.objects[i].get()->update(1.0 / FPS);
        }
        for (Timeline* i : tms) {
            i->update(1.0 / FPS);
        }
        window.setTitle(std::to_string(CurrentFrame - 1));
        window.display();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (CurrentFrame - 1 == breakpoint) {
            window.close();
        }
    }
    stopped = true;
    if (not(Animate)) {
        e_log.write(std::to_string(HigestFPS) + " - higest");
        e_log.write(std::to_string(LowestFPS) + " - lowest");
    }
    return 0;
}