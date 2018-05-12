#pragma once

class Dusk2Dawn;

class Daytime {
public:
    Daytime();
    ~Daytime();
    bool isDay();

private:
    Dusk2Dawn *m_dusk2dawn;
};
