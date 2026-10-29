#pragma once

class Screen
{
public:
    Screen();
    ~Screen();

    void call(int callerID);

private:
    int m_Environment;
    int m_Surface;
};