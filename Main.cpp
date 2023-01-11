#include <SFML/Graphics.hpp>
using namespace sf;

const int num = 8; //Ekrano vietos kuriose kompiuterio masineles keiciasavo pozicija
int momentas[num][2] = { 300, 610,
                      1270,430,
                      1380,2380,
                      1900,2460,
                      1970,1700,
                      2550,1680,
                      2560,3150,
                      500, 3300 };

struct Formule
{
    float x, y, greitis, posukis; 
    int n;

    Formule() { greitis = 3; posukis = 0; n = 0; }

    //Suteikia judejima kompiuterio automobiliams
    void judam()
    {
        x += sin(posukis) * greitis;
        y -= cos(posukis) * greitis;
    }
    //Kompiuterio masinos vaziavimas lenktyniu trasoje
    void raskBota()
    {
        float tx = momentas[n][0];
        float ty = momentas[n][1];
        float beta = posukis - atan2(tx - x, -ty + y);
        if (sin(beta) < 0) posukis += 0.005 * greitis; 
        else posukis -= 0.005 * greitis;
        if ((x - tx) * (x - tx) + (y - ty) * (y - ty) < 25 * 25) n = (n + 1) % num;
    }
};


int main()
{
    //Sukuria zaidimo langa
    RenderWindow app(VideoMode(1500, 1500), "Greiti ir isiute XXI");
    app.setFramerateLimit(60);

    //Zaidimo lauko kurimas
    Texture t1, t2, t3;
    t1.loadFromFile("PNG/trasa.png");
    t2.loadFromFile("PNG/camaro.png");

    //Paveiksliukai kurie bus objektai judantys
    Sprite sBackground(t1), sCar(t2);
    sBackground.scale(2, 2);

    //Nustato pradzios taska automobilio
    sCar.setOrigin(22, 22);

    //Sukurimas mano valdomos masinos ir kompiuterio
    const int IKI = 2;
    Formule masina[IKI];
    for (int i = 0;i < IKI;i++)
    {
        masina[i].x = 300 + i * 50;
        masina[i].y = 1700 + i * 80;
        masina[i].greitis = 9 + i;
    }

    //Mano valdomos masinos parametrai, greitis, posukio greitis, greitejimas, letejimas, stabdymas
    float greitis = 0, posukis = 0;
    float TopGreitis = 10.0;
    float acc = 0.2, dec = 0.3;
    float PosukioGreitis = 0.08;

    //Ekrano ribos
    int ribaX = 0, ribaY = 0;


    //Visi procesai vyksta kol langas yra atidarytas
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }
        //Valdymas pasitelkiant mygtukus
        bool Up = 0, Right = 0, Down = 0, Left = 0;
        if (Keyboard::isKeyPressed(Keyboard::Up)) Up = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) Right = 1;
        if (Keyboard::isKeyPressed(Keyboard::Down)) Down = 1;
        if (Keyboard::isKeyPressed(Keyboard::Left)) Left = 1;

        //Masinos judejimo greitis tiesiai, kampu arba atgal
        if (Up && greitis < TopGreitis)
            if (greitis < 0)  greitis += dec;
            else   greitis += acc;

        if (Down && greitis > -TopGreitis)
            if (greitis > 0) greitis -= dec;
            else  greitis -= acc;

        if (!Up && !Down)
            if (greitis - dec > 0) greitis -= dec;
            else if (greitis + dec < 0) greitis += dec;
            else greitis = 0;

        if (Right && greitis != 0)  posukis += PosukioGreitis * greitis / TopGreitis;
        if (Left && greitis != 0)   posukis -= PosukioGreitis * greitis / TopGreitis;

        //Visi kintamieji yra atkartojami sukurtai kompiuterio valdomai masinai
        masina[0].greitis = greitis;
        masina[0].posukis = posukis;

        //Pasitelkiamas void kuris judina masinas
        for (int i = 0;i < IKI;i++) masina[i].judam();

        //Fokusuoja kamera ant objekto
        for (int i = 1;i < IKI;i++) masina[i].raskBota();


        //Fono vaizdas
        app.clear(Color::Blue);

        //Neleidzia kamerai iseiti is ekrano ribu
        if (masina[0].x > 310) ribaX = masina[0].x - 320;
        if (masina[0].y > 220) ribaY = masina[0].y - 240;

        //Nustato ribas
        sBackground.setPosition(-ribaX, -ribaY);
        app.draw(sBackground);

        Color masinosSpalvos[5] = { Color::White, Color::Red};

        for (int i = 0;i < IKI;i++)
        {
            sCar.setPosition(masina[i].x - ribaX, masina[i].y - ribaY);
            sCar.setRotation(masina[i].posukis * 180 / 3.141593);
            sCar.setColor(masinosSpalvos[i]);
            app.draw(sCar);
        }

        app.display();
    }

    return 0;
}