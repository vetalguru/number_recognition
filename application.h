// Copyright 2024 Vitalii Shkibtan

#ifndef APPLICATION_H
#define APPLICATION_H

class Application
{
public:
    Application(int argc, char* argv[]);
    ~Application() = default;

    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;

    Application(Application&&) = delete;
    Application&& operator=(Application&&) = delete;

    int run() const noexcept;
};

#endif // APPLICATION_H
